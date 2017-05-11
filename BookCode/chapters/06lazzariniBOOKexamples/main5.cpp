/////////////////////////////////////////////////////
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <snd_defs.h>

/** convolution program.
    
    Convolution of an input file and an impulse
	
    \n
    conv infile.* impulse.* outfile.wav gain

*/
int main(int argc, char** argv) {

  SNDFILE *psfo, *psfi[2];
  float *buffer, gain;
  int chans,i,j=0,cnt=0,pt=0,size;
  float sr, *imp, *del;
  
  if(argc == 5){


    // allocate buffer, delay & table memory  
    buffer = new float[def_vsize];
    gain = atof(argv[4]);

    // now we open the files 
    if(!(psfi[0] = soundin_open(argv[1], &chans, &sr))){
      printf("error opening input file\n");
      exit(-1);
    }

    if(chans > 1 || sr != def_sr) {
      printf("unsupported channels or sr\n");
      exit(-1);
    }

     if(!(psfi[1] = soundin_open(argv[2], &chans, &sr))){
      printf("error opening impulse file\n");
      exit(-1);
    }

    if(chans > 1 || sr != def_sr) {
      printf("unsupported channels or sr\n");
      exit(-1);
    }

    if(!(psfo = soundout_open(argv[3]))){
      printf("error opening output file\n");
      exit(-1);
    }
  
     size = dataframes(psfi[1]);
     del = new float[size];
     imp = new float[size];
     memset(del, 0, sizeof(float)*size);

     // copy impulse into table  
       
     do {
       cnt = soundin(psfi[1],buffer);
       for(i=0; i < cnt; i++, j++) imp[j] = buffer[i];
     } while(cnt);
     
     
    printf("Processing (this might take a while)... \n");
     // process
    j=0;
    do {
      cnt = soundin(psfi[0], buffer);
      fir(buffer,imp,del,size,&pt);
      for(i=0; i < def_vsize; i++) buffer[i] *= gain;
      soundout(psfo,buffer);
      memset(buffer,0,sizeof(float)*def_vsize);
      if(j%(def_vsize*50)==0) {
	fprintf(stdout, "%.3f secs..\n", j/def_sr); 
      }
      j+=def_vsize; 
    } while(cnt); 
        
    // tail (last N-1 samples)
    cnt = size - 1;
    do {
      fir(buffer,imp,del,size,&pt);
      for(i=0;i < def_vsize; i++) buffer[i] *= gain;
      soundout(psfo,buffer);
      memset(buffer,0,sizeof(float)*def_vsize);
      cnt -= def_vsize;
    }	while(cnt >= 0);

    // close file & free memory
    soundin_close(psfi[0]);
    soundin_close(psfi[1]);
    soundout_close(psfo);    
    delete[] buffer;
    delete[] imp;
    delete[] del;

    return 0;
  }
  else {
    printf("usage: conv infile.* impulse.* outfile.wav gain  \n");
    return 1;
  }
}
