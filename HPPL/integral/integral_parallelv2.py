import numpy as np
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

def func(x):
    return np.exp(-(x**2))

a = -10**3
b = 10**3
N = 10**5
h = (b-a)/N

steps = N//size
l = rank*steps
r = (rank+1)*steps

def integral(fun, a, b, N, l, r):
    res = list()
    h = (b-a)/N
    x = np.arange(a,b+h,h)
    
    for x_i in x[l:r]:
        res.append(fun(x_i))
    
    result = h*(np.sum(res[1:-1])+(res[0]+res[-1])/2)
    return result

result = integral(func, a, b, N, l, r)
#print(f"process number {rank} and result {result}")

data = comm.gather(result, root=0)

if rank == 0:
    print(sum(data))
