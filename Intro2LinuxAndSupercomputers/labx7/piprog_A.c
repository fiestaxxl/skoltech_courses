/*	Author: Nikita Kuznetsov
 *	Date of creation: 29.11.2021, 12:30	version 0.0
 *	LabX-7
 *
 *	Summary: pi calculation using random numbers and areas
 *	
 *	To compile: gcc -o piprog_A -fopenmp -O3 piprog_A.c
 *	To run: ./piprog_A
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>		//sqrt
#include <omp.h>

#define NPOINTS		8000000000	// number trials
#define GENSEED		1235791		// prime random seed

int main()
{
	long ncirc = 0;
	double pi, dpi;
	int numthrd = omp_get_max_threads();
	unsigned long long num_trials = NPOINTS;

	double tstart = omp_get_wtime();

#pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc)
{
	double x, y, t, dres1, dres2;
	struct drand48_data rbuf;
	int mythrid = omp_get_thread_num();
	long rseed = (mythrid+1) * GENSEED;
	unsigned long long i;

	srand48_r(rseed, &rbuf);

	#pragma omp for reduction(+:ncirc)	// split the work
	for( i = 0;i < num_trials; i++) {	//among the team
		
		drand48_r(&rbuf, &dres1);	// re-entrant random num gen
		drand48_r(&rbuf, &dres2);	// [0..1)

		x = 2.0 * dres1 - 1;		// place the circle around 0
		y = 2.0 * dres2 - 1;
		t = x*x + y*y;

		if( t <= 1.0 ) ncirc++;
	}
}	//end parallel region

	double tend = omp_get_wtime();
	double tlaps = tend - tstart;

	pi = 4.0 * (double) ncirc/ (double) num_trials;
	dpi = pi*sqrt(2.0/num_trials);
	fprintf(stdout, "Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f PI: %.8lf dpi: %.1g\n", num_trials, ncirc, numthrd, tlaps, pi, dpi);

	return 0;
}
