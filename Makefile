# makefile for APC524 HW1
CC=gcc
CFLAGS= -O3 -Wall -pedantic -std=c99
# CFLAGS= -g -Wall -pedantic -std=c99
EXEC = diff test dipole

.PHONY: all

all: test interp

interp: pp.o interp.o
test: pp.o test.o
	$(CC) -o $@ $^

%.o:%.c pp.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o test interp
