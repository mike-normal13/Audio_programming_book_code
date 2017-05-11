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

/** convolution main program

\b usage: convolution impulse infile outfile dur(s) \n
\n
impulse: impulse response filename \n
infile: input filename \n
outfile: output filename \n

all files are supposed to be mono files of the same format \n
some examples can be found in the audio directory  

*/

int
main(int argc, char **argv){

  SNDFILE *fimpulse, *fin, *fout;
  SF_INFO impulse_info, input_info;
  float *impulse, *in, *out; 
  int read=0, written = 0, i, j, dur, isize, osize;
  float *buff;

  if(argc < 4) {
    printf("%s: unsufficient number of" 
	   " arguments(got %d, needed 3)", argv[0], argc-1);
    usage();
    exit(-1);
  }
 
  if(!(fimpulse = sf_open(argv[1], SFM_READ, &impulse_info))){
    printf("could not open %s\n", argv[1]);
    exit(-1);
  }

  if(!(fin = sf_open(argv[2], SFM_READ, &input_info))){
    printf("could not open %s\n", argv[2]);
    exit(-1);
  }
 
  if(!formats_equal(impulse_info, input_info)){
    sf_close(fimpulse);
    sf_close(fin);
    printf("%s and %s formats are not equal or files not mono\n",
            argv[1], argv[2]);
    exit(-1);
  }
   
  dur = input_info.frames;
  isize = impulse_info.frames;
  osize = dur+isize-1;
  buff = new float[100];
  impulse = new float[isize];
  in = new float[dur];
  out = new float[osize];

  if(!(fout = sf_open(argv[3], SFM_WRITE, &input_info))){
    printf("could not open %s\n", argv[3]);
    exit(-1);
  }

  sf_command (fimpulse, SFC_SET_NORM_FLOAT, NULL, SF_FALSE);
  sf_command (fin, SFC_SET_NORM_FLOAT, NULL, SF_FALSE);
  sf_command (fout, SFC_SET_NORM_FLOAT, NULL, SF_FALSE);

 
  for(j=0; j < isize; j+=read){                       
    read = sf_read_float(fimpulse, buff, 100);
     for(i=0; i < read; i++) impulse[i+j] = buff[i];
  }
  
  for(j=0; j < dur; j+=read){
    read = sf_read_float(fin, buff, 100);
     for(i=0; i < read; i++) in[i+j] = buff[i];
  }
  
  convol(impulse, in, out, isize, dur);

  for(j=0; j < osize;j+=written){
    for(i=0; i < 100; i++) 
      if(i+j < osize) buff[i] = out[i+j];
      else buff[i] = 0.f;
    written = sf_write_float(fout, buff, i);
  }

  delete[] out;
  delete[] in;
  delete[] impulse;
  delete[] buff;
  
  sf_close(fout);
  sf_close(fin);
  sf_close(fimpulse);
  
  return 0;
	
}

void
usage(){
  puts("\n\n   usage: convolution impulse"
       " input output \n");
}

