/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// morphing function
//////////////////////////////////////////////
#include "spec.h"
#include <stdio.h>
#include <float.h>

void pvmorph(float* input1, float *input2, float *output, 
  float morpha, float morphfr, int fftsize, float sr){

int i;
float amp1, amp2, fr1, fr2;
double div;

for(i=0; i< fftsize; i+=2){

amp1 = input1[i];
amp2 = input2[i];
output[i] = amp1 + (amp2-amp1)*morpha;

if(i){
// interpolate frs
fr1 = input1[i+1];
fr2 = input2[i+1];
div = fr1 ? fr2/fr1 : HUGE_VAL;
div = div > 0 ? div : -div;
output[i+1] = (float)(fr1*pow(div, morphfr));

}
else {
// this is the nyquist frequency band
amp1 = input1[i+1];
amp2 = input2[i+1];
output[i+1] = amp1 + (amp2-amp1)*morpha;

}

}

}
