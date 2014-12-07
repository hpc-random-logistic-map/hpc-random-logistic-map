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
#k = column in file where y values reside, index starts at 0
#returns a sorted list of tuples l = [(nt,y)]
def maketuple(d,k):
    da = np.genfromtxt(d,delimiter = ',', dtype = float)
    nt = [row[0] for row in da]
    nt = nt[1:len(nt)]
    y = [row[k] for row in da]
    y = y[1:len(y)] #remove header
    mytuple = zip(nt,y)
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
    
d_6t =  makedict(maketuple('/home/amy/Documents/hpc/Le/HW06/OMP-MPI-Jacobi/t6.csv',1))
d_12t =  makedict(maketuple('/home/amy/Documents/hpc/Le/HW06/OMP-MPI-Jacobi/t12.csv',1))
nprocs12 = d_12t.keys()
nprocs12.sort()
nprocs = d_6t.keys()  #number of procs
nprocs.sort()

stavg = getavg(d_6t, nprocs)   #avg time for each numprocs
stime = stavg[0]    #serial time
stavg12 = getavg(d_12t, nprocs12)
stime12 = stavg12[0]    

col_h = ['processes*threads','wall-time/iteration','Speedup','Efficiency','Serial-Fraction']

# strong speedup calculation 6 threads
ssu = []
for i in range(0,len(stavg)):
    ssu.append(stime/stavg[i])

# strong speedup calculation 12 threads
ssu12 = []
for i in range(0,len(stavg12)):
    ssu12.append(stime12/stavg12[i])

    
# strong efficiency calculation
sef = []
for i in range(0,len(stavg)):
    sef.append((stime/stavg[i])/nprocs[i])

# strong efficiency calculation
sef12 = []
for i in range(0,len(stavg12)):
    sef12.append((stime12/stavg12[i])/nprocs12[i])
    
# karp-flatt metric (serial fraction) calculations
kf = []
for i in range(0,len(ssu)):
    if i == 0:
        kf.append(-1)
    else:
        kf.append(((1/ssu[i]) + (1/nprocs[i])) / (1 - (1/nprocs[i])))

# karp-flatt metric (serial fraction) calculations
kf12 = []
for i in range(0,len(ssu12)):
    if i == 0:
        kf12.append(-1)
    else:
        kf12.append(((1/ssu12[i]) + (1/nprocs12[i])) / (1 - (1/nprocs12[i])))


# walltime/iteration
wi = []
niter = 1000.0   # number of iterates 
for i in range(0,len(ssu)):
    wi.append(stavg[i]/niter)

# walltime/iteration
wi12 = []
for i in range(0,len(ssu12)):
    wi12.append(stavg12[i]/niter)
    
# write results to file
a = [list(x) for x in izip(nprocs, wi, ssu, sef, kf)]
roofFile = open("jacobi-hybrid_2mpi-6threads_200x200x200.csv",'wb')
wr = csv.writer(roofFile)
wr.writerow(col_h)
wr.writerows(a)

a12 = [list(x) for x in izip(nprocs12, wi12, ssu12, sef12, kf12)]
roofFile12 = open("jacobi-hybrid_1mpi-12threads_200x200x200.csv",'wb')
wr12 = csv.writer(roofFile12)
wr12.writerow(col_h)
wr12.writerows(a12)


fig,ax = plt.subplots()

# # strong speedup graph
# print nprocs12
# print ssu12
# ax.semilogy([0,1,100],[0,1,100],c='r',label='ideal speedup',basey=10)
# ax.semilogy(nprocs[1:len(nprocs)],ssu[1:len(ssu)],c='g',marker = ".",markersize=10,lw = 1.3,label='6 threads/2 MPI procs',basey=10)
# ax.semilogy(nprocs12[1:len(nprocs12)],ssu12[1:len(ssu12)],c='b',marker = ".",markersize=10,lw = 1.3,label='12 threads/1 MPI proc',basey=10)
# ax.set_ylim(ymax=100)
# lgd = ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
# plt.xlabel('number of threads')
# plt.ylabel('speedup')
# plt.title('Speedup for Hybrid-Jacobi over 16 nodes\n Intel Xeon CPU X5660 @ 2.8 GHz')
# fig.savefig('speedup_jacobi_hybrid.png', bbox_extra_artists=(lgd,), bbox_inches='tight')

# # strong efficiency
print nprocs12
print sef12
ax.semilogy([0,2,4,6,8,10,200],[1,1,1,1,1,1,1],c='r',label='100%',basey=10)
ax.semilogy(nprocs[1:len(nprocs)],sef[1:len(sef)],c='g',marker = ".",markersize=10,lw = 1.3,label='6 threads/2 MPI procs',basey=10)
ax.semilogy(nprocs12[1:len(nprocs12)],sef12[1:len(sef12)],c='b',marker = ".",markersize=10,lw = 1.3,label='12 threads/1 MPI proc',basey=10)
ax.set_ylim(ymax=1.5)    
lgd = ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
plt.xlabel('number of threads')
plt.ylabel('efficiency')
plt.title('Efficiency for Hybrid-Jacobi over 16 nodes\n Intel Xeon CPU X5660 @ 2.8 GHz')
fig.savefig('efficiency_jacobi_hybrid.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
