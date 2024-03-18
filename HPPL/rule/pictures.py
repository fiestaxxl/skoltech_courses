import matplotlib.pyplot as plt
from mpi4py import MPI
import numpy as np
import matplotlib.cm as cm
from scipy.ndimage import convolve1d
import imageio

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
cartesian = comm.Create_cart(dims = [size],periods =[True],reorder=False)


def neighbours(rank, size):
    left,right = cartesian.Shift(direction = 0,disp=1)
    return left, right

def send_ghost(obj_l, obj_r, func, rank, size):
    left, right = func(rank,size)
    comm.send(obj_l,left)
    comm.send(obj_r, right)

def recv_ghost(func, rank, size):
    left, right = func(rank,size)
    ghost_r = comm.recv(obj_l, right)
    ghost_l = comm.recv(obj_r, left)
    return ghost_l, ghost_r
   
    
N = 4
M = [0,0,0,1,1,1,0,1,1,1]*N
K = [1,1,1]
O = [0,0,1]

def evolution(M, K, O, mode='wrap'):
    kws = dict(mode=mode)
    res = convolve1d(M,K,**kws)
    ones = convolve1d(M,O,**kws)
    
    true_zeros = (res == 1) & (ones == 1)
    res[true_zeros] = 0
    res[res == 3] = 0
    res[res == 2] = 1
    res[res == 0] = 0
    
    return res


def pic(M,name):
    fig, ax = plt.subplots(figsize=(8,6))
    ax.set_aspect('auto')
    ax.imshow(M, interpolation='nearest', cmap=cm.Greys)
    ax.axis('off')
    fig.savefig(name)
    plt.close(fig);

    
step, residue = divmod(len(M),size)

if rank == 0:
    x = len(M)
    y = len(M)
    Result = np.empty((y,x))
    Result[0,:] = M

print(rank, Result)
'''   
if rank != (size-1):
    l_bord = rank*step
    r_bord = (rank+1)*step
else:
    l_bord = rank*step
    r_bord = (rank+1)*step + residue
   
for i in range(1,y):
    X = M[l_bord:r_bord]
'''    

''' 
x = len(M)
y = 100
Results = np.empty((y,x))
Results[0,:] = M

files = list()
for i in range(1,y):
    pic(Results,f"{i+100}_pic.png")
    Results[i,:] = evolution(Results[i-1,:],K,O)
    files.append(f"{i+100}_pic.png")
    
images = list()
for file in files:
    images.append(imageio.imread(file))
imageio.mimsave(f'110.gif', images, fps = 13);  
'''
