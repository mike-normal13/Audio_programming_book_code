/////////////////////////////////////////////////////
// 
// envelopes
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////
#include <snd_defs.h>

float line(float pos1, float dur, float pos2, int *cnt, float cr){
      int durs =  (int)  (dur*cr);
	  if((*cnt)++ < durs)
	  return pos1 + *cnt*(pos2-pos1)/durs;
      else return pos2;
}

float expon(float pos1, float dur, float pos2, int *cnt, float cr){
      int durs =  (int)  (dur*cr);
	  if((*cnt)++ < durs)
	  return (float) (pos1*pow((double)pos2/pos1, (double)*cnt/durs));
      else return pos2;
}

float interp(float pos1, float dur, float pos2, double alpha,
            int *cnt, float cr){
      int durs =  (int)  (dur*cr);
	  if((*cnt)++ < durs)
	  return (float) (pos1 + (pos2-pos1)*pow((double) *cnt/durs, alpha));
      else return pos2;
}


float adsr(float maxamp, float dur, float at, float dt, 
			float sus, float rt, int *cnt, float cr)
		{
    float a;
	// convert to time in samples
	at = at*cr;
	dt = dt*cr;
	rt = rt*cr;
	dur = dur*cr;

	if(*cnt < dur) { // if time < total duration
	// attack period
	if(*cnt <= at) a = *cnt * (maxamp/at);
	// decay period
	else if(*cnt <= (at+dt)) 
		   a = ((sus - maxamp)/dt)*(*cnt - at) + maxamp;
	     // sustain period
	     else if(*cnt <= (dur - rt))
		   a = sus;
		 // release period
		 else if(*cnt > (dur - rt)) 
		   a = -(sus/rt)*(*cnt - (dur - rt)) + sus;
	}
	else a = 0.f;
    
	// update time counter
    (*cnt)++;
	return  a;
		}





