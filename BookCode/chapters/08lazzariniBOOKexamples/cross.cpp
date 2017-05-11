/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// cross-synthesis function
//////////////////////////////////////////////

#include "spec.h"

void crosspec(float* maginput, float *phasinput, float *output, 
  int fftsize){

int i;
float mag, phi;

// take care of real-valued points at 0Hz and Nyquist
output[0] = maginput[0];
output[1] = maginput[1];	

for(i=2; i< fftsize; i+=2){

   // get the magnitudes of one input
   mag = (float) sqrt(maginput[i]*maginput[i]+maginput[i+1]*maginput[i+1]);
   // get the phases of the other
   phi = (float) atan2(phasinput[i+1], phasinput[i]);
   // combine them and convert to rectangular form
   output[i] = (float) (mag*cos(phi));
   output[i+1]= (float) (mag*sin(phi));

}

}

