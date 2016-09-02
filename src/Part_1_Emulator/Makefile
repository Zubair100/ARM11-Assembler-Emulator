CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

emulate: emulate_helper_functions.c emulate.o 

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
