PROJECT = trove
HEADERS = $(PROJECT).h
OBJ = trove.o globals.o parsing.o build.o search.o \
remove.o update.o dealonefile.o compressfile.o decompressfile.o

C11     =  cc -std=c11
CFLAGS  =  -Wall -pedantic -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm

trove.o : trove.c $(HEADERS)
	$(C11) $(CFLAGS) -c trove.c

globals.o : globals.c $(HEADERS)
	$(C11) $(CFLAGS) -c globals.c


parsing.o : parsing.c $(HEADERS)
	$(C11) $(CFLAGS) -c parsing.c

build.o : build.c $(HEADERS)
	$(C11) $(CFLAGS) -c build.c

search.o : search.c $(HEADERS)
	$(C11) $(CFLAGS) -c search.c

remove.o : remove.c $(HEADERS)
	$(C11) $(CFLAGS) -c remove.c

update.o : update.c $(HEADERS)
	$(C11) $(CFLAGS) -c update.c

dealonefile.o : dealonefile.c $(HEADERS)
	$(C11) $(CFLAGS) -c dealonefile.c

compressfile.o : compressfile.c $(HEADERS)
	$(C11) $(CFLAGS) -c compressfile.c

decompressfile.o : decompressfile.c $(HEADERS)
	$(C11) $(CFLAGS) -c decompressfile.c

clean:
	rm -f $(PROJECT) $(OBJ)
