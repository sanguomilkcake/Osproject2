#include "trove.h"

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

void update_trove(char *filelist[])
{
    for(int a=0 ; filelist[a] != NULL ; ++a) {
        if(debug) {
            printf("\tupdating \"%s\" in trove-file", filelist[a]);
        }

//  PERFORM THE EQUIVALENT OF BUILDING/ADDING TO THE TROVE-FILE
        findfiles(filelist[a]);
    }
}
