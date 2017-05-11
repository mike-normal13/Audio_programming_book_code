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

/** cross-synthesis main program

\b usage: cross infile1 infile2 outfile dur(s) \n
\n
infile1: input1 (magnitudes) filename \n
infile2: input2 (phases) filename \n
outfile: output filename \n
\n
all files are supposed to be mono \n
some examples can be found in the audio directory  

*/

int
main(int argc, char **argv){
  SNDFILE *fin, *fin2, *fout;
  SF_INFO input_info, input_info2;
  float *win, *in, *in2, *out, *spec, *spec2;
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

  if(!(fin2 = sf_open(argv[2], SFM_READ, &input_info2))){
    printf("could not open %s \n", argv[2]);
    exit(-1);
  }

  if(!formats_equal(input_info, input_info2)){
    sf_close(fin2);
    sf_close(fin);
    printf("%s and %s formats are not equal or files not mono\n",
	   argv[1], argv[2]);
    exit(-1);
  }
  dur = input_info.frames > input_info2.frames ? 
    input_info2.frames : input_info.frames;
  buff = new float[100];
  win = new float[fftsize];
  in = new float[dur];
  in2 = new float[dur];
  /* number of full dataframes */
  frames = ((dur*dataratio)/fftsize);
  /* extra frames [not completely filled] */
  extraframes = 1 + 
    (frames*fftsize - dur*dataratio)/fftsize;
  /* size of spectral data */
  specsize = (frames + extraframes)*fftsize;
  spec = new float[specsize];
  spec2 = new float[specsize];
  outsize = specsize/dataratio;
  out = new float[outsize];

  if(!(fout = sf_open(argv[3], SFM_WRITE, &input_info))){
    printf("could not open %s \n", argv[3]);
    exit(-1);
  }

  for(i=0; i< fftsize; i++) win[i] = 0.5f - (float)(0.5*cos(i*twopi/fftsize));
  
  for(j=0; j < dur; j+=read){
    read = sf_read_float(fin, buff, 100);
    for(i=0; i < read; i++) in[i+j] = buff[i];
  }

  for(j=0; j < dur; j+=read){
    read = sf_read_float(fin2, buff, 100);
    for(i=0; i < read; i++) in2[i+j] = buff[i];
  }
 
  
  outsize = stft(in, win, spec, dur, fftsize, hopsize);

  stft(in2, win, spec2, dur, fftsize, hopsize);

  for(i=0; i < outsize; i += fftsize) 
    crosspec(&spec[i], &spec2[i], &spec[i], fftsize);

 
  dur = istft(spec, win, out, 
  	      outsize,  fftsize, hopsize);
   
  for(j=0; j < dur;j+=written){
    for(i=0; i < 100 && j < dur; i++){ 
      if(i+j < dur) buff[i] = out[i+j];
      else buff[i] = 0.f;
    }
    written = sf_write_float(fout, buff, i);
  }
   
  delete[] out;
  delete[] in;
  delete[] in2;
  delete[] spec;
  delete[] spec2;
  delete[] win;
  delete[] buff;

  sf_close(fout);
  sf_close(fin);
  sf_close(fin2);
  return 0;
} 
	

void
usage(){

  puts("\n\n   usage: cross input1 input2 output \n");

}
