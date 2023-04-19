# include "trove.h"

bool setOptionAlready = false;
enum options option = SEARCH;
char *fileName = "/tmp/trove";
//char **cmlWords;
int fdw = 0;
int filenum = 0;
int wordLength = 4;
int cmlWordnum = 0;
char ** realPathList = NULL;
bool setLength = false;
int thepipe[2];
