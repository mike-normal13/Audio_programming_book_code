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
/* addgen.c :  simple waveform additive synthesis using oscbank */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <portsf.h>
#include <breakpoints.h>
#include <time.h>
#include <wave.h>

/* set size of multi-channel frame-buffer */
#define NFRAMES (1024)
/* TODO define program argument list, excluding flags */
enum {ARG_PROGNAME,ARG_OUTFILE,ARG_DUR,ARG_SRATE,ARG_NCHANS, ARG_AMP,ARG_FREQ,ARG_TYPE,ARG_NOSCS,ARG_NARGS};
enum { WAVE_SQUARE,WAVE_TRIANGLE,WAVE_SAWUP,WAVE_SAWDOWN,WAVE_PULSE};
enum {SAW_DOWN,SAW_UP };
/* TODO: for full waveshape adsyn: make table[noscs] of amp and freq for each osc , according to waveshape*/
int isnum(const char* num,double* val);
OSCIL*   new_oscilp(double srate,double phase);

int isnum(const char* num,double* val)
{
	char *stopchar;
	long len;
	double x;

	if(num==NULL || val == NULL)
		return 0;
	len = strlen(num);
	if(len==0)
		return 0;
	x = strtod(num,&stopchar);
	if(*stopchar != '\0')
		return 0;
	*val = x;
	return 1;
}




