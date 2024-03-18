
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpi4py import MPI


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

start_time = MPI.Wtime()

def step(r,x):
    return r*x*(1-x)


def iterat_r(x0, r_min, r_max, m, n, steps):
    R = np.linspace(r_min,r_max, steps)
    X = list()
    R_list = list()
    for r in R:
        x = x0
        for i in range(m+n):
            if (i>n) and (x not in X):
                X.append(x)
                R_list.append(r)
            x = step(r,x)
    return X, R_list



dots = 1000//size
r_size = 4/size

    
x0 = 0.1
m = 20
n = 100
    

X, R = iterat_r(x0, r_size*(rank), r_size*(rank+1), m, n, dots)
data = comm.gather((X,R), root=0)

if rank == 0:
    R = list()
    X = list()
    for i in range(len(data)):
        R = np.concatenate((R,data[i][1]))
        X = np.concatenate((X,data[i][0])) 
    np.savetxt("X.csv", X, delimiter=",") 
    np.savetxt("R.csv", R, delimiter=",")  
    #print(MPI.Wtime()-start_time)
    print(f"Number of processes: {size} done")





