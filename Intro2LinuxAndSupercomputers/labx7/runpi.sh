#!/usr/bin/bash
#SBATCH --output=piprog_%j.out
#SBATCH --partition=cpu
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --time=10:00
#SBATCH --mem-per-cpu=10M

export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-4}

time ./piprog
