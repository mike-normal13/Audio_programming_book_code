/* aiff2wave.c:
   libsndfile example
   converts an AIFF into a WAVE file
*/
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

#define BUFFRAMES 512

void usage();

int main(int argc, char** argv) {

  SNDFILE *psf_in, *psf_out;
  SF_INFO *psfinfo_in, *psfinfo_out;
  float *buffer;
  sf_count_t count;
  int chans;
  
  if(argc < 3) {
    usage();
    exit(-1);
  }

  /* memory for SF_INFO structures */
  psfinfo_in = (SF_INFO *) malloc(sizeof(SF_INFO)); 
  psfinfo_out = (SF_INFO *) malloc(sizeof(SF_INFO)); 

  /* open input  */
  if(!(psf_in = sf_open(argv[1],SFM_READ,psfinfo_in))){
   printf("error opening input file\n");
   exit(-1);
  }
  /* check to see if format is AIFF */
  if((psfinfo_in->format & SF_FORMAT_TYPEMASK) == SF_FORMAT_AIFF){
 
    /* allocate buffer memory  */
  buffer = (float *) malloc(sizeof(float)*BUFFRAMES);
    
  /* Now we initialise the SF_INFO structure 
     with the same sampling rate... */
  psfinfo_out->samplerate = psfinfo_in->samplerate;
  /* ... same number of channels... */
  psfinfo_out->channels = chans =  psfinfo_in->channels;
  /* and WAV format with the same precision */ 
  psfinfo_out->format = SF_FORMAT_WAV | (psfinfo_in->format &                
                                          SF_FORMAT_SUBMASK);
  /* now we open the file */
   if(!(psf_out = sf_open(argv[2], SFM_WRITE,psfinfo_out))){
   printf("error opening output file\n");
   exit(-1);
  }
   /* and we copy the data, frame by frame */
  do {
    count = sf_readf_float(psf_in, buffer, BUFFRAMES/chans);
    sf_writef_float(psf_out, buffer, count);
  }
  while(count);

    sf_close(psf_in);
    sf_close(psf_out);
    free(psfinfo_in);
    free(psfinfo_out);    
    free(buffer);
    return 0;
  }
  else {
    printf("Not an AIFF file\n");
    sf_close(psf_in);
    return 1;
  }
}

void usage(){
  printf("\nusage: aiff2wave input.aif output.wav\n");
}
