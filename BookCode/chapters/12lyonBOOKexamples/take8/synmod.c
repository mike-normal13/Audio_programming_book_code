#include <stdio.h>
#include <stdlib.h>
#include "synmod.h"

main(int argc, char **argv)
{
	
	OSCMOD *oscs;
	MIXOUT *mixes;
	NOISEMOD *noises; 
	NOISEMOD *sahs; /* We now define a pointer for the SAH modules */
	
	int osc_count = 0;
	int mix_count = 0;
	int noise_count = 0; 
	int sah_count = 0; /* We now keep count of SAH modules */
	char modname[64];
	int i;
	float duration = 10.0; 
	
	oscs = (OSCMOD *) malloc(MAXMODS * sizeof(OSCMOD));
	mixes = (MIXOUT *) malloc(MAXMODS * sizeof(MIXOUT));
	noises = (NOISEMOD *) malloc( MAXMODS * sizeof(NOISEMOD));
	/* Allocate SAH units: */
	sahs = (NOISEMOD *) malloc(MAXMODS * sizeof(NOISEMOD));
	
	print_header();
	
	while(scanf("%s", modname) != EOF){
		if(! strcmp(modname, "OSC")){
			read_osc(oscs, osc_count++);
		}
		
		else if(! strcmp(modname, "NOISE")){
			read_noise(noises, noise_count++);
		}
		/* Here we scan for SAH modules */
		else if(! strcmp(modname, "SAH")){
			read_noise( sahs, sah_count++);
		}
		
		else if(! strcmp(modname, "MIXOUT")){
			read_mix(mixes, mix_count++);
		} 
		else if(! strcmp(modname, "DURATION")){
			scanf("%f", &duration);
		} 
		else {
			fprintf(stderr,"%s is an unknown module\n", modname);
		}
	} 
	/* Once again we modify the global initialization function */
	initialize_globals(oscs, osc_count, noises, noise_count, sahs, sah_count);
	
	for(i = 0; i < osc_count; i++){
		print_osc(oscs[i]);
	}
	for(i = 0; i < noise_count; i++){
		print_noise(noises[i]);
	}
	/* Here we print the SAH modules. */
	for( i = 0; i < sah_count; i++){
		print_sah( sahs[i]);
	}
	for(i = 0; i < mix_count; i++){
		print_mix(mixes[i]);
	}
    print_score(duration);     
}

void read_osc(OSCMOD *oscs, int count)
{
	scanf("%s %s %s %s %s %s %s",oscs[count].sig_out,
		  oscs[count].frequency, oscs[count].waveform,
		  oscs[count].sig_am, oscs[count].sig_fm,
		  oscs[count].omin, oscs[count].omax);
	if(count >= MAXMODS){
		fprintf(stderr,"Number of oscillators has exceeded maximum: %d\n",
				MAXMODS);
		exit(1);
	}
}


void print_osc(OSCMOD osc)
{
	float omin, omax;
	float mo2;
	
	printf("%s oscil ", osc.sig_out);
	if(!strcmp(osc.sig_am, "NONE")){
		printf("1.0, ");
	} else {
		printf("%s, ", osc.sig_am);
	}
	if(!strcmp(osc.sig_fm, "NONE")){
		printf("%s, ", osc.frequency);
	} else {
		printf("%s * (1.0 + %s), ",osc.frequency, osc.sig_fm);
	}
	if(!strcmp(osc.waveform, "SINE")){
		printf("isine\n");
	}
	else if(!strcmp(osc.waveform, "TRIANGLE")) {
		printf("itriangle\n");
	}
	else if(!strcmp(osc.waveform, "SAWTOOTH")){
		printf("isawtooth\n");
	}
	else if(!strcmp(osc.waveform, "SQUARE")){
		printf("isquare\n");
	}
	else if(!strcmp(osc.waveform, "PULSE")){
		printf("ipulse\n");
	}
	else {
		fprintf(stderr,"print_osc: %s is unknown - using sine instead\n",osc.waveform);
		printf("isine\n");
	}
	sscanf(osc.omin,"%f",&omin); // convert strings to floats
	sscanf(osc.omax,"%f",&omax);
	if(omin != -1.0 || omax != 1.0){ // rescale output if necessary
		mo2 = (omax - omin) / 2.0;
		printf("%s = %s + (%f*%s + %f)\n",
			   osc.sig_out, osc.omin, mo2, osc.sig_out, mo2);
	}
}

