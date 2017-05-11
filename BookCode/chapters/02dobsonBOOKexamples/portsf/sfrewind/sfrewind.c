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

/* test/demo of portsf read->seekback->overwite mode */
/* this is not something it is recommended to do; 
   but there will always be that one situation...
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "portsf.h"

/* uses sine lookup table */
/* using truncated lookup; so need long table for reasonable quality */
#define TABLEN (16384)


#define BUFLEN (32768)			/* = 16384 + 2048 */
#ifndef M_PI 
#define M_PI (3.141592654)
#endif

#define PEAKAMP  (0.75)

void gensine(double sintable[], long tabsize)
{   	
	double step;
	int i;	
	step = (2.0 * M_PI)/(double)tabsize;
	for(i=0; i<tabsize; i++)
		sintable[i] = sin(step * (double)i);
		
}

int
main(int argc, char **argv)
{
	int ofd,rc = 0;
	int length,i;
	long srate = 44100;
	PSF_PROPS props;
	double dlength;
	long tablen = TABLEN;
	char *name = NULL;
	double *sinbuf = NULL;
	double dur,incr,frqstep,thissamp;
	double frq1, frq2,curfrq, d_index;
	double ramp = 1.0,rampincr = 1.0 / (double) BUFLEN;
	double dtablen, ampfac, ampfac1, ampincr;
	long tabindex;
	float outsamp;
	float *scalebuf;
	long got,put,pos;
	long wholebufs, remainder;
	
	if(argc<3){
		printf("sfrewind. usage: sfrewind dur outfile\n");
		printf("        dur: basic length of outfile (secs)\n");
		exit(1);
	}
	dur = atof(argv[1]);
	if(dur <=0.0){
		printf("silly duration\n");
		exit(1);
	}
	length = (int)( dur * (double) srate);
	dlength = (double) length;
	sinbuf = (double *) malloc(tablen  * sizeof(double));
	if(sinbuf==NULL){
		puts("not enough memory for sine table\n");
		exit(1);
	}
	gensine(sinbuf,tablen);
	scalebuf = (float *) malloc(BUFLEN * sizeof(float));
	if(scalebuf==NULL){
		puts("no memory for main buffer\n");
		exit(1);
	}
	
	name = argv[2];
	
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	props.srate =44100;
	props.chans = 1;
	props.samptype = PSF_SAMP_16; /* so we can test dither too */
	props.format = psf_getFormatExt(name);
	props.chformat = STDWAVE;

	ofd = psf_sndCreate(name,&props,1,0, PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("failed to create file %s\n",name);
		rc = 1;
		goto finish;
	}    
    psf_sndSetDither(ofd,1);
    
	/*** STAGE 1: generate very long chirp with envelope ***/
	
    
    frq1 = 50.0;
	frq2 = 5000.0;
	frqstep = (frq2 - frq1) / dlength;
	thissamp = 0.0;
	outsamp = 0.0f;
	tabindex= 0;
	d_index = 0.0;
	curfrq = frq1;

	wholebufs = length / BUFLEN;
	remainder = length - (wholebufs * BUFLEN);
	ampincr = 1.0 / (double) length;
	ampfac1 = 0;
    dtablen = (double) tablen;
    /* the eagle-eyed will spot a useful maths optimisation somewhere near here... */
	for(i=0; i < wholebufs; i++){
		int j;
		for(j=0; j < BUFLEN; j ++){
			ampfac = 1.0 - ramp;
			scalebuf[j] = (float) ((sinbuf[tabindex] * ampfac * PEAKAMP * ampfac1));
			curfrq += frqstep;
			incr = (curfrq * dtablen) / (double) srate;
			d_index += (long) incr;
			if(d_index >= dtablen)
				d_index -= dtablen;
			tabindex = (long) d_index;
			if(tabindex < 0)
				tabindex = 0;
			if(ampfac>=1.0)	
				rampincr = 0.0;
			ramp -= rampincr;
			ampfac1 += ampincr;
		}
		if(psf_sndWriteFloatFrames(ofd,scalebuf,BUFLEN) < BUFLEN){
			fprintf(stderr,"error writing data\n");
			rc = 1;
			goto finish;
		}
		pos = psf_sndTell(ofd);
		fprintf(stderr,"POS = %ld\n",pos);
	}
	/* simple but distinct remainder*/
	printf("creating end ramp with %ld samples\n",remainder);
	rampincr = 1.0 / (double)remainder;
	ramp = 0.0;
	for(i = 0; i < remainder; i++){
		ampfac = 1.0 - ramp;
		scalebuf[i] = (float) ((sinbuf[tabindex] * ampfac * PEAKAMP * ampfac1));
		curfrq += frqstep;
		incr = (curfrq * (double)tablen) / (double) srate;
		d_index += (long) incr;
		if(d_index >= (double)tablen)
			d_index -= (double)tablen;
		tabindex = (long) d_index;
		if(tabindex < 0)
			tabindex = 0;	
		ramp += rampincr;
		ampfac1 += ampincr;
	}
	if(psf_sndWriteFloatFrames(ofd,scalebuf,remainder) <remainder){
		fprintf(stderr,"error writing data\n");
		rc = 1;
		goto finish;
	}

	pos = psf_sndSize(ofd);
	fprintf(stderr,"created %ld samples\n",pos);	
    
    /*  STAGE 2; rewind part of the way, read and overwrite */
    
	/* sets overwrite mode, marks the file as 'existing',
		thus effectively fixing its size at where we are now.
	*/
	if(psf_sndSeek(ofd,-pos/3,PSF_SEEK_CUR)){		
		fprintf(stderr,"error rewinding file\n");
		rc = 1;
		goto finish;
	}		

	pos = psf_sndTell(ofd);
	printf("rescaling from %ld\n",pos);
	/* maximally inefficient routine:
        read one sample; step back, write new sample
        so we shuffle thru the data one sample at a time.
    */
	got = psf_sndReadFloatFrames(ofd,scalebuf,1);
	if(got != 1){
		printf("error reading 1 sample\n");
		rc = 1;
		goto finish;
	}
	
	while(got==1){
		if(psf_sndSeek(ofd,-1,PSF_SEEK_CUR)){
			fprintf(stderr,"\nerror seeking back 1 sample");
			goto finish;
		}
		/* obvious change: reduce amplitiude! */
		scalebuf[0] *= 0.25f;
		if((put = psf_sndWriteFloatFrames(ofd,scalebuf,1)) != 1){
			fprintf(stderr,"\nerror writing data: read %ld samps, written %ld samps",
				got,put);
			rc = 1;
			break;
		}		
		got = psf_sndReadFloatFrames(ofd,scalebuf,1);			
	}
	if(rc==0){
		/* add extra block: to verify further writes are allowed */
		for(i=0;i < BUFLEN;i++)
			scalebuf[i] = 0.0f;
		if((put = psf_sndWriteFloatFrames(ofd,scalebuf,BUFLEN)) != BUFLEN){
			fprintf(stderr,"error writing last block\n");	
		}
	}
	printf("done\n");
	pos = psf_sndTell(ofd);
	printf("final pos = %ld\n",pos);
finish:			
	if(ofd >=0)
		if(psf_sndClose(ofd) < 0)
			printf("\nerror closing sndfile");
	if(sinbuf)			
		free(sinbuf);
	psf_finish();
	return rc;
}
