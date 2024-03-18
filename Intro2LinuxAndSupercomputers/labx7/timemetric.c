#include <stdio.h>
#include <time.h>

#define CLOCK_MY CLOCK_REALTIME

#define NANOSEC_in_SEC 1000000000LL
#define MICROSEC_in_SEC 1000000.0
#define NANOSEC_in_MICROSEC 1000.0

#define dsec(tx,ty) ((double)((ty.tv_sec)-(tx.tv_sec))*1+(double)((ty.tv_nsec)-(tx.tv_nsec))/NANOSEC_in_SEC)

#define dmicrosec(tx,ty) ((double) (ty.tv_sec - tx.tv_sec) * MICROSEC_in_SEC + (double)(ty.tv_nsec-tx.tv_nsec)/NANOSEC_in_MICROSEC)

#define llnanosec(tx,ty) ((long long unsigned) (ty.tv_sec - tx.tv_sec) * NANOSEC_in_SEC + (long long unsigned)(ty.tv_nsec - tx.tv_nsec))

#include "rdtsc.c"

int main()
{
        struct timespec t1, t2;
        struct timespec twait, trest;
    long long unsigned diffns = 0;
    long long unsigned rdt1, rdt2;
        int i=0;

#ifdef RDTSC
    rdt1 = rdtsc();
    rdt2 = rdt1;
        while( rdt2 <= rdt1) { rdt2 = rdtsc(); i++; }
    diffns = rdt2-rdt1;
        printf("cycles rdtsc resolution: %lld ns  Overhead: %f ns loopcount: %d\n", diffns, (double)diffns/i, i);
#endif

    i=0;
        clock_gettime(CLOCK_MY, &t1);
    t2.tv_nsec = t1.tv_nsec;
        while( t2.tv_nsec <= t1.tv_nsec) { clock_gettime(CLOCK_MY, &t2); i++; }
    diffns = llnanosec(t1,t2);
        printf("clock resolution: %lld ns  Overhead: %f ns loopcount: %d\n", diffns, (double)diffns/i, i);

    twait.tv_sec = 10;
    twait.tv_nsec =1000;
        clock_gettime(CLOCK_MY, &t1);
        /* start timing region */


            nanosleep(&twait, &trest);

        /* end timing region */
        clock_gettime(CLOCK_MY, &t2);

        diffns = dsec(t1,t2);
        printf("elapsed time: %lld s\n", diffns);
}
