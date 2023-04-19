//  CITS2002 Project 2 2022
//  Student1:   23792058   Ying   jiayang   100

#include "trove.h"

int main(int argc,  char *argv[])
{
    if(argc < 2)
    {
        usage();
        exit(EXIT_FAILURE);
    }

    char **cmlWords; // words or filelists from command line
    int opt; // the option

    // parsing arguments
    while((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        cmlWords = parsingArgument(argc, argv, opt, cmlWords);

    }

    // if there is no words or filelists
    if(cmlWordnum == 0)
    {
        usage();
    }

    // print information about command line argument
    printf("\toption = %d,\tfilename =%s,\tnum = %d\n",option, fileName, cmlWordnum);
    for(int i = 0 ; i < cmlWordnum; i++)
    {
        printf("\tword: %s\n", cmlWords[i]);
    }


    if(option == BUILD)
    {
        buildFile(cmlWords);
    }
    else if(option == SEARCH)
    {
        if(setLength || cmlWordnum != 1)
        {
            usage();
        }
        searchWord(cmlWords[0]);
    }
    else if(option == REMOVE)
    {
        removeFile(cmlWords, cmlWordnum);
    }
    else if (option == UPDATE)
    {
        updateFile(cmlWords, cmlWordnum);
    }

    exit(0);
}

