#!/usr/bin/bash

export OMP_NUM_THREADS
for x in 1 2 4
do
	OMP_NUM_THREADS=$x
	./piprog
done