OSCIL*   new_oscilp(double srate,double phase)
{
	OSCIL* p_osc;

	p_osc = (OSCIL*) malloc(sizeof(OSCIL));
	if(p_osc==NULL)
		return NULL;
	p_osc->srate = srate;
	p_osc->twopiovrsr = TWOPI / (double)srate;
	p_osc->curfreq = 0.0;
	p_osc->curphase = TWOPI * phase;
	p_osc->incr = 0.0;
	return p_osc;
}


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
	/* TODO: define an output frame buffer if channel width different 	*/
	double amp,freq,dur;
	long outframes,nbufs,noscs,i;
	long remainder;
	int samptype = PSF_SAMP_16;
	OSCIL** oscs = NULL;
	double* oscamps = NULL;
	double* oscfreqs = NULL;
	double phase = 0.0;
	double ampfac,freqfac,ampadjust;
	
	FILE* fpfreq = NULL;
	BRKSTREAM *freqstream = NULL, *ampstream = NULL;
	unsigned long brkfreqSize = 0;
	double freq_minval = 0.0,freq_maxval= 0.0;
	FILE* fpamp = NULL;
	unsigned long brkampSize = 0;
	double amp_minval = 0.0,amp_maxval= 0.0;
	clock_t starttime, endtime;
	int wavetype;
	oscs = NULL;
	printf("OSCGEN: additive synthesis of basic waveforms\n");								/* STAGE 2 */
	
	/* process any optional flags: remove this block if none used! */
	if(argc > 1){
		char flag;
		while(argv[1][0] == '-'){
			flag = argv[1][1];
			switch(flag){
			/*TODO: handle any  flag arguments here */
			case('\0'):
				printf("Error: missing flag name\n");
				return 1;
			case('s'):
				if(argv[1][2] =='\0'){
					printf("Error: -s flag requires parameter\n");
					return 1;
				}
				samptype = atoi(&(argv[1][2]));
				if(samptype <= PSF_SAMP_UNKNOWN || samptype > PSF_SAMP_IEEE_FLOAT){
					printf("Error: -s value out of range\n");
					return 1;
				}
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
			/* TODO: add required usage message */
			"usage: oscgen [-sN] outfile dur srate nchans amp freq wavetype noscs\n"			
		    "  wavetype:     0 = square\n"
		    "                1 = triangle\n"
		    "                2 = saw up\n"
		    "                3 = saw down\n"
			"                4 = pulse\n"
			);
		return 1;
	}
	/* STAGE 4 */
	/* TODO: any other argument processing and setup of variables, 		     
	   output buffer, etc., before creating outfile
	*/

	/* define outfile format */
	outprops.srate = atoi(argv[ARG_SRATE]);
	if(outprops.srate <= 0){
		printf("error: srate must be positive\n");
		return 1;
	}
	outprops.chans = atoi(argv[ARG_NCHANS]);
	if(outprops.chans <= 0){
		printf("error: nchans must be positive\n");
		return 1;
	}
	outprops.samptype = (psf_stype) samptype;
	outprops.chformat = STDWAVE;

	dur =  atof(argv[ARG_DUR]);
	if(dur <= 0.0){
		printf("Error: amp must be positive\n");
		return 1;
	}
	outframes = (unsigned long) (dur * outprops.srate + 0.5);
	nbufs =  outframes / nframes;	 
	remainder = outframes - nbufs * nframes;
	if(remainder > 0)
		nbufs++;
	noscs = atoi(argv[ARG_NOSCS]);
	if(noscs < 1){
		printf("Error: noscs must be positive\n");
		return 1;
	}
	wavetype = atoi(argv[ARG_TYPE]);
	if(wavetype < WAVE_SQUARE || wavetype > WAVE_PULSE) {
		printf("Error: wavetype value out of range\n");
		return 1;
	}
	/*  always startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
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
	/* open brkfiles if any */
	fpfreq = fopen(argv[ARG_FREQ],"r");
	if(fpfreq == NULL){
		if(!isnum(argv[ARG_FREQ],&freq)){
			printf("Error: file %s does not exist\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		else{
			if(freq <= 0.0){
				printf("Error: freq must be positive\n");
				error++;
				goto exit;
			}
		}
	}
	else {
		freqstream = bps_newstream(fpfreq,outprops.srate,&brkfreqSize);
		if(freqstream == NULL) {
			printf("Error reading freq breakpoint file %s\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		if(bps_getminmax(freqstream,&freq_minval,&freq_maxval)) {
			printf("Error reading range of breakpoint file %s\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		if(freq_minval <= 0.0 || freq_maxval <= 0.0) {
			printf("Error: negative frequency values in breakpoint file %s\n",argv[ARG_FREQ]);
			error++;
			goto exit;
		}
		if(freq_minval >= outprops.srate/2  || freq_maxval >= outprops.srate/2){
			printf("Error: frequency values above %ld in breakpoint file %s\n", outprops.srate/2, argv[ARG_FREQ]);
			error++;
			goto exit;
		}
	}

	fpamp = fopen(argv[ARG_AMP],"r");
	if(fpamp== NULL){
		if(!isnum(argv[ARG_AMP],&amp)){
			printf("Error: file %s does not exist\n",argv[ARG_AMP]);
			error++;
			goto exit;
		}
		else{
			if(amp <= 0.0 || amp > 1.0){
				printf("Error: amp value out of range: 0 < amp <= 1.0\n");
				error++;
				goto exit;
			}
		}
	}
	else {
		ampstream = bps_newstream(fpamp,outprops.srate,&brkampSize);
		if(ampstream == NULL){
			printf("Error reading amp breakpoint file %s\n",argv[ARG_AMP]);
			error++;
			goto exit;
		}
		if(bps_getminmax(ampstream,&amp_minval,&amp_maxval)){
			printf("Error reading range of breakpoint file %s\n",argv[ARG_AMP]);
			error++;
			goto exit;
		}
		
		if(amp_minval < 0.0 || amp_minval > 1.0  || amp_maxval < 0.0 || amp_maxval > 1.0){
			printf("Error: amplitude values out of range in breakpoint file %s\n",argv[ARG_AMP]);
			error++;
			goto exit;
		}
	}
	/* create oscillator, and amp and freq arrays */
	oscamps = (double*) malloc(noscs * sizeof(double));
	if(oscamps==NULL){
		puts("no memory!\n");
		error++;
		goto exit;
	}
	oscfreqs = (double*) malloc(noscs * sizeof(double));
	if(oscfreqs==NULL){
		puts("no memory!\n");
		error++;
		goto exit;
	}
	/* create amp/freq data  for requested waveshape, try to normalize so oscs sum to 1 */
	ampfac = 1.0;
	freqfac = 1.0;
	ampadjust  = 0.0;

	switch(wavetype){
	case(WAVE_SQUARE):
		for(i=0;i< noscs;i++){
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 2.0;
			ampadjust += ampfac;
			ampfac = 1.0 /freqfac;

		}		
		for(i=0; i < noscs;i++)
			oscamps[i] /= ampadjust;
		break;
	case(WAVE_TRIANGLE):
		for(i=0;i< noscs;i++){
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 2.0;
			ampadjust += ampfac;
			ampfac = 1.0 /(freqfac*freqfac);			
		}		
		for(i=0; i < noscs;i++)
			oscamps[i] /= ampadjust;
		phase = 0.25;
		break;
	case(WAVE_SAWUP):
	case(WAVE_SAWDOWN):
		for(i=0;i< noscs;i++){
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 1.0;
			ampadjust += ampfac;
			ampfac = 1.0 /(freqfac);
		}
		if(wavetype == WAVE_SAWUP)
			ampadjust = -ampadjust;
		for(i=0; i < noscs;i++)
			oscamps[i] /= ampadjust;
		break;				
	case(WAVE_PULSE):
		freqfac = 1.0;
		ampfac = 1.0 / noscs;
		ampadjust  = 0.0;
		for(i=0;i< noscs;i++){
			oscamps[i] = ampfac;
			oscfreqs[i] = freqfac;
			freqfac += 1.0;
		}
		break;
	}

	oscs = (OSCIL**) malloc(noscs * sizeof(OSCIL *));
	if(oscs== NULL){
		puts("no memory!\n");
		error++;
		goto exit;
	}
	for(i=0;i < noscs;i++){
		oscs[i] =  new_oscilp(outprops.srate,phase);
		if(oscs[i] == NULL){
			puts("no memory for oscillators\n");
			error++;
			goto exit;
		}
	}

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
	starttime = clock();
	/* TODO: init any loop-related variables */

	for(i=0; i < nbufs; i++) {			
		long j,k,l;
		double val;
        if(i == nbufs - 1)
            nframes = remainder;
		
		for(j=0; j < nframes; j++){			
			if(freqstream)
				freq = bps_tick(freqstream);
			if(ampstream)
				amp = bps_tick(ampstream);
			val = 0.0;
			for(l = 0; l < noscs; l++)	 {								
				val += oscamps[l] * sinetick(oscs[l],freq * oscfreqs[l]);
			}
			for(k=0;k < outprops.chans;k++){				
				outframe[j*outprops.chans + k] = (float)(val  * amp);
			}
		}		
		if(psf_sndWriteFloatFrames(ofd,outframe,nframes) != nframes){
			printf("Error writing to outfile\n");
			error++;
			break;
		}
	}
	endtime = clock();
	printf("Elapsed time =  %f secs\n",(endtime-starttime)/(double)CLOCKS_PER_SEC);
	printf("Done: %d errors\n",error);
	/* report PEAK values to user */							/* STAGE 6 */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
		for(i=0;i < outprops.chans;i++){
			peaktime = (double) peaks[i].pos / (double) outprops.srate;
			printf("CH %ld:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
		}
	}	
	/* do all cleanup  */    											/* STAGE 7 */
exit:	 		
	if(ofd >= 0)
		psf_sndClose(ofd);
	if(outframe)
		free(outframe);
	if(peaks)
		free(peaks);

	if(oscs){
		for(i=0;i < noscs;i++){
			free(oscs[i]);
		}
		free(oscs);
	}
	if(oscamps)
		free(oscamps);
	if(oscfreqs)
		free(oscfreqs);
	psf_finish();
	return error;
}

