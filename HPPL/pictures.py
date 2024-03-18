import matplotlib.pyplot as plt
from mpi4py import MPI
import numpy as np
import imageio
from copy import deepcopy


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

def pic(M,name):
    fig, ax = plt.subplots()
    ax.imshow(M)
    ax.axis('off')
    #fig.savefig(name)
    plt.close(fig);
   
img = plt.imread(r'C:\Users\ivang\Desktop\123.jpg')   

M = deepcopy(img)
files = list()
shp = np.shape(img)

x,y = divmod(shp[0],size)

M = np.roll(M,rank*x,axis = 0)


for i in range(rank*x,(rank+1)*x):
    pic(M,f'{i+100}animation.png')
    files.append(f'{i+100}animation.png')
    M = np.roll(M,1,axis = 0)


if (y != 0):
    if rank == 0:
        M = deepcopy(img)
        M = np.roll(M,size*x, axis=0)
        for i in range(shp[0]-y,shp[0]):
            pic(M,f'{i+100}animation.png')
            files.append(f'{i+100}animation.png')
            M = np.roll(M,1,axis = 0)

 
data = comm.gather(files)

if rank == 0:
    files = list()
    for i in range(len(data)):
        files = np.concatenate((files,data[i]))
    files = np.sort(files)
    
    #images = list()
    #for file in files:
        #images.append(imageio.imread(file))
    #imageio.mimsave(f'3.gif', images, fps = 60)
    print("all done");
