///////////////////////////////////////////////////// 
// delays
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

#include <snd_defs.h>

float delay(float *sig, float dtime, 
	    float *del, int *p, int vecsize, float sr){
  int dt;
  float out;
  dt = (int) (dtime*sr);
  for(int i=0; i < vecsize; i++){
    out = del[*p];
    del[*p] = sig[i];
    sig[i] = out;
    *p = (*p != dt-1 ? *p+1 : 0);
  }
  return *sig;
}

float comb(float *sig, float dtime, float gain,
	   float *delay, int *p, int vecsize, float sr){
  int dt;
  float out;
  dt = (int) (dtime*sr);
  for(int i=0; i < vecsize; i++){
    out = delay[*p];
    delay[*p] = sig[i] + out*gain;
    sig[i] = out;
    *p = (*p != dt-1 ? *p+1 : 0);
  }
  return *sig;
}

float allpass(float *sig, float dtime, float gain,
	      float *delay, int *p, int vecsize, float sr){
  int dt;
  float out;
  dt = (int) (dtime*sr);
  for(int i=0; i < vecsize; i++){
    out = delay[*p];
    delay[*p] = sig[i] + out*gain;
    sig[i] = out  - gain*sig[i];
    *p = (*p != dt-1 ? *p+1 : 0);
  }
  return *sig;
}


float vdelay(float *sig, float vdtime, float maxdel,
	     float *delay, int *p, int vecsize, float sr){
  int mdt,rpi;
  float out, rp, vdt, frac, next;
  vdt = vdtime*sr;
  mdt = (int) (maxdel*sr);
  if(vdt > mdt) vdt = (float) mdt;
  for(int i=0; i < vecsize; i++){
    rp = *p - vdt;
    rp = (rp >= 0 ? (rp < mdt ? rp : rp - mdt) : rp + mdt);
    rpi = (int) rp;  
    frac = rp - rpi;
    next = (rpi != mdt-1 ? delay[rpi+1] : delay[0]);
    out =  delay[rpi] + frac*(next - delay[rpi]);  
    delay[*p] = sig[i];
    sig[i] = out;
    *p = (*p != mdt-1 ? *p+1 : 0);
  }
  return *sig;
}

float flanger(float *sig, float vdtime, float fdb, float maxdel,
	      float *delay, int *p, int vecsize, float sr){
  int mdt, rpi;
  float out, rp, vdt, frac,next;
  vdt = vdtime*sr;
  mdt = (int) (maxdel*sr);
  if(vdt > mdt) vdt = (float) mdt;
  for(int i=0; i < vecsize; i++){
    rp = *p - vdt;
    rp = (rp >= 0 ? (rp < mdt ? rp : rp - mdt) : rp + mdt);
    rpi = (int) rp;  
    frac = rp - rpi;
    next = (rpi != mdt-1 ? delay[rpi+1] : delay[0]);
    out =  delay[rpi] + frac*(next - delay[rpi]);
    delay[*p] = sig[i] + out*fdb;
    sig[i] = out;
    *p = (*p != mdt-1 ? *p+1 : 0);
  }
  return *sig;
}

float fir(float *sig, float *imp, float *del, 
	  int length, int *p, int vecsize, float sr){
   
  float out=0.f; int rp;
  for(int i=0; i < vecsize; i++){
    del[*p] = sig[i];
    *p = (*p != length-1 ? *p+1 : 0);
    for(int j=0; j < length; j++){
      rp = *p+j;
      rp = (rp < length ? rp : rp - length);  
      out += (del[rp]*imp[length-1-j]);
    }
    sig[i] = out;
    out = 0.f;
  }
  return *sig;
}

