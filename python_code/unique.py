# csv2hdf5.py
# USAGE: csv2hdf5 csv_input.csv hdf5_output.hdf5

import csv
import sys
#import h5py
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pdb
csvf = open(sys.argv[1], 'r')

csvreader = csv.reader(csvf)
data =[]
maxp = 0
#csvreader.next() # skip first row
#pdb.iset_trace()
for index,row in enumerate(csvreader):
    flag = False
    for i in data:
        if(row == i):
            flag = True
            break
    if (flag == False):
        data.append(row)  
    if (int(row[2])>maxp):
        maxp = int(row[2])
csvf.close()

#print data
count = np.zeros(451)
#pdb.set_trace()
for row in data:
    count[int(row[2])] += 1
#print count
#for i in count:
 #   i = i/len(data)
count = count /len(data)
print maxp
#print len(data)
#plt.xrange(0,451)
plt.bar(range(0,451),count,width=1)
plt.xlim([0,maxp])
plt.ylim([0,1])
plt.yticks(np.arange(0, 1, .1))
plt.xlabel("Period Order")
plt.ylabel("frequency of P_orbit")

plt.savefig("test.png")


