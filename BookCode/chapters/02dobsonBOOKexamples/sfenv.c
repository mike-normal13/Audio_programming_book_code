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
/* main.c :  generic soundfile processing main function*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <breakpoints.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_BRKFILE,ARG_NARGS};

int main(int argc, char* argv[])
{
	PSF_PROPS inprops,outprops;														/* STAGE 1 */
	long framesread;	
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;
	float* outframe = NULL;

	FILE* fp = NULL;
	unsigned long npoints;	
	BREAKPOINT* points = NULL;
	BREAKPOINT leftpoint,rightpoint;
	unsigned long  ileft,iright;
	double curpos,incr,width,height;
	int more_points;

	printf("SFENV: apply envelope to soundfile\n");									/* STAGE 2 */

	/* check rest of commandline */
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: SFENV infile outfile envelope.brk\n"
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
	/* TODO: verify infile format for this application */
	if(inprops.chans != 1){
		printf("Error: only mono files supported\n");
		error++;
		goto exit;
	}
	/* allocate space for  sample frame buffer ...*/
	inframe = (float*) malloc(nframes * inprops.chans * sizeof(float));
	if(inframe==NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
	/* check file extension of outfile name, so we use correct output file format*/
	outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if(outformat == PSF_FMT_UNKNOWN){
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	inprops.format = outformat;
	outprops = inprops;
																					/* STAGE 4 */
	/* TODO: any other argument processing and setup of variables, 		     
	   output buffer, etc., before creating outfile
	*/

	/* read breakpoint file and verify it */
	fp = fopen(argv[ARG_BRKFILE],"r");
	if(fp == NULL){
		printf("Error: unable to open breakpoint file %s\n",argv[ARG_BRKFILE]);
		error++;
		goto exit;
	}
	npoints = 0;
	points = get_breakpoints(fp, &npoints);
	if(points==NULL){
		printf("No breakpoints read.\n");
		error++;
		goto exit;
	}
	if(npoints < 2){
		printf("Error: at least two breakpoints required\n");		
		free(points);
		fclose(fp);
		return 1;
	}	
	/* we require breakpoints to start from 0 */
	if(points[0].time != 0.0){
		printf("Error in breakpoint data: first time must be 0.0\n");		
		error++;
		goto exit;
	}	

	if(!inrange(points,0.0,1.0,npoints)){
		printf("Error in breakpoint file: values out of range -1 to +1 \n");
		error++;
		goto exit;
	}

	/* handle outfile */
	/* TODO:  make any changes to outprops here  */
	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}

	/* TODO: if outchans != inchans, allocate outframe, and modify main loop accordingly */
	ofd = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("Error: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}

	printf("processing....\n");														/* STAGE 5 */

	/* init time position counter for reading envelope */
	incr = 1.0 / inprops.srate;
	/* setup counters to track through the breakpoint data */
	curpos = 0.0;
	ileft = 0;
	iright = 1;
	/* setup first span */
	leftpoint = points[ileft];
	rightpoint = points[iright];
	width = rightpoint.time - leftpoint.time;
	height = rightpoint.value - leftpoint.value;	
	more_points = 1;
	while ((framesread = psf_sndReadFloatFrames(ifd,inframe,nframes)) > 0){
		int i;
		double frac,thisamp;
		
		for(i=0; i < framesread;i++){
			if(more_points){
				if(width == 0.0)
					thisamp =  rightpoint.value;						
				else {
					/* not vertical: get interp value from this span */
					frac = (curpos - leftpoint.time) / width;
					thisamp = leftpoint.value + ( height * frac);
				}					
				/* move up ready for next sample */
				curpos += incr;
				if(curpos > rightpoint.time){			/*  need to go to next span? */
					ileft++; iright++;
					if(iright < npoints){						/* if we have another span, go there */
						leftpoint = points[ileft];
						rightpoint = points[iright];
						width = rightpoint.time - leftpoint.time;
						height = rightpoint.value - leftpoint.value;
					}
					else
						more_points = 0;
				}
			}
			inframe[i] = (float)(inframe[i] * thisamp);
		}
		if(psf_sndWriteFloatFrames(ofd,inframe,framesread) != framesread){
			printf("Error writing to outfile\n");
			error++;
			break;
		}	
	}

	if(framesread < 0)	{
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else
		printf("Done: %d errors\n",error);
		
	/* report PEAK values to user */												/* STAGE 6 */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
			for(i=0;i < inprops.chans;i++){
				peaktime = (double) peaks[i].pos / (double) inprops.srate;
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
			}
	}	
	/* do all cleanup  */    														/* STAGE 7 */
exit:	 	
	if(ifd >= 0)
		psf_sndClose(ifd);
	if(ofd >= 0)
		psf_sndClose(ofd);
	if(inframe)
		free(inframe);
	if(peaks)
		free(peaks);
	/*TODO: cleanup any other resources */
	if(fp)
		fclose(fp);

	psf_finish();
	return error;
}

