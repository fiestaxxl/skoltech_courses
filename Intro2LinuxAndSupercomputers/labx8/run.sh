#!/bin/bash
# script to run MPI implementation ./run.sh <number of threads>

module load mpi/openmpi-3.1.2
mpicc -o pim -O3 piprog_MPI.c -lm

NP=${1:-4}
export OMPI_MCA_btl_base_warn_component_unused=0
mpirun -np $NP ./pim
