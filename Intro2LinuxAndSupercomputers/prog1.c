/*      Author: Nikita Kuznetsov
 *      Date of creation: 16.11.2021, 13:00     verison 0.0
 *      Exercise 2
 *
 *      Summary:
 *      Allocates file in memory with given size in Bytes
 *
 *      Examples:
 *      prog -s <long> in out
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*      program definitions
 *      put here all explicit numbers and definitions
 */
#define SIZEBUF 128     // this is the size of the input buffer
#define OPTFLAGS "s:"   // option flags as compiler symbolic string

/*      global variables
 */
extern int optind;
const char optflags[] = OPTFLAGS;       // option flags

void usage(int argc, char *argv[])
{
        fprintf(stderr, "usage: %s [-%c <int>] in out\n", argv[0], optflags[0]);
        exit(0);
}

long getparams(int argc, char *argv[])
{
        const char *flags = optflags;
        char *param = NULL;
        int opt;
        int flag = 0;
        extern char * optarg;
        long parsize = 0;

        while ( (opt = getopt(argc, argv, flags)) != -1 ) {
                if ( opt == optflags[0] ) {     // Note: swith needs const in case
                                param = optarg;
                                parsize = atoll(param);
                                // break;       should NOT break the while loop to collect all flags/parameters
                }
                else usage(argc, argv);
        }
        return parsize;
}

int main(int argc, char *argv[])
{
        int f1 = fileno(stdin); // file descriptor for the standard streams
        int f2 = fileno(stdout);
        int readflg = O_RDONLY;
        int writflg = O_CREAT | O_WRONLY | O_TRUNC;
        off_t fpos;
        ssize_t nread, nwrit;
        char buf[SIZEBUF];      // use preprocessor defined constants
        long parsize = getparams(argc, argv);
        struct stat finfo;

        fprintf(stderr, "option arguments: %ld\n",parsize);

        int k = optind;
        if (k < argc) {
                fprintf(stderr, "open stdin: %d %s\n", k, argv[k]);
                f1 = open(argv[k], readflg); // open for read,position stream at beggining
                if ( f1 < 0 ) perror("open input file");
        }
        k++;
        if(k < argc) {
                fprintf(stderr, "open stdout: %d %s\n",k,argv[k]);
                f2 = open(argv[k],writflg); //printf("\n"); // w-Open for write and truncate to zero len,
                if ( f2 < 0 ) perror("open output file");  // create if it does not exits
        }
        if ( (fpos = lseek(f2, (off_t)0, SEEK_END)) < 0 ) perror("lseek");
        else fprintf(stderr, "write position in file %s is: %ld Bytes\n",argv[k], fpos);

        if ( (fpos = lseek(f2, parsize, SEEK_END)) < 0 ) perror("lseek");
        else fprintf(stderr, "moving beyound EOF by %ld Bytes, current position %ld Bytes\n",parsize, fpos);

        while ( (nread = read(f1, buf, SIZEBUF)) ) { // get a string from stdin, NULL when EOF or error
                nwrit = 0;
                do {
                        nread -= nwrit;
                        nwrit = write(f2, buf+nwrit,nread); // if write is interrupted,it may return less bytes
                } while ( nwrit < nread ); // in that case the call is restarted untill all bytes are written
        }

        if ( fstat(f2, & finfo) < 0 ) perror("fstat"); //fileno extract file descriptior from FILE *
        else {
                fprintf(stderr, "file size: %ld\n",finfo.st_size);
                fprintf(stderr, "blocks allocated: %ld size on disk: %ld Bytes\n",
                                finfo.st_blocks, finfo.st_blocks * 512);
        }
        fprintf(stderr, "all done\n");  // message to the terminal
}