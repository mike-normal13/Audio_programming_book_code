#ifndef FOURIER_H
#define FOURIER_H

#include "rfftw.h"
#include <sndfile.h>

/** real-signal fft function (FFTW interface)
   
    in: input array of N samples (time-domain) \n
	out: externally-allocated array of N points, which will \n
	receive the DFT output (positive spectrum only, with \n
	re{0Hz} and re{Nyquist} in out[0] and out[1], respectively) \n
	N: transform size \n

*/

void fft(float *in, float *out, int N);
/** real-signal inverse fft function (FFTW interface)
  
	in: input array of N/2 spectral points, 
	the positive spectrum only, with \n
	re{0Hz} and re{Nyquist} in out[0] and out[1], respectively) \n
	out: externally-allocated array of N samples (time-domain) \n
	which will hold the output signal \n
	N: transform size \n

*/
void ifft(float *in, float *out, int N);

/** format test
 */
int formats_equal(SF_INFO a, SF_INFO b);
#endif
