#include <stdio.h>
#include <math.h>

#define SAMPLING_RATE 44100
#define NUM_SECONDS 3
#define NUM_SAMPLES (NUM_SECONDS * SAMPLING_RATE)
#define PI  3.14159265
#define FREQUENCY 440

int main()
{
    int j; 
	for(j = 0; j < NUM_SAMPLES; j++) {
		float sample;
		sample = sin(2 * PI * FREQUENCY * j / SAMPLING_RATE);
		printf("%f\n", sample);
	}
	return 0;
}


