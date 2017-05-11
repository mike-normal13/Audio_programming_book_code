/////////////////////////////////////////////////////
// 
// oscillators  
//
// (c) V Lazzarini, 2005
//////////////////////////////////////////////////////


float osc(float *output, float amp, float freq,  
		float *table, float *index, 
		int length, int vecsize, float sr)
{  
	 
	   // increment       
	   float incr = freq*length/sr;

	   // processing loop 
	   for(int i=0; i < vecsize; i++){
       
		// truncated lookup
		output[i] = amp*table[(int)(*index)];
       
		*index += incr;      
        while(*index >= length) *index -= length;
	    while(*index < 0) *index += length;

	   }


	   return *output;
	 
}  

float osci(float *output, float amp, float freq,  
		float *table, float *index, float phase,
		int length, int vecsize, float sr)
{  
	 
	   // increment       
	   float incr = freq*length/sr, frac, pos,a ,b;
	   phase = phase < 0 ? 1+phase : phase;
	   int offset = (int)(phase*length)%length;

	   // processing loop 
	   for(int i=0; i < vecsize; i++){
       pos = *index + offset;
	   
	   // linear interpolation
	   frac = pos  - (int)pos;
	   a = table[(int)pos]; 
	   b = table[(int)pos+1]; 
	   output[i] = amp*(a + frac*(b - a));
	   
       *index += incr;
       while(*index >= length) *index -= length;
	   while(*index < 0) *index += length;
	   }


	   return *output;
	 
}  

float oscc(float *output, float amp, float freq,  
		float *table, float *index, float phase,
		int length, int vecsize, float sr)
{  
	 
	   // increment       
	   float incr = freq*length/sr, frac, fracsq, fracb;
	   float pos,a,b,c,d, tmp;
	   phase = phase < 0 ? 1+phase : phase;
	   int offset = (int)(phase*length)%length;

	   // processing loop 
	   for(int i=0; i < vecsize; i++){

       pos = *index + offset; 
	   
	    // cubic interpolation   
	   frac = pos  - (int)pos;    
	   a = (int) pos > 0 ? table[(int)pos-1] : table[length-1];
	   b = table[(int)pos];
	   c = table[(int)pos+1];
       d = table[(int)pos+2];

	   tmp = d + 3.f*b;
	   fracsq = frac*frac;
	   fracb = frac*fracsq;

	   output[i] = amp*(fracb*(- a - 3.f*c + tmp)/6.f +
                        fracsq*((a+c)/2.f - b) +
						frac*(c + (-2.f*a - tmp)/6.f) + b);

       *index += incr;

	   while(*index >= length) *index -= (length);
	   while(*index < 0) *index += length;

	   }


	   return *output;
	 
}  
