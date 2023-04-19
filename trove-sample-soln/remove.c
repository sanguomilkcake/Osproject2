#include "trove.h"
#include <dirent.h>

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

static void remove_filename_from_trove(char *filename)
{
    char    absolute[MAXPATHLEN];
    realpath(filename, absolute);

    int fileN   = find_filename(absolute);
    if(fileN < 0) {
        fprintf(stderr, "\"%s\" is not in trove-file\n", absolute);
        exit(EXIT_FAILURE);
    }
    else {
        if(debug) {
            printf("\tremoving \%s\" from trove-file", absolute);
        }
        remove_fileN_from_filenames(fileN);
        remove_fileN_from_words(fileN);
    }
}

//  ----------------------------------------------------------------------

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
                    remove_filename_from_trove(fullpath);
                }

//  ELSE, IS IT A DIRECTORY?  RECURSIVELY TRAVERSE DIRECTORY
                else if(S_ISDIR(statinfo.st_mode)) {
                    recursively_traverse_directory(fullpath);
                }
            }

//  ALL OTHER TYPES OF DIRECTORY ENTRIES ARE IGNORED
	}
	closedir(dirp);
    }
    else {
        fprintf(stderr, "cannot open directory %s\n", dirname);
    }
}

void remove_from_trove(char *filelist[])
{
    for(int a=0 ; filelist[a] != NULL ; ++a) {
        struct stat statinfo;

        if(stat(filelist[a], &statinfo) == 0) {
            if(S_ISREG(statinfo.st_mode)) {
                remove_filename_from_trove(filelist[a]);
            }
            else if(S_ISDIR(statinfo.st_mode)) {
                recursively_traverse_directory(filelist[a]);
            }
        }
        else {
            perror(filelist[a]);
            exit(EXIT_FAILURE);
        }
    }
}
