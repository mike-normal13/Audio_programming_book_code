/////////////////////////////////////////////////////
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <snd_defs.h>

/** flanging program with envelopes.
    
    Flanges an input sound with a delay time envelope.
	
    \n
    delay infile.* outfile.wav maxdelay(s) env_dur(s)

*/
int main(int argc, char** argv) {

  SNDFILE *psfo, *psfi;
  float *buffer;
  int chans, bytes = sizeof(float)*def_vsize, cnt=0, pt=0, 
    ts=0;
  
  float sr, dur, dtime, *comp, *del, del1[2]={0.f, 0.f};
  
  if(argc == 5){
    dtime  = (float) atof(argv[3]);
    dur = (float) atof(argv[4]);

    // allocate buffer, delay & table memory  
    buffer = new float[def_vsize];
    comp = new float[def_vsize];
    del = new float[(int)(dtime*def_sr)];
    memset(del, 0, sizeof(float)*(int)(dtime*def_sr));
    
    // now we open the files 
    if(!(psfi = soundin_open(argv[1], &chans, &sr))){
      printf("error opening output file\n");
      exit(-1);
    }

    if(chans > 1 || sr != def_sr) {
      printf("unsupported channels or sr\n");
      exit(-1);
    }

    if(!(psfo = soundout_open(argv[2]))){
      printf("error opening output file\n");
      exit(-1);
    }
  

    do {
      cnt = soundin(psfi, buffer);
      memcpy(comp, buffer, bytes);
      flanger(buffer, line(.0001f, dur, dtime, &ts), 0.8f,
	      dtime,  del, &pt);
      balance(buffer, comp, del1);
      soundout(psfo,buffer, cnt);
    } while(cnt); 
 
    // close file & free memory
    soundin_close(psfi);
    soundout_close(psfo);    
    delete[] buffer;
    delete[] comp;
    delete[] del;

    return 0;
  }
  else {
    printf("usage: delay infile.* outfile.wav maxdelay(s) env_dur(s) \n");
    return 1;
  }
}
