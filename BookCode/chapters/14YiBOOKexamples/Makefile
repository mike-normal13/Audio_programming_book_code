
SRCS=main.cpp Note.cpp Performer.cpp PerformerGroup.cpp
OBJS=main.o Note.o Performer.o PerformerGroup.o

all: main

main: $(OBJS)
	g++ -o $@ $(OBJS)

sco: main
	./main > example.sco

wav: sco
	csound example.orc example.sco -Wo example.wav

mp3: wav
	lame -b192 example.wav example.mp3

orc: 
	blue -j createOrc.jy

.PHONY: clean
    
clean:
	-rm *.o *.exe main main.exe example.sco example.wav example.mp3
