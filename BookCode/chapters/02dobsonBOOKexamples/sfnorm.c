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
/* sfnorm.c :  normalize soundfile to user-defined dB level */
#include <stdio.h>
#include <stdlib.h>
#include <portsf.h>
#include <math.h>



#define NFRAMES (1024)


double maxsamp(float* buf, unsigned long blocksize);


double maxsamp(float* buf, unsigned long blocksize)
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


int main(int argc, char* argv[])
{
	PSF_PROPS props;
	long framesread;
	long blocksize;
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;
	float* frame = NULL;
	psf_format outformat =  PSF_FMT_UNKNOWN;
	
	int i;
	double dbval, inpeak = 0.0;
	float ampfac, scalefac;
	unsigned long nframes = NFRAMES;


	printf("SFNORM: normalize soundfile to given dB level\n");

	if(argc < 4){
		printf("insufficient arguments.\n"
				"usage:\n\t"
				"sfnorm infile outfile dBval\n"
				"\twhere dBval <= 0.0\n");
		return 1;
	}
	/* be good, and startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	
	dbval = (atof(argv[3]));
	if(dbval > 0.0){
		printf("Error: dBval cannot be positive.\n");
		return 1;
	}
	ampfac  = (float) pow(10.0,dbval/20.0);

	ifd = psf_sndOpen(argv[1],&props,0);																		  
	if(ifd < 0){
		printf("Error: unable to open infile %s\n",argv[1]);
		return 1;
	}
	/* allocate space for  sample frame buffer*/
	frame = (float*) malloc(nframes * props.chans * sizeof(float));
	if(frame==NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}
	/* and allocate space for PEAK info */
	peaks  =  (PSF_CHPEAK*) malloc(props.chans * sizeof(PSF_CHPEAK));
	if(peaks == NULL){
		puts("No memory!\n");
		error++;
		goto exit;
	}

	/* get peak info: scan file if required */
	if(psf_sndReadPeaks(ifd,peaks,NULL) > 0){
		long i;
		printf("Using PEAK data.\n");
		for(i=0;i < props.chans;i++)
			if(peaks[i].val > inpeak)
				inpeak = peaks[i].val;				
	}
	else {
		framesread = psf_sndReadFloatFrames(ifd,frame,nframes);
		printf("No PEAK data found: scanning file...\n");
		while (framesread > 0){
			double thispeak;

			blocksize =  framesread * props.chans;
			thispeak = maxsamp(frame,blocksize);
			if(thispeak > inpeak)
				inpeak = thispeak;				
			framesread = psf_sndReadFloatFrames(ifd,frame,nframes);
		}
		if(framesread < 0)	{
			printf("Error scanning infile.\n");
			error++;
			goto exit;
		}
		/* rewind infile */
		if((psf_sndSeek(ifd, 0, PSF_SEEK_SET)) < 0){
			printf("Error: unable to rewind infile.\n");
			error++;
			goto exit;
		}
	}
	
	/* make sure there is something to normalize! */
	if(inpeak==0.0){
		printf("infile is silent! Outfile not created.\n");
		goto exit;
	}

	printf("infile peak amp = %.4f (%.2f dB)\n",inpeak, 20 * log10(inpeak));
	
	/* check file extension of outfile name, so we use correct output file format*/
	outformat = psf_getFormatExt(argv[2]);
	if(outformat == PSF_FMT_UNKNOWN){
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",argv[2]);
		error++;
		goto exit;
	}
	props.format = outformat;

	ofd = psf_sndCreate(argv[2],&props,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("Error: unable to create outfile %s\n",argv[2]);
		error++;
		goto exit;
	}
	
	/* calculate final scale factor from infile peak and user target level */
	scalefac = (float)(ampfac / inpeak);
	printf("normalizing by %.4f...\n",scalefac);
	
	framesread = psf_sndReadFloatFrames(ifd,frame,nframes);
	while (framesread > 0){
		blocksize =  framesread * props.chans;
				
		for(i = 0; i < blocksize; i++)			
			frame[i] *= scalefac;

		if(psf_sndWriteFloatFrames(ofd,frame,framesread) != framesread){
			printf("Error writing to outfile\n");
			error++;
			break;
		}			
		framesread = psf_sndReadFloatFrames(ifd,frame,nframes);
	}
	if(framesread < 0)	{
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else
		printf("Done.\n");
		
	/* report PEAK values to user */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
			for(i=0;i < props.chans;i++){
				peaktime = (double) peaks[i].pos / (double) props.srate;
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
			}
	}	
	/* do all cleanup  */    
exit:	 	
	if(ifd >= 0)
		psf_sndClose(ifd);
	if(ofd >= 0)
		psf_sndClose(ofd);
	if(frame)
		free(frame);
	if(peaks)
		free(peaks);
	psf_finish();
	return error;
}






