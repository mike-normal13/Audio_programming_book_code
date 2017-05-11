CSOUND = csound
CC = gcc

all:	DDC4.wav

DDC4.create:	algoTrappedFinal.c
	$(CC) -o DDC4.create -O3 algoTrappedFinal.c -lm

algoTrappedFinal.sco:	DDC4.create
	./DDC4.create > algoTrappedFinal.sco

DDC4.wav:	algoTrappedFinal.orc algoTrappedFinal.sco
	$(CSOUND) -W algoTrappedFinal.orc algoTrappedFinal.sco -d -t0 -o DDC4.wav

dac:	algoTrappedFinal.orc algoTrappedFinal.sco
	$(CSOUND) -W algoTrappedFinal.orc algoTrappedFinal.sco -d  -t0 -o dac

