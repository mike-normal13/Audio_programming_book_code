/////////////////////////////////////////////////////
// 
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <snd_defs.h>

/** simple synthesis program with envelopes.
    
    Generates a sawtooth sound with frequency and amplitude 
    envelopes. This program also shows the use of 
    interpolating oscillators.\n
    \n
    envelope sndfile.wav amp freq(Hz) dur(secs)

*/
int main(int argc, char** argv) {

  SNDFILE *psf;
  float *buffer;
  int smps, cnt1=0, cnt2=0;
  
  float dur, amp, freq, *wave,  ndx=0;
  
  if(argc == 5){
    amp   = (float) atof(argv[2]);
    freq  = (float) atof(argv[3]);
    dur =   (float)atof(argv[4]);
    smps   = (int) (dur*def_cr);
 
    // allocate buffer & table memory  
    buffer = new float[def_vsize];
    wave   = saw_table(30);
    
    // now we open the file 
    if(!(psf = soundout_open(argv[1]))){
      printf("error opening output file\n");
      exit(-1);
    }
  
    for(int i=0; i < smps; i++){
      oscc(buffer, 
	   amp*adsr(1.f,dur,0.05f, 0.1f, 0.7f, 0.2f, &cnt1),
	   expon(freq,dur/2,freq*2, &cnt2),
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
    printf("usage: envelope sndfile.wav amp freq(hz) dur(s)\n");
    return 1;
  }
}
