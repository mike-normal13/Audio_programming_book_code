/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////

//////////////////////////////////////////////
// simple lowpass and highpass functions
//////////////////////////////////////////////

#include "spec.h"

void simplp(float *input, float *output, int fftsize){

int i,k;
float mag, magin, phi;
double pi = twopi/2.;

// The low-pass contour is 1 at 0Hz
// and 0 at the Nyquist
output[0] = 1.f;
output[1] = 0.f;	

for(i=2, k=1; i < fftsize; i+=2, k++){

   // get the magnitudes of input
   magin = (float) sqrt(input[i]*input[i]+input[i+1]*input[i+1]);
   // apply the spectral contour
   mag = (float) cos((pi*k)/fftsize)*magin;
   // get the phases
   phi = (float) atan2(input[i+1], input[i]);
   // convert to rectangular form
   output[i] = (float) (mag*cos(phi));
   output[i+1]= (float) (mag*sin(phi));

}

}

void simphp(float *input, float *output, int fftsize){

int i,k;
float mag, magin, phi;
double pi = twopi/2.;

// The low-pass contour is 1 at 0Hz
// and 0 at the Nyquist
output[0] = 1.f;
output[1] = 0.f;	

for(i=2, k=1; i < fftsize; i+=2, k++){

   // get the magnitudes of input
   magin = (float) sqrt(input[i]*input[i]+input[i+1]*input[i+1]);
   // apply the spectral contour
   mag = (float) sin((pi*k)/fftsize)*magin;
   // get the phases
   phi = (float) atan2(input[i+1], input[i]);
   // convert to rectangular form
   output[i] = (float) (mag*cos(phi));
   output[i+1]= (float) (mag*sin(phi));

}

}
