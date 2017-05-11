/////////////////////////////////////////////
// Spectral processing examples
//
// (c) V Lazzarini, 2005
//
//  see License.txt for copyright information
////////////////////////////////////////////////

#include "spec.h"

/** DFT takes an input signal *in of size N
   outputs spectrum *out with N pairs of
   complex values [real, imag]
*/

void DFT(float *in, float *out, int N){
for(int i=0,k=0; k<N; i+=2, k++){
       out[i]=out[i+1]=0.f;
       for(int n =0; n < N; n++){
    out[i] += (float)(in[n]*cos((k*n*twopi)/N));
    out[i+1] -= (float)(in[n]*sin((k*n*twopi)/N));
  }
       out[i] /= N;
       out[i+1] /= N;
     }
}
