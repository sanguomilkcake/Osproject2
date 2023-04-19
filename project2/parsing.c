# include "trove.h"

void usage(void)
{
    printf("Usage: ./trove [-f filename] [-b | -r | -u] [-l length] filelist\n"
            "or     ./trove [-f filename] word\n");
    exit(EXIT_FAILURE);
}

static void checkIsValid(int opt)
{
    if(!(setOptionAlready))
    {
        if(opt == 'r')
            {option = REMOVE;}
        else if (opt == 'u')
            {option = UPDATE;}
        else if (opt == 'b')
            {option = BUILD;}

        setOptionAlready = true;
    }
    else
    {
        usage();
    }
}

char** parsingArgument(int argc, char **argv, int opt, char **cmlWords)
{
    // add the char pointer to the array of pointers
    if(opt == 1)
    {
        cmlWords = realloc(cmlWords, (cmlWordnum+1) * sizeof(cmlWords[0]));
            if(cmlWords == NULL)
            {
                fprintf(stderr, "%s can not request memory\n", argv[0]);
                exit(EXIT_FAILURE);
            }

        cmlWords[cmlWordnum] =  [optind-1];
        cmlWordnum++;
    }

    // get command line option
    else if(opt == 'b'|| opt == 'r' || opt == 'u')
    {
        checkIsValid(opt);
    }
    else if(opt == 'f')
    {
        fileName = strdup(optarg);

    }
    else if(opt == 'l')
    {
        int i = 0;
        while(optarg[i] != '\0')
        {   if(!isdigit(optarg[i]))
            {
                fprintf(stderr, "Please input correct word length\n");
                usage();
            }
            i++;
        }
        wordLength = atoi(optarg);
        if(wordLength  <= 0)
        {
            fprintf(stderr, "Word length should be positive\n");
            usage();
        }
        setLength = true;
    }
    else
    {
        usage();
    }

    return cmlWords; // return the array of char pointer(word or file list).

}



