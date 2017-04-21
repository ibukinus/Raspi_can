# Makefile

all: can_lib.o
	gcc -Wall -O2 -o can_lib can_lib.o

can_lib.o: can_lib.c
	gcc -c can_lib.c

clean:
	rm -f can_lib can_lib.o
