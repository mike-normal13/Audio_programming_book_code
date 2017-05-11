/* sinetext.c */
/* write  sinewave as text  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* conditional compilation - is M_PI defined? */
#ifndef M_PI
#define M_PI (3.141592654)
#endif

int main(int argc, char** argv)
{
	int i,nsamps;
	double samp;
	double twopi = 2.0 * M_PI;
	double angleincr;
	/* set number of points to create */
	nsamps = 50;
	/* make one complete cycle */
    angleincr = twopi / nsamps;
	for(i=0;i < nsamps; i++){
		samp = sin(angleincr *i);		
		fprintf(stdout,"%.8lf\n",samp);		
	}	
	fprintf(stderr,"done\n");
	return 0;
}