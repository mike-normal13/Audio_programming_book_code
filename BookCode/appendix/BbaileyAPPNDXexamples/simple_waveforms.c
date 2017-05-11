// 
// simple_waveforms
// author: jonathan bailey, (c) 2009 
// see License.txt for license info
// 

#include "simple_waveforms.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif

// sine wave - the simplest waveform for a simple oscillator
void create_wave_sine(float *sine, int wave_length) {
  for(int i = 0; i < wave_length; i++) {
    sine[i] = sin(2.0*M_PI*i/(float)wave_length);
  } 
}

// sawtooth wave - ramp from 1 to -1 across the period
void create_wave_saw(float *saw, int wave_length) {
  for(int i = 0; i < wave_length; i++) {
    saw[i] = 1-i*2.0/(float)(wave_length);
  }
}

// triangle wave - ramp from -1 to 1 at the middle of the period, 
// and then back down to -1
void create_wave_triangle(float *triangle, int wave_length) {
  float halfway = ((float)wave_length)/2.;
  for(int i = 0; i < wave_length; i++) {
    if(i < halfway) 
      triangle[i] = -1 + i*2.0/(float)(halfway);
    else 
      triangle[i] = 1 - (i-halfway)*2.0/(float)(halfway);
  }
}

// square wave - 1 until the middle of the period, and then back down to -1
void create_wave_square(float *square, int wave_length) {
  float halfway = ((float)wave_length)/2.;
  for(int i = 0; i < wave_length; i++)
    square[i] = (i < halfway) ? 1 : -1;
}
