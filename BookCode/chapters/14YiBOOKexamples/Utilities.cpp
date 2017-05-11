#ifndef UTILITIES
#define UTILITIES

#include <cmath>

using namespace std;

/** Given a pch notation string, return value in base 10 */
int getBaseTen(float pch) {
	int octave = (int)pch;
	float remainder = pch - octave;
	
	int baseTen = octave * 12;
	
	if(remainder < .5) {
		baseTen += (int)floor(remainder * 100);
	} else {
		baseTen += (int)ceil(remainder * 100);
	}
		
	return baseTen;
}

float getPch(float baseTen) {
    float octave  = ((int)baseTen / 12);
    float pitch   = (baseTen - (octave * 12)) / 100.0f;

    return octave + pitch;
}


/* Given a pch, add interval to that pch */
float pchAdd(float pch, float addValue) {
    float baseTen = getBaseTen(pch);
    baseTen += addValue;

    return getPch(baseTen);
}

/** Returns the number of steps between two pch's */
float pchDiff(float pch1, float pch2) {

    int baseTen1 = getBaseTen(pch1);
    int baseTen2 = getBaseTen(pch2);

    int diff = baseTen2 - baseTen1;
    
    return diff;
}

#endif
