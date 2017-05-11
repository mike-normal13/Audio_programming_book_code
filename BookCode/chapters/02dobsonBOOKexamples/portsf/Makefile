#makefile for portsf
POBJS = ieee80.o portsf.o

# CFLAGS = -I ../include -D_DEBUG -g
# on strange 64 bit platforms must define CPLONG64
CFLAGS = -Dunix -O2 -I ../include

CC=gcc

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean sfreverse sfrewind
all:	libportsf.a sfreverse sfrewind


clean:
	-rm -f $(POBJS)
	cd sfrewind; make clean;
	cd sfreverse; make clean;

veryclean:
	-rm -f $(POBJS) 
	cd sfrewind; make clean;
	cd sfreverse; make clean;
	rm -f libportsf.a; 

libportsf.a:	$(POBJS)
	ar -rc libportsf.a $(POBJS)
	ranlib  libportsf.a

install:	libportsf.a
	cp libportsf.a ../lib

sfreverse:	
	cd sfreverse; make

sfrewind:	
	cd sfrewind; make
	
#
#	dependencies
#
portsf.c:	../include/portsf.h ieee80.h
