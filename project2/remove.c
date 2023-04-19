#include "trove.h"

bool isFile(char* filename)
{
    struct stat path;

    stat(filename, &path);

    int ret = S_ISREG(path.st_mode);
    if(ret  == 0)
        return false;
    return true;
}


void reWrite(MYFILE file, int fdw)
{
    int savfd = dup(STDOUT_FILENO);
    dup2(fdw, STDOUT_FILENO);

    printf("%s\n", file.filePath);

    for(int i = 0; i < file.wordNum; i++)
    {
        printf("%s  ",file.fileContent[i]);
    }
    printf("\n+\n");
    fflush(stdout);
    dup2(savfd, STDOUT_FILENO);
    close(savfd);
}



void getRealpath(char* removeFnames[filenum], char** fileList, int filenum)
{
    for(int i = 0; i < filenum; i++)
    {
        char  realPath[MAXPATHLEN];
        char *res = realpath(fileList[i], realPath);
        char *dupRealpath = strdup(realPath);
        if (!res)
        {
            perror("realpath()");
            exit(EXIT_FAILURE);
        }

        if(isFile(realPath))
        {
            removeFnames[i] = dupRealpath;
        }
        else
        {
            fprintf(stderr,"please input correct file path\n");
            usage();
        }

    }
}



void removeFile(char ** fileList, int filenum)
{
    if(filenum == 0)
    {
        usage();
    }

    char *removeFnames[filenum]; // a array storing the names of all file we want to remove
    getRealpath(removeFnames,fileList, filenum);


    PMYFILE pf; // a pointer points to MYFILE datatype
    int removeFilenum = 0; // the number of files we want to remove
    pf = parsingFile(&removeFilenum);

    int fdw;
    if(( fdw = open(fileName, O_WRONLY | O_CREAT |O_TRUNC , 0644)) < 0)
    {
        fprintf(stderr,"cannot open for write\n");
        exit(EXIT_FAILURE);
    }

    // remove files
    for(int i = 0; i < removeFilenum; i++)
    {   bool isRemove = false;
        for(int j = 0; j < filenum; j++)
        {
            if(!strcmp(pf[i].filePath, removeFnames[j]))
            {
                isRemove = true;
                printf("\tremove %s\n",removeFnames[j]);
            }
        }
        if(!isRemove)
        {
            reWrite(pf[i], fdw);
        }
    }
    close(fdw);
    copressFile(fileName);

}



