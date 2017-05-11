/////////////////////////////////////////////////////
// 
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <snd_defs.h>

/** subtractive synthesis program with envelopes.
    
	Generates a filtered sawtooth sound with amplitude 
	and filter cf envelopes. This program also shows the use of 
	interpolating oscillators.\n
	\n
	filter sndfile.wav amp freq(Hz) dur(secs)

*/
int main(int argc, char** argv) {

  SNDFILE *psf;
  float *buffer;
  int smps, bytes = sizeof(float)*def_vsize, cnt1=0, cnt2=0;
  
  float dur, amp, freq, *wave, *comp, ndx=0;
  
  if(argc == 5){
  amp   = (float) atof(argv[2]);
  freq  = (float) atof(argv[3]);
  dur =   (float)atof(argv[4]);
  smps   = (int) (dur*def_cr);
 
  // allocate buffer, delay & table memory  
  buffer = new float[def_vsize];
  comp = new float[def_vsize]; 
  float del[2]={0.f, 0.f}, del1[2]={0.f,0.f};
  wave   = saw_table(30);
    
  // now we open the file 
   if(!(psf = soundout_open(argv[1]))){
   printf("error opening output file\n");
   exit(-1);
  }
  
   for(int i=0; i < smps; i++){
    oscc(buffer, 
		adsr(amp,dur,0.05f, 0.1f, amp*0.7f, 0.2f, &cnt1),
		freq,
		wave,&ndx);	
	memcpy(comp, buffer, bytes);
	resonator(buffer, expon(freq*8,dur,freq*4, &cnt2), 50, del);
	balance(buffer, comp, del1);
    soundout(psf,buffer);
  }
 
  // close file & free memory
    soundout_close(psf);    
    delete[] buffer;
	delete[] comp;
	delete[] wave;

    return 0;
  }
  else {
    printf("usage: filter sndfile.wav amp freq(hz) dur(s)\n");
    return 1;
  }
}
