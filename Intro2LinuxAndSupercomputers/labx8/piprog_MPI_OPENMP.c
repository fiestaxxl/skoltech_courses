/*	Author: Nikita Kuznetsov
 *	Date of creation: 04.12.2021, 0:50	version 0.0
 *	LabX-8
 *
 *	Summary: pi calculation using random numbers and areas 
 *		 MPI + OpenMP implementation
 *	
 *	To compile: mpicc -o pim1 -O3 -fopenmp piprog_MPI_OPENMP.c -lm 
 *	To run:
 *
 *      NPOINTS		TIME (1 thread)
 *	800000000	~14 sec	
 *	8000000000	~144 sec
 *	2000000000	~36 sec
 *	4000000000	~80 sec
 *      3500000000	~70 sec
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>	// sqrt
#include <mpi.h>
#include <omp.h>
#include <time.h>

#define NPOINTS		3500000000	// number trials
#define GENSEED		1235791		// prime random seed

// here time calculation finction added in order to compute time in different way
#define NANOSEC_in_SEC 1000000000LL
#define dsecond(tx,ty) ((double)((ty.tv_sec)-(tx.tv_sec))+(double)((ty.tv_nsec)-(tx.tv_nsec))/NANOSEC_in_SEC)

int main(int argc, char **argv)
{	
	long ncirc = 0;
	double pi, dpi;
	int numthrd;
	int mythrid;
	unsigned long long num_trials = NPOINTS;
	unsigned long long local_ncirc = 0;
	int rank, size;
	struct timespec tstart, tend;

	MPI_Init(&argc, &argv);
	MPI_Comm_size( MPI_COMM_WORLD, &numthrd);
	MPI_Comm_rank( MPI_COMM_WORLD, &mythrid);

	clock_gettime(CLOCK_REALTIME, &tstart);

#pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc, local_ncirc, mythrid)
{

	double x, y, t, dres1, dres2;
	struct drand48_data rbuf;
	int mythrid_omp = omp_get_thread_num();
	long rseed = (mythrid_omp+mythrid) * GENSEED;
	unsigned long long i;

	srand48_r(rseed, &rbuf);

	for( i = 0; i < num_trials; i++ ) { 	// do not split work !
		
		drand48_r(&rbuf, &dres1);	// re-entrant random num gen
		drand48_r(&rbuf, &dres2);	// [0..1)

		x = 2.0 * dres1 -1;		// place the circle around 0
		y = 2.0 * dres2 - 1;
		t = x*x + y*y;
		
		if( t <= 1.0 ) local_ncirc++;
	}	
	#pragma omp atomic
		ncirc += local_ncirc;
}	// end of OpenMP parallel

	MPI_Reduce(&local_ncirc, &ncirc, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	clock_gettime(CLOCK_REALTIME, &tend);
	double tlaps = dsecond(tstart, tend);
	
	num_trials *= numthrd;		// all threads same work
	pi = 4.0 * (double) ncirc/ (double) num_trials;
	dpi = pi*sqrt(2.0/num_trials);
	
	if(mythrid == 0)
	fprintf(stdout,"Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f PI: %.8lf dpi: %.lg\n", num_trials, ncirc, numthrd, tlaps, pi, dpi);
	
	MPI_Finalize(); // MPI finalization added
	return 0;
}
