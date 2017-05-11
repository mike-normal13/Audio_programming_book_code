/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <spec.h>
#include <sndfile.h>


/** simple DFT demonstration program
    dft filename
*/
int
main(int argc, char *argv[]){
  SNDFILE *fin;
  FILE *fout;
  SF_INFO input_info;
  float *sig, *spec, buf[100], mag, sr;
  int size, i, k, read;

  if(!(fin = sf_open(argv[1], SFM_READ, &input_info))){
    printf("could not open %s\n", argv[1]);
    exit(-1);
  }
  
  size = input_info.frames;
  sr = input_info.samplerate;
  sig = new float[size];
  spec = new float[size*2];
  read = sf_read_float(fin,sig,size);
      
  DFT(sig, spec, size);

  fout = fopen("spec.txt","w");
  fprintf(fout,"%f\n", sr); 
  for(i=0, k=0; i<size; i++, k+=2) {
    if(i<size/2){
      mag = sqrt(spec[k]*spec[k] + spec[k+1]*spec[k+1])*2;
      printf("[%.1f] %1.2f\n", i*sr/size, mag);
      fprintf(fout,"%f\n", mag); 
    }
  }

  sf_close(fin);
  fclose(fout);
  delete[] sig;
  delete[] spec;
  return 0;	
}
