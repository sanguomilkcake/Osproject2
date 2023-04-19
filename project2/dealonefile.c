# include "trove.h"

char** writeWords(int fd,int fdw, int *wordNum,char* word,char **wordList)
{
    if(wordList == NULL) // if NULL in list then put the first char
    {
        if ((wordList  = realloc(wordList, (*wordNum+1)* sizeof(wordList[0]))) == NULL)
        {
            fprintf(stderr, "cannot allocate\n");
            close(fd); close(fdw);
            exit(1);
        }

        printf("%s  ", word);
        wordList[*wordNum] = word;
        *wordNum += 1;
    }

    else // else check duplicated words
    {
        bool isDuplicated = false;
        for(int i = 0; i < *wordNum; i++)
        {
            if(!strcmp(word, wordList[i]))
            {
                isDuplicated = true;
                break;
            }
        }

        // if not duplicated word, put in
        if(!isDuplicated)
        {
            wordList  = realloc(wordList, (*wordNum+1)* sizeof(wordList[0]));
            if(wordList == NULL)
            {
                fprintf(stderr,"cannot allocate\n");
                close(fd); close(fdw);
                exit(1);
            }

            printf("%s  ", word);
            wordList[*wordNum] = word;
            *wordNum += 1;
        }
    }
    return wordList;
}


void dealOneFile(char *realPath, int fdw)
{
    // open file for read;
    int fd;
    if((fd = open(realPath ,O_RDONLY)) < 0)
    {
        fprintf(stderr,"cannot open for read '%s'\n", realPath);
        exit(EXIT_FAILURE);
    }

    size_t got;
    char **wordList = NULL; // a list storing word of one file
    char c[1];
    char* word = NULL;
    int charNum = 0; // length of a word
    int wordNum = 0; // the number of words in a file

    // redirect the output stream to fdw
    int savfd = dup(STDOUT_FILENO);
    dup2(fdw, STDOUT_FILENO);
    printf("%s\n",realPath);

    while((got = read(fd, c, sizeof c[0])) > 0) // every loop get a char
    {

        // if it is a alphanumeric characters, then add char to to current word
        if(isalnum(c[0]))
        {

            if((word = (realloc(word, (charNum+1)* sizeof(word[0])))) == NULL)
            {
                fprintf(stderr,"cannot allocate\n");
                close(fd); close(fdw);
                exit(1);
            }
            word[charNum] = c[0];
            charNum += 1;
        }

        // if not, check the word length and if it is a duplicated word
        else
        {
            if(charNum >= wordLength)
            {
                // add '\0' to the end of the word
                if((word = (realloc(word, (charNum+1)* sizeof(word[0])))) == NULL) // get the word with correct length
                {
                    fprintf(stderr,"cannot allocate\n");
                    close(fd); close(fdw);
                    exit(1);
                }
                word[charNum] = '\0';

                // write the word to file and add it ti the wordList for checking duplicated word;
                wordList = writeWords(fd, fdw, &wordNum, word, wordList);
            }

            // begin read a new word from the file
            word = NULL;
            charNum = 0;
        }
    }

    // free the allocated memory
    for(int i = 0; i < wordNum; i++)
    {
        free(wordList[i]);

    }
    free(wordList);

    printf("\n+\n");

    // finish dealing a file
    fflush(stdout);
    dup2(savfd, STDOUT_FILENO);
    close(savfd);
    close(fd);
    printf("\tfinding words in\t%s\n", realPath);
    printf("\tfound %d words\n", wordNum);
}
