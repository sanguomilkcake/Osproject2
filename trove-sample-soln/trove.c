#include "trove.h"
#include <getopt.h>

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

#define	OPTLIST		"bf:l:ru"

//  REPORT ERRORS WITH COMMAND-LINE OPTIONS TO stderr
static void usage(char *progname)
{
    fprintf(stderr, "Usage: %s [-f filename] [-b | -r | -u] [-l length] filelist\n", progname);
    fprintf(stderr, "or     %s [-f filename] word\n", progname);

    fprintf(stderr, "\nwhere options are:\n");

    fprintf(stderr,
    "  -b\t\tbuild a new trove-file\n");
    fprintf(stderr,
    "  -f filename\tprovide the name of the trove-file to be built or searched\n");
    fprintf(stderr,
    "  -l length\tspecify the minimum-length of words added to the trove-file\n");
    fprintf(stderr,
    "  -r\t\tremove information from the trove-file\n");
    fprintf(stderr,
    "  -u\t\tupdate information in the trove-file\n");

    fprintf(stderr, "\nPlease report any bugs to %s\n", AUTHOR_EMAIL);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    char *progname  = (progname = strrchr(argv[0],'/')) ? progname+1 : argv[0];
    debug           = (getenv("DEBUG") != NULL);    // not required for project

//  NOT REQUIRED FOR PROJECT; JUST USED TO SUPPORT THE WEB-BASED DISPLAY
    if(getenv("DUMP")) {
        trovefile   = strdup( getenv("DUMP") );
        exit( dump_trovefile() );
    }

    bool bflag	= false;        // build the trove-file
    int  lflag  = -1;           // minimum length words to be indexed
    bool rflag	= false;        // remove information from the trove-file
    bool uflag	= false;        // update from the trove-file

    int		opt;

    opterr	= 0;
    while((opt = getopt(argc, argv, OPTLIST)) != -1) {
	switch (opt) {

//  BUILD TROVE-FILE
	case 'b' :  bflag	= !bflag;
		    break;

//  REMOVE INFORMATION FROM TROVE-FILE
	case 'r' :  rflag	= !rflag;
		    break;

//  UPDATE INFORMATION WIN
	case 'u' :  uflag	= !uflag;
		    break;

//  PROVIDE NAME OF TROVE-FILE
        case 'f' :  trovefile   = strdup(optarg);
                    CHECK_ALLOC(trovefile);
                    break;

//  PROVIDE MINIMUM LENGTH OF WORDS
	case 'l' :  lflag	= atoi(optarg);
                    if(lflag < 1) {
                        fprintf(stderr,"%s : invalid wordlength %i\n",
                            progname, lflag);
                        argc = -1;
                    }
		    break;

//  IF NONE OF THE ABOVE, VALID OPTIONS
	default :   fprintf(stderr,"%s : illegal option -%c\n",
                            progname, optopt);
		    argc = -1;
		    break;
	}
    }
    argv	+= optind;
    argc	-= optind;

//  ENSURE THAT THE COMBINATION OF OPTIONS IS CORRECT
    int sum_bru = bflag+rflag+uflag;
    if(argc < 1 || sum_bru > 1 || (sum_bru == 0 && lflag > 0)) {
	usage(progname);
    }

    if(lflag > 0) {
        minlength   = lflag;
    }

    int status  = EXIT_SUCCESS;

//  BUILD A NEW TROVE-FILE WITH WORDS FOUND VIA filelist
    if(bflag) {
        build_trove(argv);
        write_trovefile();
    }
//  REMOVE ALL REFERENCES TO FILES FOUND VIA THE filelist
    else if(rflag) {
        read_trovefile();
            remove_from_trove(argv);
        write_trovefile();
    }
//  UPDATE AN EXISTING TROVE-FILE WITH WORDS IN FILES FOUND VIA THE filelist
    else if(uflag) {
        read_trovefile();
            remove_from_trove(argv);
            update_trove(argv);
        write_trovefile();
    }
//  OVERWISE, SEARCH EXISTING TROVE-FILE FOR A REQUIRED WORD
    else {
        read_trovefile();
        status  = (search_words(argv[0]) > 0) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if(debug && status == 0) {
        printf("\t%i unique file%s\n", nfilenames, PLURAL(nfilenames));
        printf("\t%i unique word%s\n", nwords, PLURAL(nwords));
    }

    exit(status);
    return status;
}
