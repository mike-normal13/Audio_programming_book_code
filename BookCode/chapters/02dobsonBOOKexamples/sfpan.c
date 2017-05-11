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
/* sfpan.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <breakpoints.h>

#define NFRAMES (1024)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE,ARG_BRKFILE,ARG_NARGS};

typedef struct panpos {
	double left;
	double right;
} PANPOS;


PANPOS constpower(double position)
{
	PANPOS pos;
	const double  piovr2    = 4.0 * atan(1.0) * 0.5;	/* pi/2: 1/4 cycle of a sinusoid */
	const double  root2ovr2 = sqrt(2.0) * 0.5;
	double thispos = position * piovr2;				    /* scale position to fit the pi/2 range */
	double angle = thispos * 0.5;					    /* each channel uses a 1/4 of a cycle */

	pos.left    = root2ovr2 * (cos(angle) - sin(angle));
	pos.right	= root2ovr2 * (cos(angle) + sin(angle));
	return pos;
}

int main(int argc, char* argv[])
{
	PSF_PROPS inprops, outprops;									  /* STAGE 1 */
	long framesread;
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	unsigned long nframes = NFRAMES;
	float* inframe = NULL;
	float* outframe = NULL;

	PANPOS pos;
	FILE* fp = NULL;
	unsigned long size;	
	double	sampletime,timeincr;
	BREAKPOINT *points = NULL;

	printf("SFPAN: pan mono sound into stereo\n");		 /* STAGE 2 */
	/* no optioanl flags used */
	/* check rest of commandline */
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
				"usage:\n\t"
				"sfgain infile outfile posfile.brk\n"
				"\tposfile.brk  is breakpoint file with values in range  -1.0 <= pos <= 1.0\n"
				" where -1.0 = full Left, 0 = Centre, +1.0 = full Right"
				);
		return 1;
	}
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	
	ifd = psf_sndOpen(argv[ARG_INFILE],&inprops,0);										/* STAGE 3 */									  
	if(ifd < 0){
		printf("Error: unable to open infile %s\n",argv[ARG_INFILE]);
		error++;
		goto exit;
	}

	if(inprops.chans != 1){
		printf("Error: infile must be mono.\n");
		goto exit;
	}
	
	/* allocate space for sample frame buffer */									   
	inframe = (float*) malloc(nframes * inprops.chans * sizeof(float));
	if(inframe==NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
	/* check file extension of outfile name, so we use correct output file format*/
	outformat = psf_getFormatExt(argv[2]);
	if(outformat == PSF_FMT_UNKNOWN){
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",argv[2]);
		error++;
		goto exit;
	}
	inprops.format = outformat;
	outprops = inprops;
																				  /* STAGE 4 */
	/* read breakpoint file and verify it */
	fp = fopen(argv[ARG_BRKFILE],"r");
	if(fp == NULL){
		printf("Error: unable to open breakpoint file %s\n",argv[ARG_BRKFILE]);
		error++;
		goto exit;
	}
	size = 0;
	points = get_breakpoints(fp, &size);
	if(points==NULL){
		printf("No breakpoints read.\n");
		error++;
		goto exit;
	}
	if(size < 2){
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

	if(!inrange(points,-1,1.0,size)){
		printf("Error in breakpoint file: values out of range -1 to +1 \n");
		error++;
		goto exit;
	}

	/* we need a stereo outfile */	
	outprops.chans = 2;

	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
																			
	/* create stereo output buffer */
	outframe = (float *) malloc(nframes * outprops.chans * sizeof(float));
	if(outframe == NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}

	ofd = psf_sndCreate(argv[ARG_OUTFILE],&outprops,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("Error: unable to create outfile %s\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	
	printf("processing....\n");											/* STAGE 5 */				

	/* init time position counter for reading envelope */
	timeincr = 1.0 / inprops.srate;
	sampletime = 0.0;
	
	while ((framesread = psf_sndReadFloatFrames(ifd,inframe,nframes)) > 0){
		int i,out_i;
		double stereopos;
		
		for(i=0, out_i = 0; i < framesread; i++){			
			stereopos = val_at_brktime(points, size, sampletime);
			pos = constpower(stereopos);
			outframe[out_i++] = (float)(inframe[i] * pos.left);
			outframe[out_i++] = (float)(inframe[i] * pos.right);
			sampletime += timeincr;
		}

		if(psf_sndWriteFloatFrames(ofd,outframe,framesread) != framesread){
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
		
	/* report PEAK values to user */								/* STAGE 6 */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
			for(i=0;i < outprops.chans;i++){
				peaktime = (double) peaks[i].pos / (double) inprops.srate;
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
			}
	}	
	/* do all cleanup  */    									   /* STAGE 7 */
exit:
	if(ifd >= 0)
		psf_sndClose(ifd);
	if(ofd >= 0)
		psf_sndClose(ofd);
	if(inframe)
		free(inframe);
	if(peaks)
		free(peaks);
	if(points)
		free(points);

	if(outframe)
		free(outframe);

	psf_finish();
	return error;



}