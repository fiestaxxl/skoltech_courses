from mpi4py import MPI
import os

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

path = os.path.join(os.getcwd(),'data')
files = [file for file in os.listdir(path) if file.endswith('.txt')]

local_max = 0

if size > 1:
    index = rank
else:
    index = 0
    
while index < len(files):
    with open(os.path.join(path, files[index])) as f:
        num = sum(1 for line in f)
    local_max = max(local_max, num)
    index += size
        
result = comm.reduce(local_max, op = MPI.MAX, root = 0)


if rank == 0:
    print ('Max length is equal to', result)
