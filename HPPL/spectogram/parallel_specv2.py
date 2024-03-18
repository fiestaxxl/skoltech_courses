import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


pi = np.pi
t=np.linspace(-20*2*pi, 20*2*pi, 3**9+6)
y=np.sin(t)*np.exp(-t**2/2/20**2)
y=y+np.sin(3*t)*np.exp(-(t-5*2*pi)**2/2/20**2)
y=y+np.sin(5.5*t)*np.exp(-(t-10*2*pi)**2/2/5**2)

def get_specgram(time, signal, size, rank, width = 1.5, osc = 20, nwindowsteps = 1000):
    
    step = nwindowsteps//size
    position = np.linspace(-osc,osc,nwindowsteps)
    specgram = np.empty((len(time),step))
    
    l = rank*step
    r = (rank+1)*step
    
    for ind, val in enumerate(position[l:r]):
        window_width = width*2*np.pi
        window_position = val*2*np.pi
        window_function = np.exp(-(time-window_position)**2/(2*window_width**2))
        y_window = signal*window_function
        
        
        w=np.fft.fftfreq(len(signal), d=(time[1]-time[0])/2/pi)
        dims =[min(time)/(2*np.pi), max(time)/(2*np.pi), w[0], 2*w[int(len(time)/2)-1]] 
        specgram[:,ind] = abs(np.fft.fft(y_window))
    
    return specgram, dims, np.shape(specgram)[0]

spec, dims, height = get_specgram(t, y, size, rank)
print(f"{rank} process is done ")
data = comm.gather((spec,dims), root=0)


if rank == 0:
    dims = data[0][1]
    spec = np.empty((height,0))
    for i in range(len(data)):
        spec = np.concatenate((spec,data[i][0]),axis = 1) 
    print(f"Number of processes: {size}")
    
    np.savetxt("dims.csv", dims, delimiter = ",")
    np.savetxt("spec.csv", spec, delimiter = " ")
	
