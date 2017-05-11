/////////////////////////////////////////////////////
// 
// filters 
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

#include <snd_defs.h>

float lowpass(float* sig, float freq, float *del, int vecsize, float sr){

        double costh, coef;
        costh = 2. - cos(2*pi*freq/sr);
        coef = sqrt(costh*costh - 1.) - costh;
        
		for(int i =0; i < vecsize; i++){
		   sig[i] = (float) (sig[i]*(1 + coef) - *del*coef);
           *del = sig[i];
        }

        return *sig;
}


float highpass(float* sig, float freq, float *del, int vecsize, float sr){

        double costh, coef;
        costh = 2. - cos(2*pi*freq/sr);
        coef = costh - sqrt(costh*costh - 1.);
        
		for(int i =0; i < vecsize; i++){
		   sig[i] = (float) (sig[i]*(1 - coef) - *del*coef);
           *del = sig[i];
        }

        return *sig;
}

float resonator(float* sig, float freq, float bw, float *del, int vecsize,
				float sr){
         
	  double r, rsq, rr, costh, a;
      rr = 2*(r = 1. - pi*(bw/sr));
	  rsq = r*r;
	  costh = (rr/(1.+rsq))*cos(2*pi*freq/sr);
      a = (1 - rsq)*sin(acos(costh));

	  for(int i=0; i < vecsize; i++){
      sig[i] = (float)(sig[i]*a + rr*costh*del[0] - rsq*del[1]);
      del[1] = del[0];
	  del[0] = sig[i];
	  }

      return *sig;
}

float bandpass(float* sig, float freq, float bw, float *del, int vecsize,
				float sr){
         
	  double r, rsq, rr, costh, a, w;
      rr = 2*(r = 1. - pi*(bw/sr));
	  rsq = r*r;
	  costh = (rr/(1.+rsq))*cos(2*pi*freq/sr);
      a = (1 - r);

	  for(int i=0; i < vecsize; i++){
	  w = a*sig[i] + rr*costh*del[0] - rsq*del[1];
      sig[i] = (float)(w - r*del[1]);
      del[1] = del[0];
	  del[0] = (float) w;
	  }

      return *sig;
}

float  balance(float *sig, float *cmp, float* del, float freq, 
			   int vecsize, float sr){

        double costh, coef;
        costh = 2. - cos(2*pi*freq/sr);
        coef = sqrt(costh*costh - 1.) - costh;

	   for(int i=0; i < vecsize; i++){
		del[0] =  (float)((sig[i] < 0 ? -sig[i] : sig[i])*(1+coef) 
								- del[0]*coef);
		del[1] =  (float)((cmp[i] < 0 ? -cmp[i] : cmp[i])*(1+coef) 
								- del[1]*coef);
		sig[i] *= (float)(del[0] ? del[1]/del[0] : del[1]);
	   }

	   return *sig;
}
