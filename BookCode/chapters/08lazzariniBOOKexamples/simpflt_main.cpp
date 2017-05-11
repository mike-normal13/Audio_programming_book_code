/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//
//  see License.txt for copyright information
//////////////////////////////////////////////

#include <stdio.h>
#include "spec.h"
#include "fourier.h"

/** simpflt main program

\b usage: simpflt infile outfile dur(s) \n
\n
infile: input filename \n
outfile: output filename \n

\n
all files are supposed to be mono  \n
some examples can be found in the audio directory  

*/
int
main(int argc, char **argv){
  SNDFILE *fin, *fout;
  SF_INFO input_info;
  float *win, *in, *out, *spec;
  int outsize, specsize, fftsize=1024,hopsize=256;
  int dataratio=fftsize/hopsize;
  int frames, extraframes;
  int read=0, written = 0, i,j, dur;
  float *buff;

  if(argc < 3) {
    printf("%s: unsufficient number of" 
	   " arguments(got %d, needed 2)", argv[0], argc-1);
    usage();
    exit(-1);
  }
  if(!(fin = sf_open(argv[1], SFM_READ, &input_info))){
    printf("could not open %s \n", argv[1]);
    exit(-1);
  }
  dur = input_info.frames;
  buff = new float[100];
  win = new float[fftsize];
  in = new float[dur];
  /* number of full dataframes */
  frames = ((dur*dataratio)/fftsize);
  /* extra frames [not completely filled] */
  extraframes = 1 + 
    (frames*fftsize - dur*dataratio)/fftsize;
  /* size of spectral data */
  specsize = (frames + extraframes)*fftsize;
  spec = new float[specsize];
  outsize = specsize/dataratio;
  out = new float[outsize];

  if(!(fout = sf_open(argv[2], SFM_WRITE, &input_info))){
    printf("could not open %s \n", argv[2]);
    exit(-1);
  }
  for(i=0; i< fftsize; i++) win[i] = 0.5f - (float)(0.5*cos(i*twopi/fftsize));

  for(j=0; j < dur; j+=read){
    read = sf_read_float(fin, buff, 100);
    for(i=0; i < read; i++) in[i+j] = buff[i];
  }

  outsize = stft(in, win, spec, 
		 dur,  fftsize, hopsize);

  for(i=0; i < outsize; i += fftsize) 
    simplp(&spec[i], &spec[i], fftsize);

  dur = istft(spec, win, out, 
	      outsize,  fftsize, hopsize);
   
  for(j=0; j < dur;j+=written){
    for(i=0; i < 100 && j < dur; i++){ 
      if(i+j < dur) buff[i] = out[i+j];
      else buff[i] = 0.f;
    }
    written = sf_write_float(fout, buff, i);
  }
  delete[] spec;   
  delete[] out;
  delete[] in;
  delete[] win;
  delete[] buff;

  sf_close(fout);
  sf_close(fin);

  return 0;
} 
	


void
usage(){

  puts("\n\n   usage: simpflt input output \n");

}
