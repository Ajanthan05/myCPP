BINARY=bin
CODEDIRS=./sorceFiles
INCDIRS=. ./headerFiles

CC=g++
OPT=O0

# generate fies that encode make rules for the .h dependencies
DEPFLAGS=-MP -MD

# AUTOMATICALLY add I into all include directory
CFLAGS=-Wall -Wextra -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

CFILES=$(foreach D, $(CODEDIRS), $(wildcard $(D)/*.cpp))
$(info CFILES = $(CFILES))
# @echo $(CFILES)

OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

all : $(BINARY)

$(BINARY) : $(OBJECTS)
		$(CC) -o $@ $^

# only want .cpp files dependency here, thus $< instead of $^
%.o : %.cpp
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
