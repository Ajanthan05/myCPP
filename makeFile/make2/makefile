BINARY=bin
CODEDIRS=. lib
INCDIRS=. ./include/

CC=gcc
OPT=O0

# generate fies that encode make rules for the .h dependencies
DEPFLAGS=-MP -MD

# AUTOMATICALLY add I into all include directory
CFLAGS=-Wall -Wextra -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

CFILES=$(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))

OBJECTS=$(patsubst %.c,%.o,$(CFILES))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

all : $(BINARY)

$(BINARY) : $(OBJECTS)
		$(CC) -o $@ $^

# only want .c files dependency here, thus $< instead of $^
%.o : %.c
		$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

distribute: clean
	tar zcvf dist.tar *

# @ silences the printing of the command
# $(info ...) prints output

diff:
		$(info The status of the repository, and the volume of per-file changes:)
		@git status
		@git diff --stat

# include the dependencies
-include $(DEPFILES)
