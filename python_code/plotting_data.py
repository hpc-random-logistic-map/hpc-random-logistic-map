import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import csv
from itertools import izip
from collections import defaultdict

#define the key as the first element of a tuple
def getKey(item):
    return item[0]

#d = filename
#subtracs the 2nd and 3rd columns to get time elapsed. 2nd col is start time, 3rd col is end time
#returns a sorted list of tuples l = [(nt,y)]
def maketuple(d):
    da = np.genfromtxt(d,delimiter = ',', dtype = float)
    nt = [12*row[0] for row in da]
    for n in range(0,len(nt)):
        if nt[n]==0:
            nt[n]=1.0            
    s = [row[1] for row in da]
    e = [row[2] for row in da]
    t = [a_i - b_i for a_i, b_i in zip(e,s)]
    mytuple = zip(nt,t)
    mytuple = sorted(mytuple,key=getKey)
    return mytuple    

#returns a dictionary for the list of tuples tup
#dict key is the first element of the tuple
def makedict(tup):
    d = defaultdict(list)
    for v,k in tup:
        d[v].append(k)
    return d

#return a list of averages of dicionary values based on the keys nt
def getavg(mydict, nt):
    y = []
    for x in nt:
        y.append(np.mean(mydict[x]))
    return y
    
d_t =  makedict(maketuple('/home/amy/Documents/hpc/Project/hpc-random-logistic-map/python_code/timings.csv'))
nprocs = d_t.keys()  #number of procs
nprocs.sort()
stavg = getavg(d_t, nprocs)   #avg time for each numprocs
stime = stavg[0]    #serial time
col_h = ['processes','Speedup','Efficiency','Serial-Fraction']

# strong speedup calculation 6 threads
ssu = []
for i in range(0,len(stavg)):
    ssu.append(stime/stavg[i])

# strong efficiency calculation
sef = []
for i in range(0,len(stavg)):
    sef.append((stime/stavg[i])/nprocs[i])
    
# karp-flatt metric (serial fraction) calculations
kf = []
for i in range(0,len(ssu)):
    if nprocs[i] == 1.0:
        kf.append(-1)
    else:
        kf.append(((1/ssu[i]) + (1/nprocs[i])) / (1 - (1/nprocs[i])))

# write results to file
a = [list(x) for x in izip(nprocs, ssu, sef, kf)]
roofFile = open("random-logistic.csv",'wb')
wr = csv.writer(roofFile)
wr.writerow(col_h)
wr.writerows(a)

fig,ax = plt.subplots()

# # # strong speedup graph
# ax.semilogy([0,1,200],[0,1,200],c='r',label='ideal speedup',basey=10)
# ax.semilogy(nprocs[1:len(nprocs)],ssu[1:len(ssu)],c='g',marker = ".",markersize=10,lw = 1.3,label='data',basey=10)
# ax.set_ylim(ymax=100)
# lgd = ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
# plt.xlabel('number of processors')
# plt.ylabel('speedup')
# plt.title('Speedup for Random Logistic and Load balancer over 16 nodes\n Intel Xeon CPU X5660 @ 2.8 GHz')
# fig.savefig('speedup_random_logistic.png', bbox_extra_artists=(lgd,), bbox_inches='tight')

# # # strong efficiency
ax.semilogy([0,2,4,6,8,10,200],[1,1,1,1,1,1,1],c='r',label='100%',basey=10)
ax.semilogy(nprocs[1:len(nprocs)],sef[1:len(sef)],c='g',marker = ".",markersize=10,lw = 1.3,label='data',basey=10)
ax.set_ylim(ymax=1.5)    
lgd = ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
plt.xlabel('number of processors')
plt.ylabel('efficiency')
plt.title('Efficiency for Random Logistic and Load balancer over 16 nodes\n Intel Xeon CPU X5660 @ 2.8 GHz')
fig.savefig('efficiency_random_logistic.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
