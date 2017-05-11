/* sine.c
   simple program demonstrating how a
   digital sine wave can be synthesized,
   from "Introduction to Digital Signals"

   (c) V Lazzarini, 2007
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>
#define N 4410

void usage_and_exit();

int main(int argc, char **argv) {

  SNDFILE *fp;                                      /* soundfile handle */
  SF_INFO   sfinfo;                            /* soundfile format data */
  int i;                                          /* sample block index */  
  int n, cnt=0;                                         /* sample index */
  short sig[N];		                      /* 16-bit (2-byte) signal */
  float amp,fc,fm,ndx, sr = 44100.f;                   /* signal params */
  float phase = 0.75; 		      /* phase, in fractions of a cycle */
  double tpisr  = 8*atan(-1.)/sr; 		 /* 2*PI/sr, a constant */
  double pha = 0.0;		                   /* phase in radians */ 
  
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; /* WAVE, 16-bit PCM */ 
  sfinfo.samplerate  = sr;
  sfinfo.channels = 1;
 
  if(argc != 6) usage_and_exit();

  amp = atof(argv[2]);
  fc = atof(argv[3]);
  fm = atof(argv[4]);
  ndx = atof(argv[5]);  

  fp = sf_open(argv[1], SFM_WRITE, &sfinfo);/* open the soundfile */
  for(i = 0; i < 44100/N; i++){                     /* file-writing loop */
    for(n=0; n < N; n++){              /* the signal generation loop */
        sig[n]=(short)(amp*cos(pha));
        pha += tpisr*(fc+ndx*fm*sin(tpisr*cnt*fm));
        cnt++;     
      }
    sf_write_short(fp, sig, N);         /* write the N-sample data block */
  }
  sf_close(fp);                                        /* close the file */
}

void usage_and_exit(){
  printf("usage: fmsynth outfile.wav amp fc fm index\n"
         "where amp: amplitude (0-32767)\n"
         "fc: carrier frequency (Hz)\n"
         "fm: mod frequency (Hz)\n"
         "index: index of modulation\n"
	 );
  exit(1);
}
