/* nscale.c: Display E.T frequencies for an N-note octave, from a  given MIDI note  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
	int    notes,i,midinote;
	double frequency,ratio;	
	double c0,c5;	
	double intervals[24];

	if(argc != 3){
		printf("Usage: nscale notes miditnote\n");
		return 1;
	}

	notes  = atoi(argv[1]);
	if(notes < 1){
		printf("Error: notes must be positive\n");
		return 1;
	}
	if(notes > 24){
		printf("Error: maximum value for notes is 24\n");
		return 1;
	}

	midinote = atoi(argv[2]);		
	if(midinote < 0){
		printf("Error: cannot have negative MIDI notes!\n");
		return 1;
	}
	if(midinote > 127){
		printf("Error: maximum midinote is 127\n");
		return 1;
	}

	/*** find the frequency of the MIDI note ***/
	
	/* calc standard E.T semitone ratio */
	ratio = pow(2.0,1.0 / 12.0);
	/* find Middle C, three semitones above low A = 220 */
	c5 = 220.0 * pow(ratio,3);
	/* MIDI note 0 is C, 5 octaves below Middle C */
	c0 = c5 * pow(0.5,5);
	frequency = c0 * pow(ratio,midinote);
	

	/* calc ratio from notes, and fill the frequency array */
	ratio = pow(2.0,1.0/notes);
	for(i = 0; i < notes; i++){
		intervals[i] = frequency;
		frequency *= ratio;
	}

	/* finally, read array, write to screen */
	for(i = 0; i < notes; i++){
		printf("%f\n", intervals[i]);		
	}

	return 0;
}