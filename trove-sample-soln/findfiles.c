#include "trove.h"
#include <dirent.h>

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

static void recursively_traverse_directory(char *dirname)
{
    if(debug) {
        printf("\treading directory \"%s\"\n", dirname);
    }

//  ENSURE THAT WE CAN OPEN/READ THE DIRECTORY
    DIR *dirp	= opendir(dirname);

    if(dirp) {
        struct dirent	*dp;

//  READ EACH DIRECTORY ENTRY
	while((dp = readdir(dirp)) != NULL) {

//  ALWAYS IGNORE THIS DIRECTORY (dot) AND ITS PARENT (dotdot)
	    if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
		continue;
            }

//  RECONSTRUCT THE PATHNAME <- dir+entry 
            char        fullpath[MAXPATHLEN];
	    sprintf(fullpath, "%s/%s", dirname, dp->d_name);

//  ENSURE THAT WE CAN GET THE ATTRIBUTES OF THIS PATHNAME
            struct stat statinfo;

	    if(stat(fullpath, &statinfo) == 0) {

//  IS THIS A REGULAR FILE?
                if(S_ISREG(statinfo.st_mode)) {
                    find_words(fullpath, statinfo.st_size);
                }

//  ELSE, IS IT A DIRECTORY?  RECURSIVELY TRAVERSE DIRECTORY
                else if(S_ISDIR(statinfo.st_mode)) {
                    recursively_traverse_directory(fullpath);
                }

//  SILENTLY IGNORE ALL OTHER TYPES OF DIRECTORY ENTRY
            }
	}
	closedir(dirp);
    }
    else {
        fprintf(stderr, "cannot open directory %s\n", dirname);
    }
}

void findfiles(char *something)
{
    struct stat statinfo;

    if(stat(something, &statinfo) == 0) {
        if(S_ISREG(statinfo.st_mode)) {
            find_words(something, statinfo.st_size);
        }
        else if(S_ISDIR(statinfo.st_mode)) {
            recursively_traverse_directory(something);
        }
    }
    else {
        perror(something);
        exit(EXIT_FAILURE);
    }
}
