#include "trove.h"

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

void build_trove(char *filelist[])
{
    if(debug) {
        printf("\tbuilding trove-file \"%s\"\n", trovefile);
    }
    for(int a=0 ; filelist[a] != NULL ; ++a) {
        findfiles(filelist[a]);
    }
}
