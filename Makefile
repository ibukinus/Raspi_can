# Makefile

all: sample.c can_lib.o
	gcc -Wall -O2 -o sample sample.c can_lib.o

sample.o: sample.c
	gcc -c -std=gnu11 sample.c

can_lib.o: can_lib.c
	gcc -c -std=gnu11 can_lib.c

clean:
	rm -f sample sample.o can_lib.o
