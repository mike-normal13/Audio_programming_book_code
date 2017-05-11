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
#include <string.h>
#include <portsf.h>
#include <breakpoints.h>
#include <time.h>
#include <wave.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)

/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_OUTFILE,ARG_TYPE,ARG_DUR,ARG_SRATE, ARG_AMP,ARG_FREQ,ARG_NARGS};

enum { WAVE_SINE = 0,WAVE_TRIANGLE,WAVE_SQUARE,WAVE_SAWUP,WAVE_SAWDOWN,WAVE_NTYPES};


int main(int argc, char* argv[])
{
	PSF_PROPS outprops;									/* STAGE 1 */
	/* init all dynamic resources to default states */
	int ofd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;	
	psf_format outformat =  PSF_FMT_UNKNOWN;
	long nframes = NFRAMES;
	float* outframe = NULL;
	double amp,freq,dur;
	unsigned long outframes,nbufs,i;
	long remainder;
	int wavetype;
	OSCIL* osc = NULL;
	tickfunc tick;
	FILE* fpfreq = NULL;
	FILE* fpamp = NULL;
	BRKSTREAM*  freqstream = NULL;
	BRKSTREAM*  ampstream = NULL;
	unsigned long brkfreqSize = 0,brkampSize = 0;
	double minval,maxval;
	
	printf("SIGGEN: generate simple waveforms\n");								/* STAGE 2 */

	/* check rest of commandline */
	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
			/* TODO: add required usage message */
			"usage: siggen outfile wavetype dur srate amp freq\n"
			"where wavetype =:\n"
			"                0 = sine\n"
			"                1 = triangle\n"
			"                2 = square\n"
			"                3 = sawtooth up\n"
			"                4 = sawtooth down\n"
			"dur   = duration of outfile (seconds)\n"
			"srate = required sample rate of outfile\n"
			"amp   = amplitude value or breakpoint file (0 < amp <= 1.0)\n"
			"freq  = frequency value (freq > 0) or breakpoint file.\n"
			);
		return 1;
	}																			 /* no Stage 3! */
																				/* STAGE 4 */
	/* TODO: any other argument processing and setup of variables, 		     
	   output buffer, etc., before creating outfile
	*/

	wavetype = atoi(argv[ARG_TYPE]);
	if(wavetype < WAVE_SINE || wavetype > WAVE_NTYPES){
		printf("Error: bad value for wave type\n");
		return 1;
	}

	switch(wavetype){
	case(WAVE_SINE):
		tick = sinetick;
		break;
	case(WAVE_TRIANGLE):
		tick = tritick;
		break;
	case(WAVE_SQUARE):
		tick = sqtick;
		break;
	case(WAVE_SAWUP):
		tick = sawutick;
		break;
	case(WAVE_SAWDOWN):
		tick = sawdtick;
		break;		
	}
	
	/* define outfile format */
	outprops.srate = atoi(argv[ARG_SRATE]);
	if(outprops.srate <=0){
		printf("error: srate must be positive\n");
		return 1;
	}
	outprops.chans = 1;
	outprops.samptype = (psf_stype) PSF_SAMP_16;
	outprops.chformat = STDWAVE;

	dur =  atof(argv[ARG_DUR]);
	if(dur <= 0.0){
		printf("Error: amp must be positive\n");
		return 1;
	}
	outframes = (unsigned long) (dur * outprops.srate + 0.5);
	nbufs =  outframes / nframes;	
	remainder = outframes - nbufs * nframes;
	if(remainder)
		nbufs++;

	/* open breakpoint files, or set constants */
	fpamp = fopen(argv[ARG_AMP],"r");
	if(fpamp == NULL){
		amp = atof(argv[ARG_AMP]);
		if(amp <= 0.0 || amp > 1.0){
			printf("Error: amplitude value out of range: 0.0 < amp <= 1.0\n");
			error++;
			goto exit;
		}
	}
	else {
		ampstream = bps_newstream(fpamp,outprops.srate,&brkampSize);
		if(ampstream == NULL){
			printf("Error reading amplitude breakpoint file %s\n",argv[ARG_AMP]);
			error++;
			goto exit;
		}
		if(bps_getminmax(ampstream,&minval,&maxval)){
			printf("Error reading range of breakpoint file %s\n",argv[ARG_AMP]);
			error++;
			goto exit;
		}
		if(minval < 0.0 || minval > 1.0 || maxval < 0.0 || maxval > 1.0){
			printf("Error: amplitude values out of range in file %s: 0.0 < amp <= 1.0\n", argv[ARG_AMP]);
			error++;
			goto exit;
		}
	}
	fpfreq = fopen(argv[ARG_FREQ],"r");
	if(fpfreq== NULL){
		freq = atof(argv[ARG_FREQ]);
		if(freq <= 0.0){
			printf("Error: frequency must be positive\n");
			error++;
			goto exit;
		}
	}
	else {
		freqstream = bps_newstream(fpfreq,outprops.srate,&brkfreqSize);
		if(freqstream == NULL){
			printf("Error reading freq breakpoint file %s\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		if(bps_getminmax(freqstream,&minval,&maxval)){
			printf("Error reading range of breakpoint file %s\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		if(minval <= 0.0 ){		 /* maxval = don't care! */
			printf("Error in file %s:  frequency values must be positive\n", argv[ARG_AMP]);
			error++;
			goto exit;
		}
	}
	
	osc =  new_oscil(outprops.srate);
	if(osc == NULL){
		puts("no memory for oscillator\n");
		error++;
		goto exit;
	}
	
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		error++;
		goto exit;
	}
																							
	/* allocate space for  sample frame buffer ...*/
	outframe = (float*) malloc(nframes * outprops.chans * sizeof(float));
	if(outframe==NULL){
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
	outprops.format = outformat;
														                      
	/* handle outfile */
	/* TODO:  make any changes to outprops here  */

	peaks  =  (PSF_CHPEAK*) malloc(outprops.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
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

	printf("processing....\n");									/* STAGE 5 */
	for(i=0;i < nbufs;i++){
		long j;
		if(i == nbufs-1)
			nframes = remainder;

		for(j=0;j < nframes; j++){
			if(freqstream)
				freq = bps_tick(freqstream);
			if(ampstream)
				amp = bps_tick(ampstream);			
			outframe[j] = (float) (amp *  tick(osc,freq));
		}
		if(psf_sndWriteFloatFrames(ofd,outframe,nframes) != nframes){
			printf("Error writing to outfile\n");
			error++;
			break;
		}
	}

	printf("Done: %d errors\n",error);

	/* report PEAK values to user */												/* STAGE 6 */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
			for(i=0;i < outprops.chans;i++){
				peaktime = (double) peaks[i].pos / (double) outprops.srate;
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
			}
	}	
	/* do all cleanup  */    														/* STAGE 7 */
exit:	 		
	if(ofd >= 0)
		if(psf_sndClose(ofd))
			printf("Error closing outfile %s\n",argv[ARG_OUTFILE]);
	if(outframe)
		free(outframe);
	if(peaks)
		free(peaks);
	/*TODO: cleanup any other resources */
	if(osc)
		free(osc);
	if(fpfreq)
		if(fclose(fpfreq))
			printf("Error closing breakpoint file %s\n",argv[ARG_FREQ]);
	if(fpamp)
		if(fclose(fpamp))
			printf("Error closing breakpoint file %s\n",argv[ARG_AMP]);
	if(freqstream){
		bps_freepoints(freqstream);
		free(freqstream);
	}
	if(ampstream){
		bps_freepoints(ampstream);
		free(ampstream);
	}
	psf_finish();
	return error;
}
