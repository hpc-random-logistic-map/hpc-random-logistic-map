# csv2hdf5.py
# USAGE: csv2hdf5 csv_input.csv hdf5_output.hdf5

import csv
import sys
import h5py
import numpy as np
import matplotlib.pyplot as plt
import pdb
csvf = open(sys.argv[1], 'r')
csvreader = csv.reader(csvf)
# row format:
# r, L, period, x_0, x_1, ..., x_period-1

h5f = h5py.File(sys.argv[2], "w")

# create archive and bifurcation data groups
arch = h5f.get("archive")
if arch is None:
    arch = h5f.create_group("archive")

bif = h5f.get("bifurcation_data")
if bif is None:
    bif = h5f.create_group("bifurcation_data")

data =[]
#csvreader.next() # skip first row
#pdb.set_trace()
for index,row in enumerate(csvreader):
    print "processing row "+str(index)+":"
    print row
    flag = False
    if(index != 0):
        for i in data:
        #   print i
            if(row == i):
                flag = True
                break
        if (flag == False):
            data.append(row)
csvf.close()
#pdb.set_trace()
count = [0 for x in range(450)]
for row in data:
    print row
    if "e-" not in row[3]:
        count[int(float(row[3]))] += 1
    else:
        count[0] += 1
for row in data:

    rv = row[0]
    Lv = row[1]
    Pv = row[2]
    xv = row[3:]

    # BEGIN ARCHIVE

    r = arch.get("r_" + rv)
    if r is None:
        r = arch.create_group("r_" + rv)
        #print "made group " + r.name

    L = r.get("L_" + Lv)
    if L is None:
        L = r.create_group("L_" + Lv)
        #print "made group " + L.name

    P = L.get("P_" + Pv)
    if P is None:
        P = L.create_group("P_" + Pv)
        #print "made group " + P.name

    period = P.attrs.get("period")
    if period is None:
        P.attrs.create("period", int(Pv), dtype = int)
        period = P.attrs.get("period")
    #print "period = " + str(period)

    x = P.get("x")
    if x is None:
        x = P.create_dataset("x", (0,period), dtype="float", maxshape=(50,period))
        x.dims[0].label = "row"
        x.dims[1].label = "xvals"
    i = x.len()+1
    x.resize((i,period))
    for j in range(0, period):
        # sloppy hack to deal with really tiny numbers in the output
        if "e-" not in xv[j]:
            x[i-1,j] = float(xv[j])
        else:
            x[i-1,j] = 0.0
        #print str(x[j-1,i])

    # END ARCHIVE

    # BEGIN BIFURCATION DATA

    L = bif.get("L_" + Lv)
    if L is None:
        L = bif.create_group("L_" + Lv)
        #print "made group " + L.name

    x = L.get("x")
    if x is None:
        dt = np.dtype([("r","float"),("x_loc","float"),("p","int")])
        x = L.create_dataset("x", (0,), dtype=dt, maxshape=(None,))
    oldlen = x.len()
    newlen = oldlen + period
    x.resize(int(newlen), axis=0)
    for i in range(0, period):
        x[oldlen+i] = (float(rv),float(xv[i]),period)
        #print str(x[oldlen+i])

    # END BIFURCATION DATA

print "Created " + h5f.filename + "."
h5f.close()
