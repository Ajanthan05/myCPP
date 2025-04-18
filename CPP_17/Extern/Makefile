CC = gcc
CPP = g++

all: testmain

testmain: testmain.c util.o
	$(CPP) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f testmain *.o