#
#	makefile for text2sf
#
#
CC=gcc
CFLAGS= -ansi -O3 -Dunix -I ./include
PORTSF = -L./lib -lportsf

.c.o:
	$(CC) $(CFLAGS) -c $<


#
#	targets
#
all:	text2sf 

clean:
	-rm -f text2sf.o

./lib/portsf.a:
	cd portsf; make; make install

text2sf: ./lib/portsf.a	text2sf.o 
	$(CC) $(CFLAGS) text2sf.o  -o text2sf $(PORTSF)

#
#	dependencies
#
