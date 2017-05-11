INCLUDES 	= -I./include
LIBS 		= -L./lib -lportsf -lm
PROGS = sf2float sfgain sfnorm sfpan envx sfenv siggen oscgen tabgen
PORTSF=./lib/libportsf.a
CC = gcc

all: $(PROGS)

$(PORTSF): 
	cd portsf; make; make install

sf2float: sf2float.c $(PORTSF)
	$(CC)  -o sf2float sf2float.c $(INCLUDES) $(LIBS)

sfgain: sfgain.c $(PORTSF)
	$(CC)  -o sfgain sfgain.c $(INCLUDES) $(LIBS)

sfnorm: sfnorm.c $(PORTSF)
	$(CC)  -o sfnorm sfnorm.c $(INCLUDES) $(LIBS)

sfpan: sfpan.c breakpoints.c $(PORTSF)
	$(CC)  -o sfpan sfpan.c breakpoints.c $(INCLUDES) $(LIBS)

envx: envx.c $(PORTSF)
	$(CC)  -o envx envx.c $(INCLUDES) $(LIBS)

sfenv: sfenv.c  breakpoints.c $(PORTSF)
	$(CC)  -o sfenv sfenv.c breakpoints.c $(INCLUDES) $(LIBS)

siggen: siggen.c wave.c  breakpoints.c $(PORTSF)
	$(CC)  -o siggen siggen.c wave.c breakpoints.c $(INCLUDES) $(LIBS)

oscgen: oscgen.c gtable.c breakpoints.c $(PORTSF)
	$(CC)  -o oscgen oscgen.c gtable.c breakpoints.c $(INCLUDES) $(LIBS)

tabgen: tabgen.c gtable.c breakpoints.c $(PORTSF)
	$(CC)  -o tabgen tabgen.c gtable.c breakpoints.c $(INCLUDES) $(LIBS)

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean:	clean
	cd portsf; make veryclean