void read_mix(MIXOUT *mix, int count )
{
	scanf("%s %s", mix[count].outvar, mix[count].amplitude );
}

void print_mix( MIXOUT mix )
{
	float amp;
	sscanf(mix.amplitude, "%f", &amp);
	printf ("kenv linseg 0,.05,%f,p3-0.1,%f,.05,0\n",amp, amp);
	printf("out (%s)*kenv\n", mix.outvar);
}

void print_header(void)
{
	printf("<CsoundSynthesizer>\n\n");
	printf("sr = 44100\n");
	printf("kr = 4410\n");
	printf("ksmps = 10\n");
	printf("nchnls = 1\n\n");
	printf("<CsInstruments>\n\n");
	printf("\tinstr 1\n");
	printf("isine = 1\n");
	printf("itriangle = 2\n");
	printf("isawtooth = 3\n");
	printf("isquare = 4\n");
	printf("ipulse = 5\n");
}

void print_score(float duration)
{
	printf("\tendin\n\n");
	printf("</CsInstruments>\n");
	printf("<CsScore>\n\n");
	printf("f1 0 8192 10 1 ; sine\n");
	printf("f2 0 8192 10 1 0 .111 0 .04 0 .02 0 ; triangle\n");
	printf("f3 0 8192 10 1 .5 .333 .25 .2 .166 .142 .125 ; sawtooth\n");
	printf("f4 0 8192 10 1 0 .333 0 .2 0 .142 0 .111; square\n");
	printf("f5 0 8192 10 1 1 1 1 1 1 1 1 1 1 1 1 1; pulse\n\n");
	printf("i1 0 %f\n\n", duration);
	printf("</CsScore>\n");
	printf("</CsoundSynthesizer>\n");
}

void initialize_globals(OSCMOD *oscs, int osc_count, NOISEMOD *noises,
						int noise_count, NOISEMOD *sahs, int sah_count)
{
	int i;
	
	for(i = 0; i < osc_count; i++){
		printf("%s init 0.0\n", oscs[i].sig_out);
	}
	for(i = 0; i < noise_count; i++){
		printf("%s init 0.0\n", noises[i].sig_out);
	}
	for(i = 0; i < sah_count; i++){
		printf("%s init 0.0\n", sahs[i].sig_out);
	}
}


void read_noise(NOISEMOD *unit, int count)
{
	scanf("%s %s %s %s %s", unit[count].sig_out, unit[count].speed,
		  unit[count].seed, unit[count].omin, unit[count].omax);
}

void print_noise(NOISEMOD noise)
{
	float omin, omax, mo2;
	
	printf("%s randi 1.0, %s, %s\n", noise.sig_out, noise.speed, noise.seed);
	sscanf(noise.omin,"%f",&omin); // convert strings to floats
	sscanf(noise.omax,"%f",&omax);
	
	if(omin != -1.0 || omax != 1.0){
		mo2 = (omax - omin) / 2.0;
		printf("%s = %s + (%f*%s + %f)\n",
			   noise.sig_out, noise.omin, mo2, noise.sig_out, mo2);
	}
}

void print_sah(NOISEMOD noise)
{
	float omin, omax, mo2;
	
	printf("%s randh 1.0, %s, %s\n", noise.sig_out, noise.speed, noise.seed);
	sscanf(noise.omin,"%f",&omin); // convert strings to floats
	sscanf(noise.omax,"%f",&omax);
	
	if(omin != -1.0 || omax != 1.0){
		mo2 = (omax - omin) / 2.0;
		printf("%s = %s + (%f*%s + %f)\n",
			   noise.sig_out, noise.omin, mo2, noise.sig_out, mo2);
	}
}


