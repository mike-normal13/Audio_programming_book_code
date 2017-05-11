/* expad.c  generate exponential attack or decay breakpoint data */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char** argv)
{
	int i,npoints;
	double T,k,a;
	double ystart,yend;
    double dur,thisval,thisstep;

	if(argc != 6){
		fprintf(stderr,"insufficient arguments.\n");
		fprintf(stderr,"Usage: expad duration npoints startval endval T\n");
		return 1;
	}
    dur = atof(argv[1]);
	if(dur <=0.0){
		fprintf(stderr,"Error: duration must be positive.\n");
		return 1;
	}
	npoints = atoi(argv[2]);
	if(npoints <= 0){
		fprintf(stderr,"Error: npoints must be positive!\n");
		return 1;
	}
	ystart = atof(argv[3]);
	yend = atof(argv[4]);  
	if(yend == ystart){
		fprintf(stderr,"Warning: start and end values are the same!\n");
	}

	T = atof(argv[5]);
    k = dur/npoints;    /* npoints = "sample rate" */
	a = exp(-k/T);	    /* T = time constant */	
	
    thisstep = 0.0;
	thisval = ystart;
	fprintf(stderr,"a = %.6lf\n",a);
    /* make normalized curve, scale output to input values and range */  
    
    for(i=0;i < npoints;i++){
		printf("%.4lf\t%.8lf\n",thisstep,thisval);		
		thisval = a * thisval + (1.0-a) * yend;
		thisstep += k;
    }
    /* print final value */
	printf("%.4lf\t%.8lf\n",thisstep,thisval);
    
    fprintf(stderr,"done\n");
    return 0;
}
        
/* EXERCISE!  reduce the computations  inside the loop */    
	
	
	
