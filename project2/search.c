# include "trove.h"

PMYFILE parsingFile(int *fileNum)
{
    size_t got;
    char c[1];
    PMYFILE pfile = NULL; // a PMYFILE pointer
    char* word = NULL;
    char ** fileContent = NULL; // containing words of a file
    int wordNum = 0; // the number of words in a file
    int charNum = 0; // the number of char in a word
    bool addWord = false; // a flag for first file we read or missing files
    bool isSkip = false; // if the file not exists, then skip the content


    decompossFile(fileName, thepipe);

    // start to get a char
    while((got = read(thepipe[0], c, sizeof(c)) > 0))
    {
        // if it is "+" and addWord, add content to MYFILE data type
        if(c[0] == '+')
        {
            isSkip = false;
            if(addWord)
            {
                pfile[*fileNum].fileContent = fileContent;
                pfile[*fileNum].wordNum = wordNum;
                *fileNum += 1;
                addWord = false;
                fileContent = NULL;
                wordNum = 0;

            }
            continue;

        }

        if(isSkip)
            continue;

        // avoid read new line and space
        if(c[0] != '\n' && c[0] != ' ')
        {

            if((word = (realloc(word, (charNum+1)* sizeof(word[0])))) == NULL)
            {
                fprintf(stderr,"cannot allocate memory\n");
                exit(1);
            }

            word[charNum] = c[0];
            charNum += 1;
        }
        else if(charNum > 0) // got a new word
        {
            // add the '\0' to the word
            if((word = (realloc(word, (charNum+1)* sizeof(word[0])))) == NULL)
                {
                    fprintf(stderr,"cannot allocate\n");
                    exit(1);
                }
            word[charNum] = '\0';


            if(addWord) // if need to add word to the filecontent
            {
                if((fileContent = (realloc(fileContent, (wordNum+1)* sizeof(fileContent[0])))) == NULL) // create a file type;
                {
                    fprintf(stderr,"cannot allocate memory\n");
                    exit(1);
                }
                fileContent[wordNum] = word;
                wordNum += 1;

            }
            else
            {
                if (access(word, F_OK) == 0) // if file exists;
                {
                    if((pfile = (realloc(pfile, (*fileNum+1)* sizeof(pfile[0])))) == NULL) // create a file type and add path
                    {
                        fprintf(stderr,"cannot allocate memory\n");
                        exit(1);
                    }
                    MYFILE file;
                    file.filePath = word;
                    pfile[*fileNum] = file;
                    addWord = true;

                }
                else    // if not exists
                {
                    isSkip = true;
                }
            }

            word = NULL;
            charNum = 0;
        }

    }
    close(thepipe[0]);
    return pfile;
}



void searchWord(char *targetWord)
{
    PMYFILE pf; //  a PMYFILE pointer
    int searchFilenum = 0; // the number of files we get
    pf = parsingFile(&searchFilenum);

    int numfind = 0;
    for(int i = 0; i < searchFilenum; i++)
    {
        for(int j = 0; j < pf[i].wordNum; j++)
        {
            if(!strcmp(targetWord, pf[i].fileContent[j]))
            {
                printf("%s\n",pf[i].filePath );
                numfind += 1;

            }
            free(pf[i].fileContent[j]);
        }

    }

    // if  not find any word
    if(numfind == 0)
    {
        fprintf(stderr,"not find word\n");
        exit(1);
    }

}

