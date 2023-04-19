#include "trove.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/


//  THE TROVE-FILE IS A TEXT-FILE; THE FIRST CHARACTER OF EACH LINE PROVIDES
//  AN ABSOLUTE PATHNAME, A WORD, OR FILE-NUMBER(s) CONTAINING THE WORD
//
//      /absolute/pathname/file1
//      /absolute/pathname/file2
//      elephant
//      +1
//      +2
//      budgie
//      +2
//
//  THE TROVE-FILE WILL BE COMPRESSED AND DECOMPRESSED WITH gzip AND zcat


//  WAIT FOR gzip OR zcat PROCESSES TO TERMINATE
static int wait_for_child_process(void)
{
    int status  = 0;

    while( wait(&status) > 0) {
        ;
    }
    return status;
}

//  REMOVE ANY TRAILING end-of-line CHARACTERS FROM THE LINE (from Lecture-8)
static void trim_line(char line[])
{
//  LOOP UNTIL WE REACH THE END OF line
    int i = 0;
    while(line[i] != '\0') {
        if( line[i] == '\r' || line[i] == '\n' ) { //  carriage-return or newline?
            line[i] = '\0'; // overwrite with null-byte
            break;          // leave the loop early
        }
        i = i+1;            // iterate through character array
    }
}

//  ---------------------------------------------------------------------

int write_trovefile(void)
{
    if(debug) {
        printf("writing trove-file\n");
    }

//  INVOKE THE gzip PROGRAM, RETURN THE WRITING END OF THE PIPE
    int fd  = compress_file(trovefile);
    if(fd >= 0) {
        write_filenames_to_trovefile(fd);
        write_words_to_trovefile(fd);
        close(fd);
        wait_for_child_process();
        return 0;
    }
    perror(trovefile);
    exit(EXIT_FAILURE);
    return 1;
}

int read_trovefile(void)
{
    if(debug) {
        printf("reading trove-file\n");
    }

    filenames   = NULL;
    nfilenames  = 0;

//  INVOKE THE zcat PROGRAM, RETURN THE READING END OF A PIPE
    int fd  = decompress_file(trovefile, true);
    if(fd >= 0) {
        FILE *fp = fdopen(fd, "r");

        char line[BIGGEST_WORD];
        char *word  = NULL;

//  READ EACH LINE OF THE TEXT TROVE-FILE
        while(fgets(line, sizeof(line), fp) != NULL) {
            trim_line(line);

//  FOUND AN ABSOLUTE PATHNAME
            if(line[0] == '/') {
                add_filename(line);
            }
//  FOUND A WORD
            else if(isalnum(line[0])) {
                if(word != NULL) {
                    free(word);
                }
                word    = strdup(line);
            }
//  FOUND A FILE-NUMBER CONTAINING THE MOST RECENT WORD
            else if(line[0] == '+' && isdigit(line[1])) {
                int fileN   = atoi(&line[1]);
                add_word(word, fileN);
            }
        }
        if(word != NULL) {
            free(word);
        }
        close(fd);
        wait_for_child_process();
        return 0;
    }
    perror(trovefile);
    exit(EXIT_FAILURE);
    return 1;
}

//  THIS FUNCTION IS ONLY REQUIRED TO SUPPORT THE WEB-BASED DISPLAY
int dump_trovefile(void)
{
    fflush(stdout);

//  INVOKE THE zcat PROGRAM, RETURN THE READING END OF THE PIPE
    int fd  = decompress_file(trovefile, false);
    if(fd >= 0) {
        FILE *fp = fdopen(fd, "r");

        char line[BIGGEST_WORD];
        int fn=0, wn=0;

        while(fgets(line, sizeof(line), fp) != NULL) {
            trim_line(line);
//  AN ABSOLUTE PATHNAME
            if(line[0] == '/') {
                printf("filename%i - %s\n", ++fn, line);
            }
//  A WORD FOUND IN ONE-OR-MORE OF THE PATHNAMES
            else if(isalnum(line[0])) {
                printf("word%i - %s\n", ++wn, line);
            }
        }
        close(fd);
        wait_for_child_process();
        return 0;
    }
    perror(trovefile);
    exit(EXIT_FAILURE);
    return 1;
}
