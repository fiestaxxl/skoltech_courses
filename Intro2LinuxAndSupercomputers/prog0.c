/*
 *      Author: Nikita Kuznetsov
 *      Date of creation: 16.11.2021 13:00, verison 0.0
 *
 *      Summary:
 *      Demonstrate reopen of std I/O streams
 *      Program will copy file in first argument to second argument, or std streams
 *
 *      Examples:
 *      prog -s <long> in out
 *      prog -s <long> <in >out
 *      prog -s <long> in       should work to dump file in to terminal
 *      prog -s <long> in 2>log should work to dump file in to terminal and error stream to log
 *      some messages are printed to stderr and can be redirected to stdout with &>
 *      (note the arbitrary order of lines from stderr in redirected output)
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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
        FILE *f1 = stdin;
        FILE *f2 = stdout;
        char buf[SIZEBUF];      // use preprocessor defined constants
        long parsize = getparams(argc, argv);
        int i;
        int c_upper = 0; // count for upper letter
        int c_blank = 0; // count for black symbols

        fprintf(stderr, "option arguments: %ld\n",parsize);

        int k = optind;
        if (k < argc) {
                fprintf(stderr, "open stdin: %d %s\n", k, argv[k]);
                f1 = freopen(argv[k], "r", stdin); printf("\n"); // open for read,position stream at beggining
                if ( f1 == NULL ) perror("open file on stdin");
        }
        k++;
        if(k < argc) {
                fprintf(stderr, "open stdout: %d %s\n",k,argv[k]);
                f2 = freopen(argv[k], "w", stdout); //printf("\n"); // w-Open for write and truncate to zero len,
                if ( f2 == NULL ) perror("open file on stdout");  // create if it does not exits
        }

        while ( fgets(buf, SIZEBUF, f1) ) {     // get a string from stdin, NULL is returned when EOF or error
                buf[strlen(buf)-1] = '\0';      // remove the trailing return-character '\n'

                for (i=0; i < strlen(buf); i++) {
                        if ( isupper( buf[i] ) ) c_upper++; // if letter is upper increase
                        if ( isblank( buf[i] ) ) c_blank++; // if symbols is blank increase
                }

                // substract and compare number of symbols
                if ( c_upper  == ( strlen(buf)-1-c_blank ) && ( c_blank != strlen(buf)-1) ) {
                        puts(buf); // to use puts in such a way we imported <stdio.h> and declared f2 = stdout
                }

                c_upper = 0; // reset counters
                c_blank = 0;
        }
        fprintf(stderr, "all done\n");  // message to the terminal
}