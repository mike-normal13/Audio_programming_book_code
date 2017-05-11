/////////////////////////////////////////////////////
// function tables 
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////
#include <snd_defs.h>


float*  line_table(int brkpts, float* pts, int length){
   float start,end,incr,*table = new float[length+2];

   for(int n=2; n < brkpts*2; n+=2){
       start = pts[n-1];
	   end   = pts[n+1];
	   incr = (end - start)*1.f/(pts[n]-pts[n-2]); 
	   for(int i=(int)pts[n-2]; i < pts[n] && i < length+2; i++){
          table[i] = start;
          start += incr;
	   }
          
   }
    normalise_table(table, length);
	return table;
}

float*  exp_table(int brkpts, float* pts, int length){
   float mult,*table = new float[length+2];
   double start, end;

   for(int n=2; n < brkpts*2; n+=2){
       start = pts[n-1] + 0.00000001;
	   end   = pts[n+1] + 0.00000001;
	   mult = (float) pow(end/start,1./(pts[n]-pts[n-2])); 
	   for(int i=(int)pts[n-2]; i < pts[n] && i < length+2; i++){
          table[i] = (float) start;
          start *= mult;
	   }
          
   }
    normalise_table(table, length);
	return table;
}


float* sinus_table(int length, float phase){
   float *table = new float[length+2];
   phase *= (float)pi*2; 
   for(int n=0; n < length+2; n++)
	   table[n] = (float) cos(phase+n*2*pi/length);
   
   return table;

}

float* fourier_table(int harms, float *amps, 
				   int length, float phase){
   float a;
   float *table = new float[length+2];
   double w;
   phase *= (float)pi*2;  
   
   memset(table,0,(length+2)*sizeof(float));

   for(int i=0; i < harms; i++)
	  for(int n=0; n < length+2; n++){
	   a  = amps ?  amps[i] : 1.f;	   
       w = (i+1)*(n*2*pi/length);
	   table[n] += (float) (a*cos(w+phase));
	  }
  
   normalise_table(table, length);
   return table;
}

float* saw_table(int harms, int length){

	float *amps = new float[harms];
	for(int i=0; i < harms; i++) amps[i] = 1.f/(i+1);
    float *table = fourier_table(harms,amps,length, -0.25);
	delete[] amps;
	return table;
}

float*  sqr_table(int harms, int length){
	float *amps = new float[harms];
	memset(amps, 0, sizeof(float)*harms);
	for(int i=0; i < harms; i+=2) amps[i] = 1.f/(i+1);
    float *table = fourier_table(harms,amps,length, -0.25);
	delete[] amps;
	return table;;
}

float* triang_table(int harms, int length){
	float *amps = new float[harms];
	memset(amps, 0, sizeof(float)*harms);
	for(int i=0; i < harms; i+=2) amps[i] = 1.f/((i+1)*(i+1));
    float *table = fourier_table(harms,amps,length);
	delete[] amps;
	return table;;
}

void normalise_table(float *table, int length, 
					             float scale){
    int n;
    float max = 0.f;
	for(n=0; n < length; n++) max = table[n] > max ?
								table[n] : max;
	if(max)
    for(n=0; n < length; n++) table[n] /= max;
}
