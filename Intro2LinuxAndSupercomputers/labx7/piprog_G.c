/*	Author: Nikita Kuznetsov
 *	Date of creation: 29.11.2021, 15:30	version 0.0
 *	LabX-7
 *
 *	Summary: pi calculation using random numbers and areas 
 *		 alternative way of parallization, not distributing the
 *		 workload in the for-loop
 *		 All threads do same amount of work and ccoperate only 
 *		 on building up the N_c
 *	
 *	To compile: gcc -o piprog_G -fopenmp -O3 piprog_G.c -lm
 *	To run: ./piprog_A -s GENSEED -t NPOINTS
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>	// sqrt
#include <omp.h>

#define NPOINTS		8000000000	// number trials
#define GENSEED		1235791		// prime random seed
#define OPTFLAGS	"st:"

extern int optind;
const char optflags[] = OPTFLAGS;

void usage(int argc, char *argv[])
{
	fprintf(stderr, "usage: %s -%c <long> -%c <ull>\n", argv[0], optflags[0], optflags[1]);
	exit(0);
}

int main(int argc, char **argv)
{	
	if(argc != 5) usage(argc, argv);	

	long ncirc = 0;
	double pi, dpi;
	int numthrd = omp_get_max_threads();
	unsigned long long num_trials = strtoll(argv[4], NULL, 10);
	const long gen_seed = strtol(argv[2], NULL, 10);
	double tstart = omp_get_wtime();
	
#pragma omp parallel default(none) firstprivate(num_trials) shared(ncirc)
{
	double x, y, t, dres1, dres2;
	struct drand48_data rbuf;
	int mythrid = omp_get_thread_num();
	long rseed = (mythrid+1) * GENSEED;
	unsigned long long local_ncirc = 0;
	unsigned long long i;

	srand48_r(rseed, &rbuf);
	
	for( i = 0; i < num_trials; i++ ) { // do not split work !
		
		drand48_r(&rbuf, &dres1);	// re-entrant random num gen
		drand48_r(&rbuf, &dres2);	// [0..1)

		x = 2.0 * dres1 -1;		// place the circle around 0
		y = 2.0 * dres2 - 1;
		t = x*x + y*y;
		
		if( t <= 1.0 ) local_ncirc++;
	}
	#pragma omp atomic
		ncirc += local_ncirc;
} // end parallel region
	
	double tend = omp_get_wtime();
	double tlaps = tend - tstart;
	
	num_trials *= numthrd;		// all threads same work
	pi = 4.0 * (double) ncirc/ (double) num_trials;
	dpi = pi*sqrt(2.0/num_trials);
	fprintf(stdout,"Trials: %ld Ncirc: %ld Threads: %d Elapsed: %.2f PI: %.8lf dpi: %.lg\n", num_trials, ncirc, numthrd, tlaps, pi, dpi);
	return 0;
}
