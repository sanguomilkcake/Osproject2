#include "trove.h"

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

#define PATH_GZIP               "/usr/bin/gzip"
#define PATH_ZCAT               "/usr/bin/zcat"

/*  SOME (OLDER) SYSTEMS MAY STILL USE THESE PATHS:

#define PATH_GZIP               "/bin/gzip"
#define PATH_ZCAT               "/bin/zcat"
 */

//  EXPRESSED IN THE SYNTAX OF THE SHELL, WE ARE PEFORMING:
//      trove-program | gzip > trove-file
//
//  THIS PROGRAM IS THE PARENT PROCESS, gzip RUNS IN A NEW CHILD PROCESS
//
int compress_file(char *filename)
{
    if(debug) {
        printf("compressing trove-file\n");
    }
    fflush(stdout);

    int  thepipe[2];
    if(pipe(thepipe) != 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    switch ( fork() ) {
    case -1 :
        perror("fork");         // process creation failed
        exit(EXIT_FAILURE);
        break;

//  CHILD PROCESS RUNS gzip, READING FROM THE PIPE, WRITING TO trovefile
    case 0  :                   // new child process
        close( thepipe[1] );    // child will never write to pipe
        dup2(  thepipe[0], 0);  // duplicate the reading end's descriptor and stdin 
        close( thepipe[0] );    // close the reading end's descriptor

//  WE CAN REMOVE ANY OLD TROVE-FILE WITH unlink() OR A FLAG TO open()
//      unlink(filename);

        int fd  = open(filename, O_RDWR|O_CREAT|O_TRUNC, 0600);
        if(fd >= 0) {
            dup2(fd, 1);        // duplicate the writing end's descriptor and stdout
            close(fd);

            execl(PATH_GZIP, "gzip", NULL);
            perror(PATH_GZIP);
            exit(EXIT_FAILURE);
        }
        perror(filename);
        exit(EXIT_FAILURE);
        break;

    default :                   // parent process
        close( thepipe[0] );    // parent will never read from pipe
        break;
    }
    return thepipe[1];          // return the writing end
}


//  EXPRESSED IN THE SYNTAX OF THE SHELL, WE ARE PEFORMING:
//      zcat < trove-file | trove-program
//
//  THIS PROGRAM IS THE PARENT PROCESS, zcat RUNS IN A NEW CHILD PROCESS
//
int decompress_file(char *filename, bool announce)
{
    if(announce) {
        if(debug) {
            printf("decompressing trove-file\n");
        }
        fflush(stdout);
    }

    int  thepipe[2];
    if(pipe(thepipe) != 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    switch ( fork() ) {
    case -1 :
        perror("fork");         // process creation failed
        exit(EXIT_FAILURE);
        break;

//  CHILD PROCESS RUNS zcat, READING FROM trovefile, WRITING TO THE PIPE
    case 0  :                   // new child process
        close( thepipe[0] );    // child will never read from pipe
        dup2(  thepipe[1], 1);  // duplicate the writing end's descriptor and stdout 
        close( thepipe[1] );    // close the writing end's descriptor

        int fd = open(filename, O_RDONLY, 0);
        if(fd >= 0) {
            dup2(fd, 0);
            close(fd);

            execl(PATH_ZCAT, "zcat", NULL);
            perror(PATH_GZIP);
            exit(EXIT_FAILURE);
        }
        perror(filename);
        exit(EXIT_FAILURE);
        break;

    default :                   // parent process
        close( thepipe[1] );    // parent will never write to pipe
        break;
    }
    return thepipe[0];          // return the reading end
}
