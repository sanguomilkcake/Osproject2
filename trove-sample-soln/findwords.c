#include "trove.h"
#include <ctype.h>

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/

void find_words(char *filename, off_t nbytes)
{
    if(debug) {
        printf("\tfinding words in \"%s\"\n", filename);
    }

    char    absolute[MAXPATHLEN];
    realpath(filename, absolute);

    int fd  = open(absolute, O_RDONLY, 0);
    if(fd >= 0) {

//  ALLOCATE MEMORY TO STORE WHOLE FILE IN A CHARACTER ARRAY
//  THIS WILL LIKELY FAIL ON VERY LARGE FILES, SUCH AS 4K VIDEOS!
        char *contents  = malloc(nbytes);
        CHECK_ALLOC(contents);

        if(contents) {
//  READ WHOLE FILE INTO contents ARRAY
            if(read(fd, contents, nbytes) == nbytes) {
                int  nfound = 0;
                int  fileN  = add_filename(absolute);
                char *cend  = contents+nbytes;
                char *c     = contents;

//  SCAN EACH CHARACTER IN contents
                while(c < cend) {
                    char word[BIGGEST_WORD];
                    char *wend  = word+sizeof(word);
                    char *w     = word;

//  WHILE WE HAVE AN ALPHANUMERIC CHARACTER, BUILD UP NEXT WORD
                    while(c < cend && w < wend && isalnum(*c)) {
                        *w++  = *c++;
                    }
//  IS THIS NEW WORD LONG ENOUGH?
                    if((w - word) >= minlength) {
                        *w  = '\0';
//                      printf("\t\t'%s'\n", word);
                        add_word(word, fileN);
                        ++nfound;
                    }
//  SKIP OVER ANY NON-ALPHANUMERIC CHARACTERS BETWEEN WORDS
                    while(c < cend && !isalnum(*c)) {
                        c++;
                    }
                }

                free(contents);
                close(fd);
                if(debug) {
                    printf("\tfound %i word%s\n", nfound, PLURAL(nfound));
                }
                return;
            }
        }
        close(fd);
    }
    else {
        fprintf(stderr, "cannot open file \"%s\"\n", filename);
    }
}
