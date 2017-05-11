#makefile for sfreverse using portsf
# to test psf_sndSeek etc 

# CFLAGS = -I ../include -D_DEBUG -g
CFLAGS = -Dunix -I ../../include
PSF = ../../lib/libportsf.a
CC = gcc
.PHONY:	clean $(PSF)

.c.o:	$(CC) -c $(CFLAGS) $< -o $@ 

all:	sfreverse

sfreverse:	sfreverse.o $(PSF)
	$(CC) -o sfreverse sfreverse.o $(PSF) -lm
clean:
	rm -f *.o
	rm -f sfreverse

$(PSF):	
	cd ..; make libportsf.a; make install

