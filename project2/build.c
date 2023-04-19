#include "trove.h"

void  traverseFileRecursive(char *basePath)
{
    char path[MAXPATHLEN];
    struct dirent *dp;
    DIR *dir = opendir(basePath);


    if (!dir) // if not dir, then  enter
    {
        // get realpath
        char  filePath[MAXPATHLEN];
        char *res = realpath(basePath, filePath);
        char *dupName = strdup(filePath);

        if (!res)
        {
            perror("realpath");
            exit(EXIT_FAILURE);
        }

        //check duplicated path
        if(realPathList == NULL)
        {
            realPathList  = realloc(realPathList, (filenum+1)* sizeof(realPathList[0]));
            realPathList[filenum] = dupName;
        }
        else
        {
            for(int i = 0; i < filenum; i++)
            {
                if(!strcmp(filePath, realPathList[i]))
                {
                    return;
                }
            }
            realPathList  = realloc(realPathList, (filenum+1)* sizeof(realPathList[0]));
            realPathList[filenum] = dupName;
        }
        filenum += 1;

        return; // return and then began next dir
    }

    // if it is dir, open and traverse
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            traverseFileRecursive(path);
        }
    }
    closedir(dir);

}

void buildFile(char **cmlWords)
{

    // open and clean original file content
    int fdw;
    if(( fdw = open(fileName, O_WRONLY | O_CREAT |O_TRUNC, 0644)) < 0)
    {
        fprintf(stderr,"cannot open for write\n");
        exit(EXIT_FAILURE);
    }

    // get all file path and store them into realPathList
    for(int i = 0; i < cmlWordnum; i++)
    {
        traverseFileRecursive(cmlWords[i]);
    }


    // every loop read and write one file
    for(int i = 0; i < filenum; i++)
    {

        dealOneFile(realPathList[i], fdw);
    }
    close(fdw);

    printf("\tfinish build %d files\n",filenum);
    free(realPathList);
    copressFile(fileName);
}
