// 
// simple_waveforms
// author: jonathan bailey, (c) 2009 
// see License.txt for license info
//
// a simple set of functions to construct basic geometric waveforms
// digitally.
// 


#ifndef SIMPLE_WAVEFORMS_H
#define SIMPLE_WAVEFORMS_H

#include <math.h>

void create_wave_sine     (float *sine,     int wave_length);
void create_wave_saw      (float *saw,      int wave_length);
void create_wave_triangle (float *triangle, int wave_length);
void create_wave_square   (float *square,   int wave_length);

#endif
