#include "trove.h"

//  trove, written by Chris.McDonald@uwa.edu.au, September 2022
//  under licence - creativecommons.org/licenses/by-nc-sa/4.0/


//  FUNCTION hash_string() ACCEPTS A STRING PARAMETER,
//  AND RETURNS AN UNSIGNED 32-BIT INTEGER AS ITS RESULT
//
//  see:  https://en.cppreference.com/w/c/types/integer

static uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

//  --------------------------------------------------------------------

int nwords  = 0;

//  FOR EACH UNIQUE WORD (NODE) WE STORE THE WORD ITSELF, THE NUMBER OF FILES
//  IN WHICH IT APPEARS, AND A VECTOR OF THE FILE-NUMBERS WHERE IT'S FOUND
typedef struct _list {
    char            *word;
    int             nfiles;
    int             *files;
    struct _list    *next;
} WORDLIST;

//  WE DEFINE A WORDTABLE AS AN ARRAY OF WORDLISTs
typedef	WORDLIST    *WORDTABLE;
static  WORDTABLE   *wt   = NULL;

#define FOREACH_LIST_IN_WORDTABLE       for(int h=0 ; h<HASHTABLE_SIZE ; ++h)


//  DETERMINE IF A REQUIRED word IS STORED IN A GIVEN LIST
static WORDLIST *list_find(WORDLIST *list, char *word)
{
    while(list != NULL) {
	if(strcmp(list->word, word) == 0) {
	    return list;
	}
	list	= list->next;
    }
    return NULL;
}

//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
static WORDLIST *list_new_item(char *word, int fileN)
{
    WORDLIST *new   = malloc(sizeof(*new));
    CHECK_ALLOC(new);

    new->word       =  strdup(word);
    CHECK_ALLOC(new->word);
    new->nfiles     = 1;
    new->files      = malloc(sizeof(new->files[0]));
    CHECK_ALLOC(new->files);
    new->files[0]   = fileN;

    new->next       =  NULL;
    return new;
}

static WORDLIST *list_add(WORDLIST *list, char *word, int fileN)
{
    WORDLIST *old   = list_find(list, word);

    if(old == NULL) {
        WORDLIST *new   = list_new_item(word, fileN);
        new->next       = list;
        ++nwords;
        return new;
    }
    else {
        for(int f=0 ; f<old->nfiles ; ++f) {
            if(old->files[f] == fileN) {
                return list;
            }
        }
        old->files      = realloc(old->files, (old->nfiles+1) * sizeof(old->files[0]));
        CHECK_ALLOC(old->files);
        old->files[old->nfiles]   = fileN;
        old->nfiles     += 1;
        return list;
    }
}

void add_word(char *word, int fileN)
{
    if(wt == NULL) {
        wt      = calloc(HASHTABLE_SIZE, sizeof(wt[0]));
        CHECK_ALLOC(wt);
    }

    uint32_t h  = hash_string(word) % HASHTABLE_SIZE;    // choose list
    wt[h]       = list_add(wt[h], word, fileN);
}

//  --------------------------------------------------------------------

//  WRITE WORDS IN WORDTABLE TO TROVEFILE
int write_words_to_trovefile(int fd)
{
    FILE *fp    = fdopen(fd, "w");

    int nwords   = 0;
    FOREACH_LIST_IN_WORDTABLE {
        WORDLIST *list  = wt[h];

        while(list != NULL) {
            fprintf(fp, "%s\n", list->word);
            for(int f=0 ; f<list->nfiles ; ++f) {
                fprintf(fp, "+%i\n", list->files[f]);
            }
            ++nwords;
            list    = list->next;
        }
    }
    fflush(fp);
//  WE DO NOT CLOSE fd OR fp
    return nwords;
}

//  --------------------------------------------------------------------

int remove_fileN_from_words(int fileN)
{
    if(debug) {
        printf("removing file from trove-file\n");
    }

    int nfound  = 0;
    FOREACH_LIST_IN_WORDTABLE {
        WORDLIST *list  = wt[h];

        while(list != NULL) {
            for(int f=0 ; f<list->nfiles ; ++f) {
                if(list->files[f] == fileN) {
                    list->files[f]  = list->files[ list->nfiles-1 ];
                    --list->nfiles;
                    --f;
                    ++nfound;
                }
                else if(list->files[f] > fileN) {
                    --list->files[f];
                }
            }
            list    = list->next;
        }
    }
    return nfound;
}

//  --------------------------------------------------------------------

int search_words(char *word)
{
    if(debug) {
        printf("\tsearching for \"%s\"\n", word);
    }

    uint32_t h  = hash_string(word) % HASHTABLE_SIZE;   // choose list
    WORDLIST *list  = wt[h];

    while(list != NULL) {
        if(strcmp(list->word, word) == 0) {         // found it
            for(int f=0 ; f<list->nfiles ; ++f) {
                char *absolute  = filenames[ list->files[f] ];

                if(access(absolute, F_OK) == 0) {   // still exists?
                    printf("%s\n", absolute);
                }
            }
            return list->nfiles;
        }
        list    = list->next;
    }
    return 0;
}
