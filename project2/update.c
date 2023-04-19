# include "trove.h"

void updateFile(char ** cmlWords, int cmlWordnum)
{
    // get a realpathList that contains all absolute path of file we want to update
    for(int i = 0; i < cmlWordnum; i++)
    {
        traverseFileRecursive(cmlWords[i]);
    }

    // remove old file information
    removeFile(realPathList,filenum);


    // rewrite the new information of files we want to update
    int fdw;
    if(( fdw = open(fileName, O_WRONLY | O_CREAT |O_APPEND, 0644)) < 0)
    {
        fprintf(stderr,"cannot open for write\n");
        exit(EXIT_FAILURE);
    }

    PMYFILE pf;
    int searchFilenum = 0;
    pf = parsingFile(&searchFilenum);

    for(int i = 0; i < searchFilenum; i++)
    {
        reWrite(pf[i],fdw);
    }


    for(int i = 0; i < filenum; i++)
    {
        dealOneFile(realPathList[i], fdw);
    }

    close(fdw);

    printf("\tfinish update %d files\n",filenum);
    free(realPathList);
    copressFile(fileName);

}

























