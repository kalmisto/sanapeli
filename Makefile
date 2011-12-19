CC=clang
CFLAGS=-posix -pedantic -Wall
CFLAGS+=-I/usr/include -L/usr/lib64 -lbsd

ifeq ($(OS), LINUX)
	CFLAGS+=-I/usr/include -L/usr/lib64 -lbsd
	echo "HEI"
	echo "HEI"
	echo "HEI"
	echo "HEI"
	echo "HEI"
endif

all: slist
	${CC} ${CFLAGS} -g -o enkku englanti-suomi.c slist.o
slist:
	${CC} ${CFLAGS} -c -o slist.o slist.c
