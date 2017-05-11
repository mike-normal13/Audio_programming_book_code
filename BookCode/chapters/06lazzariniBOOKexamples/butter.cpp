/////////////////////////////////////////////////////
// 
// Butterworth filters 
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////

float butterworth(float* sig, float freq, float bw, float *del, int mode, 
				 int vecsize, float sr){
         
	  double a, a1, a2, b1, b2, tanthe, costhe, sqrtan, tansq,w;
      
	  switch(mode) {
	  case LP:
	  tanthe = 1./tan(pi*freq/sr);
	  sqrtan = sqrt(2.)*tanthe;
	  tansq = tanthe*tanthe;
	  a = 1./(1. + sqrtan + tansq);
	  a1= 2.*a;
	  a2 = a;
	  b1 = 2.*(1. - tansq)*a;
	  b2 = (1. - sqrtan + tansq)*a;
	  break;
	  
	  case HP:
	  tanthe = tan(pi*freq/sr);
	  sqrtan = sqrt(2.)*tanthe;
	  tansq = tanthe*tanthe;
	  a = 1./(1. + sqrtan + tansq);
	  a1= -2.*a;
	  a2 = a;
	  b1 = 2.*(tansq - 1.)*a;
	  b2 = (1. - sqrtan + tansq)*a;
	  break;

	  case BR:
	  tanthe = tan(pi*bw/sr);
	  costhe = 2.*cos(2*pi*freq/sr);
	  a = 1./(1. + tanthe);
	  a1= -costhe*a;
	  a2 = a;
	  b1 = -costhe*a;
	  b2 = (1. - tanthe)*a;	
	  break;

	  case BP:
	  default:
	  tanthe = 1./tan(pi*bw/sr);
	  costhe = 2.*cos(2*pi*freq/sr);
	  a = 1./(1. + tanthe);
	  a1= 0;
	  a2 = -a;
	  b1 = -tanthe*costhe*a;
	  b2 = (tanthe - 1.)*a;

	  }

	  for(int i=0; i < vecsize; i++){
	  w = sig[i] - b1*del[0] - b2*del[1];
      sig[i] = (float)(a*w + a1*del[0] + a2*del[1]);
      del[1] = del[0];
	  del[0] = (float) w;
	  }

      return *sig;
}
