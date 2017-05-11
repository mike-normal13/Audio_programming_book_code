CC = gcc
LIBS = -lm
PROGS = wonder midi2freq freq2midi cpsmidi nscale iscale breakdur expbrk expad expdecay sinetext sinetext2 tfork tfork2 tforkraw

all: ${PROGS}

clean: 
	rm -f ${PROGS} 

wonder:	wonder.c
	${CC} wonder.c -o wonder

midi2freq: midi2freq.c
	${CC} ${LIBS} midi2freq.c -o midi2freq

freq2midi: freq2midi.c
	${CC} ${LIBS} freq2midi.c -o freq2midi

cpsmidi: cpsmidi.c
	${CC} ${LIBS} cpsmidi.c -o cpsmidi

nscale:	nscale.c
	${CC} ${LIBS} nscale.c -o nscale

iscale:	iscale.c
	${CC} ${LIBS} iscale.c -o iscale

breakdur: breakdur.c
	${CC}  ${LIBS} breakdur.c -o breakdur

expbrk: expbrk.c
	${CC}  ${LIBS} expbrk.c -o expbrk

expad: expad.c
	${CC}  ${LIBS} expad.c -o expad

expdecay: expdecay.c
	${CC}  ${LIBS} expdecay.c -o expdecay

sinetext: sinetext.c
	$(CC) -o sinetext sinetext.c  ${LIBS}

sinetext2: sinetext2.c
	$(CC) -o sinetext2 sinetext2.c  ${LIBS}

tfork:	tfork.c
	$(CC) -o tfork tfork.c  ${LIBS}

tfork2:	tfork2.c
	$(CC) -o tfork2 tfork2.c  ${LIBS}

tforkraw: tforkraw.c
	$(CC) -o tforkraw tforkraw.c  ${LIBS}
