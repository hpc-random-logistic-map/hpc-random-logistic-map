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
#csvreader.next() # skip first row
#pdb.set_trace()
for index,row in enumerate(csvreader):
    #print index
	flag = False
	if(index != 0):
		for i in data:
		#	print i
			if(row == i):
				flag = True
				break
        if (flag == False):
            data.append(row)                         
csvf.close()

#print data
count = np.zeros(451)
#pdb.set_trace()
for row in data:
    count[int(row[2])] += 1

#for i in count:
 #   i = i/len(data)
count = count /len(data)


plt.bar(range(0,451),count)
plt.xlim([0,10])
plt.ylim([0,1])
plt.savefig("test.png")


