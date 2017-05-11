/* cpsmidi.c: Listing 1.4 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{	
	double c5,c0,semitoneratio;	
	double frequency;
	/*int*/double midinote;				/* could be a char */

	semitoneratio = pow(2,1.0/12);
	c5 = 220.0 * pow(semitoneratio,3);	 
	c0 = c5 * pow(0.5,5);	            

	/* if the program is not called cpsmidi, either change the lines below,
		or use the argv[0] system */
	if(argc != 2){
		printf("cpsmidi :  converts MIDI note to frequency.\n");
		printf("usage: cpsmidi MIDInote\n");
		printf(" range: 0 <= MIDInote <= 127 \n");
		return 1;
	}

	midinote =  atof(argv[1]);
	/* use argv[1] to echo a bad argument string to the user */
	if(midinote < 0.0){
		printf("Bad MIDI note value: %s\n",argv[1]);
		return 1;
	}
	if(midinote > 127.0){
		printf("%s is beyond the MIDI range!\n",argv[1]);
		return 1;
	}	
	
	frequency = c0 * pow(semitoneratio,midinote);
	printf("frequency of MIDI note %f = %f\n",midinote,frequency);
	
	return 0;
}


/* not in book version*/
#ifdef NOTDEF

/* alternative version! */
int main(int argc, char **argv)
{
	double pow(double base, double power);
	
	double frequency;
	
	int midinote;
	
	if(argc != 2){
		printf("usage: cpsmidi midinote\n");
		exit(1);
	}
	/*c0 ~= 8.175799;  */		
	midinote = atoi(argv[1]);
	/* obfuscated C - do not try this at home! */
	frequency = (220.0 * pow(pow(2,1.0/12),3) * pow(0.5,5)) * pow(pow(2,1.0/12),midinote);
	printf("frequency of MIDI note %d = %.20lf\n",midinote,frequency);

	
	return 0;
}

#endif