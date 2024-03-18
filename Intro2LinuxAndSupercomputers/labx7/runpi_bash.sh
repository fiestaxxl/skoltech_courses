#!/usr/bin/bash

for x in 1 2 3 4
do
	cat runpi.sh | sed "/cpus/s/=.*$/=$x/" | sbatch
done
