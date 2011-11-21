CC=clang

all:
	${CC} -g -I/usr/include -L/usr/lib64 -lbsd -posix -pedantic -Wall -o enkku englanti-suomi.c
