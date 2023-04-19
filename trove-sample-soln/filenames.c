#include "trove.h"

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

char    **filenames      = NULL;
int     nfilenames       = 0;

#define FOREACH_FILENAME    for(int f=0 ; f<nfilenames ; ++f)

//  POSSIBLY ADD A NEW ABSOLUTE PATHNAME TO OUR GROWING VECTOR OF PATHNAMES
int add_filename(char *absolute)
{
    FOREACH_FILENAME {
        if(strcmp(filenames[f], absolute) == 0) {       // already known?
            return f;
        }
    }
    filenames            = realloc(filenames, (nfilenames+1)*sizeof(*filenames));
    CHECK_ALLOC(filenames);
    filenames[nfilenames] = strdup(absolute);
    CHECK_ALLOC(filenames[nfilenames]);
    return nfilenames++ ;               // return index of new pathname, fileN
}

void remove_fileN_from_filenames(int fileN)
{
    if(fileN < nfilenames) {
        free(filenames[fileN]);
        filenames[fileN]    = filenames[nfilenames-1];
        --nfilenames;
    }
}

int find_filename(char *absolute)
{
    FOREACH_FILENAME {
        if(strcmp(filenames[f], absolute) == 0) {       // found
            return f;
        }
    }
    return -1;                                          // not found
}

int write_filenames_to_trovefile(int fd)
{
    FOREACH_FILENAME {
        write(fd, filenames[f], strlen(filenames[f]) );
        write(fd, "\n", 1);
    }
//  WE DO NOT CLOSE fd
    return 0;
}
