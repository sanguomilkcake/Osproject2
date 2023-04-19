NAME		= trove

HEADERS		= $(NAME).h
OBJ		= $(NAME).o \
		build.o \
		compression.o \
		filenames.o \
		findfiles.o \
		findwords.o \
		globals.o \
		remove.o \
		trovefile.o \
		update.o \
		words.o

CC		= cc
CFLAGS		= -std=c99 -Wall -Werror
LD		= cc


$(NAME) : $(OBJ)
	$(LD) -o $(NAME) $(OBJ)

%.o :	%.c $(HEADERS)
	$(CC) $(CFLAGS) $(DEFINES) -c $<

lines:
	cat *.[hc] | grep -v '^//' | grep -v '^$$' | wc -l

clean:
	rm -f $(NAME) $(OBJ) /tmp/trove
