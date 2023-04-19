#define _POSIX_C_SOURCE     200809L             // enables strdup()
#define _DEFAULT_SOURCE                         // enables realpath()
#define _BSD_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/param.h>                          // defines MAXPATHLEN
#include <sys/types.h>
#include <sys/stat.h>

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

#define	AUTHOR_EMAIL        "Chris.McDonald@uwa.edu.au"

#define DEF_TROVEFILE       "/tmp/trove"
#define DEF_MINLENGTH       4

#define BIGGEST_WORD        2000                // what should this size be?

//  RESEARCH SHOWS THAT USING PRIME-NUMBERS CAN IMPROVE PERFORMANCE
//  c.f.  https://www.quora.com/Why-should-the-size-of-a-hash-table-be-a-prime-number
#define	HASHTABLE_SIZE      997

//  A HELPFUL PREPROCESSOR MACRO TO CHECK IF ALLOCATIONS WERE SUCCESSFUL
#define CHECK_ALLOC(p)  if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }
#define PLURAL(n)       ((n)==1 ? "" : "s")

//  --------------------------------------------------------------------

//  DEFINED IN build.c
extern  void    build_trove(char *filelist[]);

//  DEFINED IN compression.c
extern  int       compress_file(char *filename);
extern  int     decompress_file(char *filename, bool announce);

//  DEFINED IN filenames.c
extern  int     add_filename(char *absolute);
extern  void    remove_fileN_from_filenames(int fileN);
extern  int     find_filename(char *absolute);
extern  int     write_filenames_to_trovefile(int fd);
extern  int     read_filenames_from_trovefile(int fd);
extern  char    **filenames;
extern  int     nfilenames;

//  DEFINED IN findfiles.c
extern  void    findfiles(char *something);

//  DEFINED IN findwords.c
extern  void	find_words(char *filename, off_t nbytes);

//  DEFINED IN globals.c
extern  bool    debug;
extern  int     minlength;                  // set with -l option
extern  char    *trovefile;                 // set with -f option

//  DEFINED IN remove.c
extern  void    remove_from_trove(char *filelist[]);

//  DEFINED IN trovefile.c
extern  int     write_trovefile(void);
extern  int     read_trovefile(void);
extern  int     dump_trovefile(void);

//  DEFINED IN update.c
extern  void    update_trove(char *filelist[]);

//  DEFINED IN words.c
extern  int     nwords;
extern  void    add_word(char *word, int fileN);
extern  int     remove_fileN_from_words(int fileN);
extern  int     write_words_to_trovefile(int fd);
extern  int     read_words_from_trovefile(int fd);
extern  int     search_words(char *word);
