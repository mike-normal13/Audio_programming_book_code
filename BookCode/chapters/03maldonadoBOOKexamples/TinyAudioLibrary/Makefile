#
#	makefile for Tiny
#
#	
#	VL 2009
#
CC=gcc

CFLAGS=-O2 -Wall  -I../include -I/usr/local/include

PSFOBJS=	tinyAudioLib.o

.c.o:
	$(CC) $(CFLAGS) -c $<
#
#	targets
#
all:	libtiny.a

libtiny.a:	$(PSFOBJS)
	ar rc libtiny.a $(PSFOBJS)
install:	libtiny.a
	cp libtiny.a ../lib/libtiny.a
	ranlib ../lib/libtiny.a

#
#	dependencies
#
tinyAudioLib.o:	../include/tinyAudioLib.h 


