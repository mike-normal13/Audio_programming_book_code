#include <stdio.h>
#include <stdlib.h>
#include <snd_defs.h>

/** Simple synthesis program.

    Generates a sinewave sound. This program shows the use of 
    truncating oscillators.\n
    \n
    oscillator sndfile.wav amp freq(Hz) dur(secs)   
*/
int main(int argc, char** argv) {

  SNDFILE *psf;
  float *buffer;
  int dur;
  
  float amp, freq, *wave, cs, ndx=0, ndx2=0;
  
  if(argc == 5){
    amp   = (float) atof(argv[2]);
    freq  = (float) atof(argv[3]);
    dur   = (int) (atof(argv[4])*def_cr);
 
    // allocate buffer & table memory  
    buffer = new float[def_vsize];
    wave =   sinus_table();
    
    // now we open the file 
    if(!(psf = soundout_open(argv[1]))){
      printf("error opening output file\n");
      exit(-1);
    }
  
    for(int i=0; i < dur; i++){
      osc(buffer,amp,
	  freq+osc(&cs,10.f,5.f,wave,&ndx2,def_len,1, def_cr),
	  wave,&ndx);	
      soundout(psf,buffer);
    }
 
    // close file & free memory
    soundout_close(psf);   
    delete[] buffer;
    delete[] wave;

    return 0;
  }
  else {
    printf("usage: oscillator sndfile.wav amp freq(hz) dur(s)\n");
    return 1;
  }
}
