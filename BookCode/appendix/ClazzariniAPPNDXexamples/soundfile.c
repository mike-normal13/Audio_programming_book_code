#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include "wave.h"  /* header file containing the wavehead struc 
                      and the declaration of update_header() */

int main(int argc, char** argv){

FILE   *fpout;           /* output file pointer */
short  *audioblock;      /* audio memory pointer */
int     end, i, j;       /* dur in frames, counter vars */
int     sr = 44100;      /* sampling rate */
int     blockframes = 256; /* audio block size in frames */
int     databytes ;  /* audio data in bytes */
unsigned int ndx = 0;   /* phase index for synthesis */
float   dur, freq;      /* duration, frequency */
double  twopi;          /* 2*PI  */
wavehead *header;

if(argc != 4) {
printf("usage: %s outfile dur freq \n", argv[0]);
exit(-1);
}

dur = atof(argv[2]);
freq = atof(argv[3]);
twopi = 8*atan(1.);
end = (int)(dur*sr);
fpout = fopen(argv[1], "wb");
audioblock = (short *) malloc(sizeof(short)*blockframes);

/* set the data size */
databytes = end*sizeof(short);

/* write the header */
header = (wavehead *) malloc(sizeof(wavehead));
update_header(header, sr, 1, 16, databytes);
fwrite(header, 1, sizeof(wavehead), fpout); 

for(i=0; i < end; i+=blockframes){
  for(j=0; j < blockframes; j++, ndx++){
     audioblock[j] = 16000*sin(ndx*twopi*freq/sr);
    }
  fwrite(audioblock,sizeof(short), blockframes, fpout);
}

free(audioblock);
free(header);
fclose(fpout);
return 0;
}

void 
 update_header(wavehead* header, int sr, int channels, 
				int precision, int databytes){
 header->magic = (*(long *)RIFF_ID);
 header->len0 = databytes + sizeof(wavehead) - 8; 
 header->magic1 = (*(long *)WAVE_ID);
 header->magic2 = (*(long *)FMT_ID);
 header->len = 16;
 header->format = 1;
 header->nchns = (short) channels;	
 header->rate =  sr;	
 header->aver =  sr*channels*precision/8;
 header->nBlockAlign = (short) (channels*precision/8);	
 header->bits = (short) precision;
 header->magic3 =   (*( long *)DATA_ID);
 header->datasize = databytes;
}
