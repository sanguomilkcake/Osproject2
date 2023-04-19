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

    else // check duplicated words
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

            //char * word = strdup(word);
            printf("%s  ", word);
            wordList[*wordNum] = word;
            //printf("%s\n",wordDup);
            *wordNum += 1;
        }
    }
    return wordList;

}

void dealOneFile(char *realPath, int fdw)
{
    int fd;
    if((fd = open(realPath ,O_RDONLY)) < 0)
    {
        fprintf(stderr,"cannot open for read '%s'\n", realPath);
        exit(EXIT_FAILURE);
    }

    size_t got;
    char **wordList = NULL;
    char c[1];
    char* word = NULL;
    int charNum = 0;
    int wordNum = 0;

    int savfd = dup(STDOUT_FILENO);
    dup2(fdw, STDOUT_FILENO);
    printf("%s\n",realPath);

    while((got = read(fd, c, sizeof c[0])) > 0)// get a char
    {
        //printf("%c", c[0]);
        // if it is a char, then char to add to current word
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

        // if not add the right word to the list for checking dup and write the word to file
        else
        {
            if(charNum >= wordLength)
            {
                if((word = (realloc(word, (charNum+1)* sizeof(word[0])))) == NULL) // get the word with correct length
                {
                    fprintf(stderr,"cannot allocate\n");
                    close(fd); close(fdw);
                    exit(1);
                }
                word[charNum] = '\0';
                // check duplicate words

                //char * wordDup = strdup(word);

                /*if(wordList == NULL) // if NULL in list then put the first char
                {
                    if ((wordList  = realloc(wordList, (wordNum+1)* sizeof(wordList[0]))) == NULL)
                    {
                        fprintf(stderr, "cannot allocate\n");
                        close(fd); close(fdw);
                        exit(1);
                    }

                    printf("%s  ", word);
                    wordList[wordNum] = word;

                    wordNum += 1;
                }

                else // check duplicated words
                {
                    bool isDuplicated = false;
                    for(int i = 0; i < wordNum; i++)
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
                        wordList  = realloc(wordList, (wordNum+1)* sizeof(wordList[0]));
                        if(wordList == NULL)
                        {
                            fprintf(stderr,"cannot allocate\n");
                            close(fd); close(fdw);
                            exit(1);
                        }

                        //char * word = strdup(word);
                        printf("%s  ", word);
                        wordList[wordNum] = word;
                        //printf("%s\n",wordDup);
                        wordNum += 1;
                    }
                }*/
                wordList = writeWords(fd, fdw, &wordNum, word, wordList);
            }

            word = NULL;
            charNum = 0;
        }
    }

    for(int i = 0; i < wordNum; i++)
    {
        free(wordList[i]);

    }
    free(wordList);
    // finish store a file

    printf("\n+\n");
    fflush(stdout);
    dup2(savfd, STDOUT_FILENO);
    close(savfd);
    close(fd);
    printf("\tfinding words in\t%s\n", realPath);
    printf("\tfound %d words\n", wordNum);
}

/*
int main(int argc, char *argv[])
{
    int fd;
    int fdw;
    //printf("\tfinding words in\t%s\n", argv[1]);
     // read file
    if((fd = open(argv[1] ,O_RDONLY)) < 0)
    {
        fprintf(stderr,"cannot open for read '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if((fdw = open(argv[2], O_WRONLY | O_CREAT |O_APPEND, 0644)) < 0)
    {
        fprintf(stderr,"cannot open for write'%s'\n", argv[2]);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // get all right length words from one file;
    size_t got;
    char **wordList = NULL;
    char c[1];
    char* word = NULL;
    int charNum = 0;
    int wordNum = 0;

    int savfd = dup(STDOUT_FILENO);
    dup2(fdw, STDOUT_FILENO);
    printf("%s\n",argv[1]);

    while((got = read(fd, c, sizeof c[0])) > 0)// get a char
    {
        //printf("%c", c[0]);
        // if it is a char, then char to add to current word
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

        // if not add the right word to the list for checking dup and write the word to file
        else
        {
            if(charNum >= atoi(argv[3]))
            {
                if((word = (realloc(word, (charNum+1)* sizeof(word[0])))) == NULL) // get the word with correct length
                {
                    fprintf(stderr,"cannot allocate\n");
                    close(fd); close(fdw);
                    exit(1);
                }
                word[charNum] = '\0';
                // check duplicate words

                //char * wordDup = strdup(word);

                if(wordList == NULL) // if NULL in list then put the first char
                {
                    if ((wordList  = realloc(wordList, (wordNum+1)* sizeof(wordList[0]))) == NULL)
                    {
                        fprintf(stderr, "cannot allocate\n");
                        close(fd); close(fdw);
                        exit(1);
                    }

                    printf("%s  ", word);
                    wordList[wordNum] = word;

                    wordNum += 1;
                }

                else // check duplicated words
                {
                    bool isDuplicated = false;
                    for(int i = 0; i < wordNum; i++)
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
                        wordList  = realloc(wordList, (wordNum+1)* sizeof(wordList[0]));
                        if(wordList == NULL)
                        {
                            fprintf(stderr,"cannot allocate\n");
                            close(fd); close(fdw);
                            exit(1);
                        }

                        //char * word = strdup(word);
                        printf("%s  ", word);
                        wordList[wordNum] = word;
                        //printf("%s\n",wordDup);
                        wordNum += 1;
                    }
                }
            }
            word = NULL;
            charNum = 0;
        }
    }

    for(int i = 0; i < wordNum; i++)
    {
        free(wordList[i]);

    }
    free(wordList);
    // finish store a file

    printf("\n+\n");
    fflush(stdout);
    dup2(savfd, STDOUT_FILENO);
    close(savfd);
    close(fd);
    close(fdw);
    printf("\tfinding words in\t%s\n", argv[1]);
    printf("\tfound %d words\n", wordNum);
    exit(0);

}
*/
