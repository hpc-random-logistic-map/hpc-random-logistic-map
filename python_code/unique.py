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

#csvreader.next() # skip first row
#pdb.set_trace()
for index,row in enumerate(csvreader):
    #print index
    flag = False
    csvf1 = open(sys.argv[1], 'r')
    csvreader1 = csv.reader(csvf1)
    for index1,row1 in enumerate(csvreader1):
        #print index1
        #print row1
        if index1== index:
            break
        if row1[2] == row[2]:
            if row[3:] == row1[3:]:
                  flag = True
                  break
    csvf1.close()
    if flag == False:
        print row[3:]                                 
csvf.close()
