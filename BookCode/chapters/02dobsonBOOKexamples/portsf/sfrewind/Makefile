#makefile for sfrewind using portsf
# to test write, seek back, read and overwrite

# CFLAGS = -I ../include -D_DEBUG -g
CFLAGS = -Dunix -O2 -I ../../include
PSF = ../../lib/libportsf.a

CC = gcc

.PHONY:	clean $(PSF)

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

all:	sfrewind

sfrewind:	sfrewind.o $(PSF)
	$(CC) -o sfrewind sfrewind.o $(PSF) -lm

clean:
	rm -f *.o
	rm -f sfrewind

$(PSF):	
	cd ..; make libportsf.a; make install

