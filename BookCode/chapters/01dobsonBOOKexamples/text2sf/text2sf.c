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

/* text2sf.c :  convert text soundfile to binary using portsf library */

#include <stdio.h>
#include <stdlib.h>
#include <portsf.h>


enum {ARG_PROGNAME,ARG_INFILE,ARG_OUTFILE, ARG_SR,ARG_CHANS,ARG_GAIN,ARG_NARGS};

/* read one multi-channel frame */

int readframe(FILE* fp,float* frame,int chans){
    int i;
    int samps=0, got;
    for(i=0;i < chans;i++){
        got = fscanf(fp,"%f",&frame[i]);
        if(got==EOF)
            return -1;
        samps += got;
    }
    if(got == chans)
        return 1;
    else 
       return 0; /* error! */
}

int main(int argc, char* argv[])
{
	PSF_PROPS props;
	long framesread;
	long totalread;
	/* init all dynamic resources to default states */
	int ifd = -1,ofd = -1;
	int i,error = 0;
    double gain = 1.0;
	PSF_CHPEAK* peaks = NULL;
	psf_format outformat =  PSF_FMT_UNKNOWN;
    FILE* fin = NULL;
    float* frame;

	printf("TEXT2SF: convert text audio data to soundfile\n");

	if(argc < ARG_NARGS){
		printf("insufficient arguments.\n"
				"usage:\n\t"
				"text2sf infile outfile srate chans gain\n");
		return 1;
	}
	/* be good, and startup portsf */
	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	 
	fin = fopen(argv[ARG_INFILE],"r");
	/* we now have a resource, so we use goto hereafter on hitting any error */
	/* tell user if source file is already floats  */
    if(fin==NULL){
        printf("cannot open infile %s\n",argv[ARG_INFILE]);
        return 1;
    }

    props.chans = atoi(argv[ARG_CHANS]);
    if(props.chans <=0){
        printf("chans must be positive!\n");
        fclose(fin);
        return 1;
    }
    props.srate = atoi(argv[ARG_SR]);
    if(props.srate <=0){
        printf("srate must be positive!\n");
        fclose(fin);
        return 1;
    }
    gain = atof(argv[ARG_GAIN]);
    if(gain <= 0.0){
        printf("gain value must be positive!\n");
        fclose(fin);
        return 1;
    }
    frame = malloc(props.chans * sizeof(float));
    if(frame==NULL){
        puts("No memoery!\n");
        fclose(fin);
        return 1;
    }
    /* fixed output to 16bit file; one alternative would be PSF_SAM_IEEE_FLOAT for 32bit float format */
    props.samptype = PSF_SAMP_16;
    props.format = PSF_STDWAVE;
    props.chformat = STDWAVE;
	/* check file extension of outfile name, so we use correct output file format*/
	outformat = psf_getFormatExt(argv[ARG_OUTFILE]);
	if(outformat == PSF_FMT_UNKNOWN){
		printf("outfile name %s has unknown format.\n"
			"Use any of .wav, .aiff, .aif, .afc, .aifc\n",argv[ARG_OUTFILE]);
		error++;
		goto exit;
	}
	props.format = outformat;

	ofd = psf_sndCreate(argv[ARG_OUTFILE],&props,0,0,PSF_CREATE_RDWR);
	if(ofd < 0){
		printf("Error: unable to create outfile %s\n",argv[ARG_OUTFILE]);
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
	printf("copying....\n");
	
	/* single-frame loop to do copy: report any read/write errors */
	framesread = readframe(fin,frame,props.chans);
	totalread = 0;		/* count sample frames as they are copied */
    /* note: readframe retval of -1 signifies EOF */
	while (framesread == 1){
		totalread++;
		/*	<--- do any processing here! ---------->*/
        for(i=0; i < props.chans; i++)
            frame[i] *= gain;
		if(psf_sndWriteFloatFrames(ofd,frame,1) != 1){
			printf("Error writing to outfile\n");
			error++;
			break;
		}			
		framesread = readframe(fin,frame,props.chans);
	}
	if(framesread == 0)	{
		printf("Error reading infile at line %d. Outfile may be incomplete.\n",totalread);
		error++;
	}
	else
		printf("Done. %d sample frames copied to %s\n",totalread,argv[ARG_OUTFILE]);
		
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
    if(fin != NULL)
		fclose(fin);
	if(ofd >= 0)
		psf_sndClose(ofd);
	if(frame)
		free(frame);
	if(peaks)
		free(peaks);
	psf_finish();
    /* return error status to caller: may be useful in a script etc */
	return error;
}
