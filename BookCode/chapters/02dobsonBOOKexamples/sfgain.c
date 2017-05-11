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
/* sfgain.c :  Change amplitude of soundfile */
#include <stdio.h>
#include <stdlib.h>
#include <portsf.h>

int main(int argc, char* argv[])
{
	PSF_PROPS props;									  /* STAGE 1 */
	long framesread;
	long totalread;
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;
	float* frame = NULL;
	psf_format outformat =  PSF_FMT_UNKNOWN;
	int i;
	float  ampfac;
	

	printf("SFGAIN: change amplitude of soundfile\n");		 /* STAGE 2 */

	if(argc < 4){
		printf("insufficient arguments.\n"
				"usage:\n\t"
				"sfgain infile outfile ampfac\n"
				"\twhere ampfac > 0.0\n");
		return 1;
	}
	/* be good, and startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	
	ampfac = (float) atof(argv[3]);
	if(ampfac <= 0.0){
		printf("Error: ampfac must be positive.\n");
		return 1;
	}


	ifd = psf_sndOpen(argv[1],&props,0);					/* STAGE 3 */													  
	if(ifd < 0){
		printf("Error: unable to open infile %s\n",argv[1]);
		return 1;
	}
	/* we now have a resource, so we use goto hereafter on hitting any error */
	
	/* check file extension of outfile name, so we use correct output file format*/
	outformat = psf_getFormatExt(argv[2]);
	if(outformat == PSF_FMT_UNKNOWN){
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",argv[2]);
		error++;
		goto exit;
	}
	props.format = outformat;

	/* allocate space for one sample frame */
	frame = (float*) malloc(props.chans * sizeof(float));
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

	ofd = psf_sndCreate(argv[2],&props,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("Error: unable to create outfile %s\n",argv[2]);
		error++;
		goto exit;
	}
	
	printf("processing....\n");										/* STAGE 4 */
	
	/* single-frame loop to do copy: report any read/write errors */
	framesread = psf_sndReadFloatFrames(ifd,frame,1);
	totalread = 0;		/* count sample frames as they are copied */
	while (framesread == 1){
		totalread++;

		for(i=0; i < props.chans; i++ )			
			frame[i] *= ampfac;

		if(psf_sndWriteFloatFrames(ofd,frame,1) != 1){
			printf("Error writing to outfile\n");
			error++;
			break;
		}			
		framesread = psf_sndReadFloatFrames(ifd,frame,1);
	}
	if(framesread < 0)	{
		printf("Error reading infile. Outfile is incomplete.\n");
		error++;
	}
	else
		printf("Done.\n");
		
	/* report PEAK values to user */								/* STAGE 5 */
	if(psf_sndReadPeaks(ofd,peaks,NULL) > 0){
		long i;
		double peaktime;
		printf("PEAK information:\n");	
			for(i=0;i < props.chans;i++){
				peaktime = (double) peaks[i].pos / (double) props.srate;
				printf("CH %d:\t%.4f at %.4f secs\n", i+1, peaks[i].val, peaktime);
			}
	}	
	/* do all cleanup  */    									   /* STAGE 6 */
exit:	 	if(ifd >= 0)
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
