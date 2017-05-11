/* synth.c
   simple program demonstrating how a
   digital wave can be synthesized using a Fourier series
   from "Introduction to Digital Signals"

   (c) V Lazzarini, 2007
*/

#include <sndfile.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#define N 4410
#define TWOPI 6.283185307

float* sawtooth(int harms, int length);
float* square(int harms, int length);
float* fourier_table(int harms, float *amps, 
                     int length, float phase);
void normalise_table(float *table, int length);
void usage_and_exit();

int main(int argc, char **argv) {

  SNDFILE *fp;                                      /* soundfile handle */
  SF_INFO   sfinfo;                            /* soundfile format data */
  int i;                                          /* sample block index */  
  int n;                                                /* sample index */
  short sig[N];		                      /* 16-bit (2-byte) signal */
  float amp, freq, sr = 44100.f;                       /* signal params */
  double index=0.0, incr; 
  int tablen = 16384;
  float *tab;

  if(argc != 6) usage_and_exit();
  
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; /* WAVE, 16-bit PCM */ 
  sfinfo.samplerate  = sr;
  sfinfo.channels = 1;
  
  fp = sf_open(argv[1],SFM_WRITE,&sfinfo);        /* open the soundfile */

  if(atoi(argv[4])==0){
    tab = sawtooth(atoi(argv[5]), tablen);
    printf("sawtooth\n");
  }
  else tab = square(atoi(argv[5]), tablen);
  freq = atof(argv[3]);
  amp = atof(argv[2]);

  incr = freq*tablen/sr;
  for(i = 0; i < 44100/N; i++){                     /* file-writing loop */
    for(n=0; n < N; n++) {                 /* the signal generation loop */
      sig[n]=(short)(amp*tab[(int)index]);
      index += incr;
      while(index >= tablen) index -= tablen;
      while(index < 0) index += tablen;
    }
    sf_write_short(fp, sig, N);         /* write the N-sample data block */
  }
  sf_close(fp);                                        /* close the file */
  free(tab);
}

/* generate a sawtooth wave */
float* sawtooth(int harms, int length){
  int i;
  float *amps = (float *) malloc(harms*sizeof(float));
  for(i=0; i < harms; i++) amps[i] = 1.0/(i+1.0);
  return fourier_table(harms,amps,length,0.75);
}

/* generate a square wave */
float* square(int harms, int length){
  int i;
  float *amps = (float *) malloc(harms*sizeof(float));
  memset(amps,0,harms*sizeof(float));
  for(i=0; i < harms; i+=2) amps[i] = 1.0/(i+1.0);
  return fourier_table(harms,amps,length,0.75);
}

/* this takes the number of harmonics, an array of harmonic weights, 
   the table length and a phase offset in fractions of a cycle */
float* fourier_table(int harms, float *amps, 
                     int length, float phase){
  int i,n;   				/* harmonic index, sample index */
  float a;			        /* amplitude */
  double w;	                        /* freq in rads of each harmonic */
  float *table = (float *) malloc(length*sizeof(float));
  phase *= (float)TWOPI;  

  memset(table,0,(length)*sizeof(float));

  for(i=0; i < harms; i++)
    for(n=0; n < length; n++){
      a  = amps ?  amps[i] : 1.f;	/* use 1, if amps not supplied */  
      w = (i+1)*(n*TWOPI/length);     /* freq of each harmonic */
      table[n] += (float) (a*cos(w+phase)); /* scale and mix */
    }
  normalise_table(table, length);         /* normalise the table */
  return table;
}

/* this takes a table and normalises it */
void normalise_table(float *table, int length){
  int n;
  float max = 0.f; 
  for(n=0; n < length; n++) /* find the peak value */
    max = table[n] > max ? table[n] : max;
  if(max) for(n=0; n < length; n++) table[n] /= max;
}

void usage_and_exit(){
  printf("usage: synth outfile.wav amp freq wtype harms\n"
         "where amp: amplitude (0-32767)\n"
         "freq: frequency (Hz)\n"
         "wtype: wave type, 0=saw, 1=square\n"
         "harms: highest harmonic number\n"
	 );
  exit(1);
}
