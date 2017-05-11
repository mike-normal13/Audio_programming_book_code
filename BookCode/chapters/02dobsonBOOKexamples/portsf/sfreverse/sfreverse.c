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

/* Reverse a soundfile using portsf, the slowest way possible!*/
/* note: no dither applied here, as we are not changing the data */


#include <stdio.h>
#include <stdlib.h>
#include "portsf.h"


int read_frame_and_reverse(float *frame,int sfd);


int main(int argc,char **argv)
{

	int ifd = -1,ofd=-1,rc=0;
	MYLONG pos, size;
	float *framebuf = NULL;
	PSF_PROPS props;


	if(argc < 3){
		printf("Usage: sfreverse infile outfile\n");
		exit(1);
	}

	if(psf_init()){
		printf("unable to start portsf\n");
		return 1;
	}
	ifd  = psf_sndOpen(argv[1],&props,0);
	if(ifd < 0){
		printf("Unable to open infile %s\n",argv[1]);
		exit(1);
	}
	
	ofd = psf_sndCreate(argv[2],&props,0,0,PSF_CREATE_RDWR);
	if(ofd<0){
		printf("Unable to open outfile %s\n",argv[2]);
		goto finish;
	}

	framebuf = (float *) malloc(props.chans * sizeof(float));
	if(framebuf==NULL){
		puts("no memory for frame buffer\n");
		goto finish;
	}

	size = psf_sndSize(ifd);
	if(size < 0)
		printf("cannot find file size\n");
	else if (size==0){
		printf("File %s is empty\n",argv[1]);
		goto finish;
	}
	printf("File size = %ld frames\n",size);
	/* seek to last sample frame */
	rc = psf_sndSeek(ifd,-1,PSF_SEEK_END);
	if(rc){
		printf("error seeking to last frame\n");
		goto finish;
	}
	
	pos = psf_sndTell(ifd);
	printf("starting at frame %ld\n",pos);
	printf("processing...\n");

	if(read_frame_and_reverse(framebuf,ifd)){
		printf("Error reading initial frame\n");
		goto finish;
	}
	while(size-- >= 0) {							
		if(psf_sndWriteFloatFrames(ofd,framebuf,1)<1){
			printf("error writing frame\n");
			goto finish;
		}
		if(size % 100000 == 0)
			printf("%ld\r",size);
		if(read_frame_and_reverse(framebuf,ifd) <0){
			/* we read the first sample, but failed to reverse; write final sample and break */
			if(psf_sndWriteFloatFrames(ofd,framebuf,1)<1){
				printf("error writing frame\n");
				goto finish;
			}
			break;
		}
	} 

	printf("\ndone\n");
finish:
	if(framebuf)
		free(framebuf);
	if(ifd >=0)
		psf_sndClose(ifd);
	if(ofd >=0)
		psf_sndClose(ofd);
	psf_finish();
	return 0;

}

int read_frame_and_reverse(float *frame,int sfd)
{
	long got;

	got = psf_sndReadFloatFrames(sfd,frame,1);
	if(got==0)
		return 1;	/* at EOF */
	if(got <0) {
		printf("got = %ld\n",got);
		return 1;		   /* this would be a true error */
	}
	/* step back two frames! */
	if(psf_sndSeek(sfd,-2,PSF_SEEK_CUR)) {
		/* we have reached frame 1, having just read it, so can't step back 2! */
		printf("reached reversed EOF!\n");	
		return -1;
	}
	return 0;
}






