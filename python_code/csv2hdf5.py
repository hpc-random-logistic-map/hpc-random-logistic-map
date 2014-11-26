# csv2hdf5.py
# USAGE: csv2hdf5 csv_input.csv hdf5_output.hdf5

import csv
import sys
import h5py
# import numpy as np

csvf = open(sys.argv[1], 'r')

# row format:
# r, L, period, x_0, x_1, ..., x_period-1

h5f = h5py.File(sys.argv[2], "w")

csvreader = csv.reader(csvf)
csvreader.next() # skip first row

for row in csvreader:

    r = h5f.get("r_" + row[0])
    if r is None:
        r = h5f.create_group("r_" + row[0])
        print "made group " + r.name

    L = r.get("L_" + row[1])
    if L is None:
        L = r.create_group("L_" + row[1])
        print "made group " + L.name

    P = L.get("P_" + row[2])
    if P is None:
        P = L.create_group("P_" + row[2])
        print "made group " + P.name

    period = P.attrs.get("period")
    if period is None:
        P.attrs.create("period", int(row[2]), dtype = int)
        period = P.attrs.get("period")
    print "period = " + str(period)

    x = P.get("x")
    if x is None:
        x = P.create_dataset("x", (0,period), dtype="float", maxshape=(period,50))
    j = x.len()+1
    x.resize((j,period))
    for i in range(0, period):
        x[j-1,i] = float(row[3+i])
        print str(x[j-1,i])