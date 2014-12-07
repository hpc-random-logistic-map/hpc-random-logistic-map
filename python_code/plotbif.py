# plotbif.py
# for use with the output of csv2hdf5.py.
# USAGE: plotbif hdf5_input.hdf5 L_value(e.g. 0.1)

import sys
import h5py
import numpy as np
import matplotlib
matplotlib.use('Agg')
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

#print x

lo_x = hi_x = x[0][0]
lo_y = hi_y = x[0][1]

for coords in x:
    if coords[2] == 1:
        plt.plot(coords[0], coords[1], "wo")
    elif coords[2] == 2:
        plt.plot(coords[0], coords[1], "go")
    elif coords[2] == 3:
        plt.plot(coords[0], coords[1], "mo")
    elif coords[2] == 4:
        plt.plot(coords[0], coords[1], "ro")
    elif coords[2] == 5:
        plt.plot(coords[0], coords[1], "yo")
    elif coords[2] == 6:
        plt.plot(coords[0], coords[1], color="purple", marker="o")
    elif coords[2] == 7:
        plt.plot(coords[0], coords[1], color="limegreen", marker="o")
    elif coords[2] == 8:
        plt.plot(coords[0], coords[1], color="blue", marker="o")
    elif coords[2] == 9:
        plt.plot(coords[0], coords[1], color="orange", marker="o")
    elif coords[2] == 10:
        plt.plot(coords[0], coords[1], color="gray", marker="o")
    elif coords[2] == 11:
        plt.plot(coords[0], coords[1], color="cyan", marker="o")
    elif coords[2] == 12:
        plt.plot(coords[0], coords[1], color="mint", marker="o")
    elif coords[2] == 24:
        plt.plot(coords[0], coords[1], color="brown", marker="o")
    else:
        plt.plot(coords[0], coords[1], "ko")

    if hi_y < coords[1]:
        hi_y = coords[1]

plt.title("Bifurcation of L = "+sys.argv[2])
plt.axis([0, 4, 0, hi_y])
plt.xlabel("r")
plt.ylabel("x value")
plt.savefig("Bifurcation_of_L="+sys.argv[2]+".png")
plt.show()

h5f.close()
