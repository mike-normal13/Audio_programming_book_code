waveform: rd.o out.o main.o portsf.a
	gcc -o waveform rd.o out.o main.o portsf.a -lm

rd.o:   rd.c waveform.h
	gcc -c -g rd.c

out.o:  out.c waveform.h portsf.h
	gcc -c -g out.c

main.o: main.c waveform.h portsf.h
	gcc -c -g main.c

portsf.a: portsf.c ieee80.c portsf.h ieee80.h 
	gcc -c -g portsf.c
	gcc -c ieee80.c
	ar rv portsf.a portsf.o ieee80.o
	rm portsf.o ieee80.o

