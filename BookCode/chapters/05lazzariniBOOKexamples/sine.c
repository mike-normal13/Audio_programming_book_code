/* sine.c
   simple program demonstrating how a
   digital sine wave can be synthesized,
   from "Introduction to Digital Signals"

   (c) V Lazzarini, 2007
*/

#include <sndfile.h>
#include <math.h>
#define N 4410

int main() {

  SNDFILE *fp;                                      /* soundfile handle */
  SF_INFO   sfinfo;                            /* soundfile format data */
  int i;                                          /* sample block index */  
  int n;                                                /* sample index */
  short sig[N];		                      /* 16-bit (2-byte) signal */
  float amp = 32767.f, freq = 440.f, sr = 44100.f;     /* signal params */
  float phase = 0.75; 		      /* phase, in fractions of a cycle */
  double twopi  = 8*atan(-1.); 			    /* 2*PI, a constant */
  phase *= twopi;		                    /* phase in radians */ 
  
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; /* WAVE, 16-bit PCM */ 
  sfinfo.samplerate  = sr;
  sfinfo.channels = 1;
  
  fp = sf_open("sinetest.wav", SFM_WRITE, &sfinfo);/* open the soundfile */

  for(i = 0; i < 44100/N; i++){                     /* file-writing loop */
    for(n=0; n < N; n++)                   /* the signal generation loop */
        sig[n]=(short)(amp*cos(twopi*(n+i*N)*freq/sr + phase));
    sf_write_short(fp, sig, N);         /* write the N-sample data block */
  }
  sf_close(fp);                                        /* close the file */
}
