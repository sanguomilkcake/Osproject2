# include "trove.h"

void decompossFile(char * name, int thepipe[2])
{
    // change the original file name
    char *newname = malloc(strlen(name) + 4);
    strcpy(newname, name);
    strcat(newname, ".gz");

    if(pipe(thepipe) != 0)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    switch (fork())
    {
        case 0:
            {
                close(thepipe[0]); // close read end
                dup2(thepipe[1], 1); // output to pip[1]
                close(thepipe[1]);

                execl("/usr/bin/zcat", "zcat", newname, NULL);
                perror("/usr/bin/zcat\n");
                exit(EXIT_FAILURE);
                break;
            }

        default:
            {
                close(thepipe[1]); // close read end
                printf("\tdecompressing %s\n", name);
                break;
            }
    }
    wait_for_one();

}


