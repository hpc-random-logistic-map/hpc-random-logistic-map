# csv2hdf5.py
# USAGE: csv2hdf5 csv_input.csv hdf5_output.hdf5

import csv
import sys
#import h5py
#import numpy as np
#import matplotlib.pyplot as plt
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
print data
