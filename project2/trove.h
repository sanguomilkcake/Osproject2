#define _POSIX_C_SOURCE     200809L
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/param.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include  <sys/stat.h>
#include <ctype.h>
#define OPTLIST "-bf:l:ru"

/* store file information */
typedef struct {
    char* filePath; /*  file realpath */
    char** fileContent; /*  words in the file   */
    int wordNum;    /* the number of word   */
} MYFILE, *PMYFILE;

/* Enum type for option */
enum options{BUILD, SEARCH, UPDATE, REMOVE};


/* DECLARE GLOBAL FUNCTIONS */

/*************************************************
Function: usage
Description: if command line argument is illegal, print usage information and exit
Input: void
Return: void
 *************************************************/
extern void usage(void);

/*************************************************
Function: parsingArgument
Description: parse command line argument
Input:
    int                                     argc            the number of command line argument
    a pointer point to a char pointer       argv            command line value
    Int                                     opt             the option from command line argument
    a pointer point to a char pointer       cmlWords        a word or files from command line
Return: a pointer point to a char pointer
Called By: main()
 *************************************************/
extern char**  parsingArgument(int argc, char **argv, int opt, char **cmlWords);

/*************************************************
Function: buildFile
Description: build file
Input:
    a pointer point to a char pointer       filelist         files from command line
Return: void
Calls: dealOneFile()    traverseFileRecursive()     copressFile()
Called By: main()
 *************************************************/
extern void buildFile(char** filelist);

/*************************************************
Function: traverseFileRecursive
Description: traverse file and save real path of file
Input:
    a pointer to char        basePath       the path of a file or directory
Return: void
Called By: buildFile()    updateFile()
 *************************************************/
extern void  traverseFileRecursive(char *basePath);

/*************************************************
Function: dealOneFile
Description: read and write content of one file
Input:
    a pointer to char        realPath       the path of a file
    int                   fdw               file descriptor
Return: void
Called By:  buildFile()     updateFile()
 *************************************************/
extern void dealOneFile(char *realPath, int fdw);

/*************************************************
Function: parsingFile
Description: parsing the file information
Input:
    int        fileNum     file numbers
Return: PMYFILE
Calls:  decompossFile()
Called By:  searchWord()    removeFile()    updateFile()
 *************************************************/
extern PMYFILE parsingFile(int *fileNum);

/*************************************************
Function: searchWord
Description: search files that contain target word
Input:
    a pointer to char        targetWord     the target word
Return: void
Calls:  parsingFile()
Called By:  main()
 *************************************************/
extern void searchWord(char *targetWord);

/*************************************************
Function: removeFile
Description: remove file
Input:
    a pointer point to a char pointer        fileList     files need to be removed
    int                                      filenum      file number
Return: void
Calls:  parsingFile()     copressFile()
Called By:  main()        updateFile()
 *************************************************/
extern void removeFile(char ** fileList, int filenum);

/*************************************************
Function: updateFile
Description: update file
Input:
    a pointer point to a char pointer        fileList     files need to be updated
    int                                      filenum      file number
Return: void
Calls:  parsingFile()   removeFile()    dealOneFile()   copressFile()
Called By:  main()
 *************************************************/
extern void updateFile(char ** filelist, int filenum);

/*************************************************
Function: reWrite
Description: rewrite file
Input:
    MYFILE        file     the file need to be written
    int           fdw      file descriptor
Return: void
Called By:  updateFile()    removeFile()
 *************************************************/
extern void reWrite(MYFILE file, int fdw);

/*************************************************
Function: wait_for_one
Description: wait for one child process
Input: void
Return: void
Called By:  copressFile()       decompossFile()
 *************************************************/
extern void wait_for_one(void);

/*************************************************
Function: copressFile
Description: compress file
Input: point to char    name    file path
Return: void
Called By:  parsingFile()
 *************************************************/
extern void copressFile(char* name);

/*************************************************
Function: decompossFile
Description: decompress file
Input:
point to char    name       file path
array of int     thepipe    pipe for communication between processes
Return: void
Called By:  buildFile()     updateFile()    removeFile()
 *************************************************/
extern void decompossFile(char * name, int thepipe[2]);

// DECLARE GLOBAL VARIABLES
extern enum options option; /*  option from command line    */
extern char *fileName;      /*  file name to write content to    */
extern char ** realPathList;    /*  store all real paths of files from file list */
extern int filenum; /*  file numbers */
extern int wordLength;  /*  target word length */
extern int cmlWordnum;  /* file or dictionary number in the file list or only 1 if it is a word */
extern bool setOptionAlready;   /*  a flag for checking if the command line argument is illegal */
extern bool setLength;          /*  a flag for checking if the command line argument is illegal */
extern int thepipe[2];          /* the pipe for communication between processes */
