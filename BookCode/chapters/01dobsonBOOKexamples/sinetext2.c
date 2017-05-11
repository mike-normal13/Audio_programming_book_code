/* sinetext2.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI (3.141592654)
#endif

/* define our program argument list */
enum {ARG_NAME,ARG_NSAMPS,ARG_FREQ,ARG_SR,ARG_NARGS};

int main(int argc, char** argv)
{
	int i,nsamps;
	double samp,freq,srate;
	double twopi = 2.0 * M_PI;
	double angleincr;
	
	if(argc != ARG_NARGS){
		fprintf(stderr,"Usage: sinetext2 nsamps freq srate\n");
		return 1;
	}
	nsamps    = atoi(argv[ARG_NSAMPS]);
	freq      = atof(argv[ARG_FREQ]);
	srate     = atof(argv[ARG_SR]);
	angleincr = twopi*freq/srate;

	for(i=0;i < nsamps; i++){
		samp = sin(angleincr*i);
		fprintf(stdout,"%.8lf\n",samp);
	}
	
	fprintf(stderr,"done.\n");
	return 0;
}