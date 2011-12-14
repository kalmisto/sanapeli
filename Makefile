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

all:
	${CC} ${CFLAGS} -g -o enkku englanti-suomi.c
slist:
	${CC} ${CFLAGS} -o slist slist.c
