/* Copyright (c) 2009 Richard Dobson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
/* envx.c :  extract amplitude envelope from mono soundifle*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>

#define DEFAULT_WINDOW_MSECS (15.0)

double maxsamp(const float* buf, unsigned long blocksize);

double maxsamp(const float* buf, unsigned long blocksize)
{
	double absval,peak = 0.0;
	unsigned long i;

	for(i=0; i < blocksize; i++) {
		absval = fabs(buf[i]);
		if(absval > peak)
			peak = absval;
	}
	return peak;
}

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_NARGS};

int main(int argc, char* argv[])
{
	PSF_PROPS inprops;														/* STAGE 1 */
	long framesread;	
	/* init all dynamic resources to default states */
	int ifd = -1;
	int error = 0;	
	float *inframe = NULL;
	/* breakpoint creation vars */
	unsigned long winsize,npoints;
	double	brktime, windur	 = DEFAULT_WINDOW_MSECS;
	FILE* fp = NULL;


	printf("ENVX: extract amplitude envelope from mono soundfile\n");		/* STAGE 2 */

	/* one optional flag: -wN */
	if(argc > 1){
		char flag;
		while(argv[1][0] == '-'){
			flag = argv[1][1];
			switch(flag){
			/*TODO: handle any  flag arguments here */
			case('\0'):
				printf("Error: missing flag name\n");
				return 1;
			case('w'):
				windur = atof(&argv[1][2]);
				if(windur<=0.0){
					printf("bad value for Window Duration. Must be positive.\n");
					return 1;
				}				
				/* READER: TODO: check windur not longer than infile! */
				break;
			default:
				break;
			}
			argc--;
			argv++;
		}
	}

	/* check rest of commandline */
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			"usage: envx [-wN] insndfile outfile.brk\n"
			"        -wN:   set extraction window size to N msecs\n"
			"              (default: 15)\n" 
			);
		return 1;
	}
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
		
	ifd = psf_sndOpen(argv[ARG_INFILE],&inprops,0);									/* STAGE 3 */										  
	if(ifd < 0){
		printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
		error++;
		goto exit;
	}
	/* verify infile format is acceptable */
	if(inprops.chans > 1){
		printf("Soundfile contains %d channels: must be mono.\n",inprops.chans);
		error++;
		goto exit;
	}
	
	/* allocate space for  sample frame buffer ...*/

	/*  set buffersize to the required envelope window size */
	windur /= 1000.0;  /* convert to secs */
	winsize = (unsigned long)(windur * inprops.srate);
	/* READER TODO: winsize might be zero here! */
	inframe = (float*) malloc(winsize * sizeof(float));
	if(inframe==NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
																					/* STAGE 4 */
	/* TODO: any other argument processing and setup of variables, 		     
	   output buffer, etc., before creating outfile
	*/
	
	/* create output breakpoint file */
	fp = fopen(argv[ARG_OUTFILE],"w");
	if(fp == NULL){
		printf("envx: unable to create breakpoint file %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	printf("processing....\n");														/* STAGE 5 */
	/* TODO: init any loop-related variables */
	npoints = 0;
	brktime = 0.0;

	while ((framesread = psf_sndReadFloatFrames(ifd,inframe,winsize)) > 0){	
		/* <--------  add buffer processing here ------>  */
		double amp;
		/* find peak sample of this block */
		amp = maxsamp(inframe,framesread);			
		if(fprintf(fp,"%f\t%f\n",brktime,amp) < 0){
			printf("Failed to write to breakpoint file %s\n",argv[ARG_OUTFILE]);
			error++;
			break;
		}
		npoints++;	
		brktime += windur;
	}
	if(framesread < 0)	{
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;		
	}
	else
		printf("Done: %d errors\n",error);
	printf("%d breakpoints written to %s\n",npoints,argv[ARG_OUTFILE]);				/* STAGE 6 */
	/* do all cleanup  */    														/* STAGE 7 */
exit:
	
	if(ifd >= 0)
		if(psf_sndClose(ifd))
			printf("envx: failed to close input file %s\n",argv[ARG_INFILE]);
	if(inframe)
		free(inframe);	
	/*TODO: cleanup any other resources */
	if(fp) {
		if(fclose(fp))
			printf("envx: failed to close output file %s\n", argv[ARG_OUTFILE]);		
	}	
	psf_finish();
	return error;
}
