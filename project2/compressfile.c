# include "trove.h"
void wait_for_one(void)
{
    int status;

    wait(&status);
    if(status != 0)
    {
        fprintf(stderr,"fail to compress/decompress file\n");
        exit(1);
    }


}

void copressFile(char* name)
{
    int pid;
    pid = fork();
    switch (pid)
    {
        case -1:
        {
            perror("fork()");
            exit(1);
        }
        case  0:

        {
            execl("/usr/bin/gzip","gzip","-f", name, NULL);
            perror("/usr/bin/gzip");
            exit(EXIT_FAILURE);

        }
        default:
        {
            printf("\tcompressing file\n");
            break;
        }
    }
    wait_for_one();
}
