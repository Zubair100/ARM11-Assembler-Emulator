CC      = gcc
CFLAGS  = -Wall -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic -g

.SUFFIXES: .c .o

.PHONY: all clean

assemble: branch_instructions.c dp_instructions.c generate_binary_instructions.c mul_instructions.c sdt_instructions.c special_instructions.c tokeniser.c utilities.c assemble.o

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
