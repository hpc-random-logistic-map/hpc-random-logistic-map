# plotbif.py
# for use with the output of csv2hdf5.py.
# USAGE: plotbif hdf5_input.hdf5 L_value(e.g. 0.1)

import sys
import h5py
import numpy as np
import matplotlib.pyplot as plt

h5f = h5py.File(sys.argv[1], "r")
bif = h5f.get("bifurcation_data")
if bif is None:
    print "your file is not what was expected."
    sys.exit()

# let's just take the first L value for now
L = bif.get("L_" + sys.argv[2])
if L is None:
    print "couldn't find group for L = " + sys.argv[2]
    sys.exit()

x = L.get("x")
if x is None:
    print "couldn't find x."
    sys.exit()

print x

lo_x = hi_x = x[0][0]
lo_y = hi_y = x[0][1]

for coords in x:
    plt.plot(coords[0], coords[1], "ro")
    if lo_x > coords[0]:
        lo_x = coords[0]
    elif hi_x < coords[0]:
        hi_x = coords[0]
    if lo_y > coords[1]:
        lo_y = coords[1]
    elif hi_y < coords[1]:
        hi_y = coords[1]
    print coords

plt.axis([lo_x, hi_x, lo_y, hi_y])
plt.xlabel("r")
plt.ylabel("x value")
plt.show()

h5f.close()