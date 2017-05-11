
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "portaudio.h"
#include "tinyAudioLib.h"
#include <string.h>

static double	sample_rate = 44100.0;
static double	in_latency = -1; 
static double	out_latency = -1; 
typedef PaDeviceIndex PaDeviceID;

static PaDeviceID		outPortID = DEFAULT_PORT;
static PaDeviceID		inPortID =  DEFAULT_PORT;

#define SAMPLES_PER_FRAME   (2)
#define FRAMES_PER_BUFFER   (512)

/* Select whether we will use floats or shorts. */
#if 1
#define SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#else
#define SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#endif

static int isOpened=0;
static int isInitialized=0;

    PaStream *stream;


void setInputLatency(double seconds) { in_latency = seconds; }
void setOutputLatency(double seconds) { out_latency = seconds; }
void setSampleRate(double sr) { sample_rate = sr; }



PaError exitPablio(void)
{
    PaError  err;
	if (!isOpened) return paNoError;
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
	isOpened = 0;
    err = Pa_Terminate();
    if( err != paNoError ) goto error;
	isInitialized = 0;
    printf("Exiting from the TinyAudioLibrary.\n");
    
    return err;
error:
    fprintf( stderr, "An error occured while exiting the TinyAudioLibrary!\n" );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    Pa_Terminate();
    return err;

}

void exitAll(void){ exitPablio(); }

PaError initPablio(void)
{
    PaStreamParameters inputParameters, outputParameters, *inPars, *outPars;

	PaDeviceIndex in_dev_ndx, out_dev_ndx;
	PaError  err;
	if (!isInitialized) {
		err = Pa_Initialize();
		if( err != paNoError ) goto error;
		isInitialized = 1;
	}

   	if (inPortID == NO_PORT) /* don't use audio in */
		inPars = NULL;
	else {
		if (inPortID == DEFAULT_PORT) /* default audio out */
			in_dev_ndx = Pa_GetDefaultInputDevice();
		else
			in_dev_ndx = inPortID;
		
		if (in_dev_ndx >= 0 && in_latency < 0) {/* suggested latency unselected, using default */
			in_latency = Pa_GetDeviceInfo( in_dev_ndx )->defaultLowInputLatency;
			if (in_latency <= 0) in_latency = 0.2;
		}
		inputParameters.device = in_dev_ndx;
		inputParameters.channelCount = 2;       /* stereo output */
		inputParameters.sampleFormat = SAMPLE_TYPE; /* 32 bit floating point output */
		inputParameters.suggestedLatency = in_latency;
		inputParameters.hostApiSpecificStreamInfo = NULL;
		inPars = &inputParameters;
	}
	
   	if (outPortID == NO_PORT) /* don't use audio in */
		outPars = NULL;
	else {
		if (outPortID == DEFAULT_PORT) /* default audio out */
			out_dev_ndx = Pa_GetDefaultOutputDevice();
		else
			out_dev_ndx = outPortID;
		
		if (out_dev_ndx >=0 && out_latency < 0) {/* suggested latency unselected, using default */
			out_latency = Pa_GetDeviceInfo( out_dev_ndx )->defaultLowOutputLatency;
			if (out_latency <= 0) out_latency = 0.2;
		}
		outputParameters.device = out_dev_ndx;
		outputParameters.channelCount = 2;       /* stereo output */
		outputParameters.sampleFormat = SAMPLE_TYPE; /* 32 bit floating point output */
		outputParameters.suggestedLatency = out_latency;
		outputParameters.hostApiSpecificStreamInfo = NULL;
		outPars = &outputParameters;
	}
    
	err = Pa_OpenStream(
              &stream,
              inPars,
              outPars,
              sample_rate,
              FRAMES_PER_BUFFER,
              paNoFlag,      
              NULL, /* no callback, use blocking API */
              NULL ); /* no callback, so no callback userData */
    if( err != paNoError ) goto error;
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

	if (!isOpened) {// if user didn't call tinyInit() 
		atexit(exitAll); //automatically call cleanup at exit 
		isOpened = 1;
	}

	return err;

error:
    fprintf( stderr, "An error occured while initializing the TinyAudioLibrary\n" );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    Pa_Terminate();
	isOpened = 0;
    return err;
}


void tinyInit()
{
	PaError err;
	isOpened = 1;
	err = initPablio();
	if (err) {
		fprintf( stderr, "an error occurred while initializing tinyAudioLib.\n");
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		fprintf( stderr, "Exiting.\n");
		exit(-1);
	}
}

void tinyExit()
{
	if(isOpened) {
		PaError err;
		err = exitPablio();
		if (err) {
			fprintf( stderr, "An error occurred while exiting tinyAudioLib.\n");
			fprintf( stderr, "Error number: %d\n", err );
			fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
			fprintf( stderr, "Exiting.\n");
			exit(-1);
		}
		
	}
	else printf("tinyAudioLib wasn't initialized. Doing nothing\n");
}


#define MAX_FRAMES (512 * 2)

void inBlockMono(float *samples, long numsamples)
{
	static float samp[MAX_FRAMES];
	long j,k;
	if (!isOpened) initPablio();
	Pa_ReadStream( stream, &samp[0], numsamples );

	for ( j = 0, k=0; j< numsamples; j++, k+=2) {
		samples[j] = samp[k]  + samp[k+1]; /* mix left and right channels */
	}
}

void outBlockMono(float *samples, long numsamples)
{
	static float samp[MAX_FRAMES];
	long j,k;
	if (!isOpened) initPablio();

	for ( j = 0, k=0; j< numsamples; j++, k+=2) {
		samp[k+1] = samp[k] = samples[j];
	}

	Pa_WriteStream( stream, &samp[0], numsamples );
}

void inBlockStereo(float *samplesLeft, float *samplesRight, long numframes)
{
	static float samp[MAX_FRAMES];
	long j,k;
	if (!isOpened) initPablio();

	Pa_ReadStream( stream, &samp[0], numframes );

	for ( j = 0, k=0; j< numframes; j++, k+=2) {
		samplesLeft [j] = samp[k];
		samplesRight[j] = samp[k+1];
	}
}


void outBlockStereo(float *samplesLeft, float *samplesRight, long numframes)
{
	static float samp[MAX_FRAMES];
	long j,k;
	if (!isOpened) initPablio();

	for ( j = 0, k=0; j< numframes; j++, k+=2) {
		samp[k]   = samplesLeft [j];
		samp[k+1] = samplesRight[j];
	}
	Pa_WriteStream( stream, &samp[0], numframes );

}

void outBlockInterleaved(float *samples, long numframes)
{
	if (!isOpened) initPablio();
	Pa_WriteStream( stream, samples, numframes );
}



void inSampleStereo(float *sampleLeft, float *sampleRight)
{
	float samples[2];
	if (!isOpened) initPablio();
	Pa_ReadStream( stream, samples, 1 );
	*sampleLeft = samples[0];
	*sampleRight = samples[1];
}


void outSampleStereo(float sampleLeft, float sampleRight)
{

	float samples[2];
	if (!isOpened) initPablio();
	samples[0] = sampleLeft;
	samples[1] = sampleRight;
	Pa_WriteStream( stream, &samples[0], 1 );
}



float inSampleMono(void) 
{
	float samples[2];
	if (!isOpened) initPablio();
	Pa_ReadStream( stream, samples, 1 );
	return samples[0] + samples[1]; /* mix left and right channels */
}


void outSampleMono(float sample) 
{
	int err;
	float samples[2];
	if (!isOpened) initPablio();

	samples[0] = sample;
	samples[1] = sample;
	err = Pa_WriteStream( stream, &samples[0], 1 );
}


PaDeviceID SelectOutPort(int port)
{
/* sets an audio OUT device (optional).
   If not set, the default audio i/o ports are used.
    port = -1 --> No out port (for programs that use only audio input)
    port = -2 --> Choose port from a console list of available ports
    port <= -3 --> set the default port ID
*/

	const PaDeviceInfo *info;
	int  j;
	if(!isInitialized) {
		Pa_Initialize();
		isInitialized = 1;
	}
	if (port > -1) {
		info = Pa_GetDeviceInfo( port );
		printf("===== OUTPUT PORT %d =====\n", port);
		printf("\nOutput port %d - [%s] %s will be activated\n", port, Pa_GetHostApiInfo(info->hostApi)->name, info->name);
		printf("==========================\n");
		outPortID = port;
		return outPortID;
	}
	else if (port == paNoDevice) { // -1 == No out port 
		outPortID = paNoDevice;
		return outPortID; 
	}
	else if (port == -2) { // -2 == choose port ID 
		printf("\n===== PLEASE, SELECT OUTPUT PORT NUMBER =====\n");
		for	(j = 0; j < Pa_GetDeviceCount(); j++) {
			info = Pa_GetDeviceInfo(j );
			if (info->maxOutputChannels >= 2 && Pa_GetHostApiInfo(info->hostApi)->type == paMME) {// only stereo MME ports allowed 
				printf("%d) Name: [%s] %s\n", j, Pa_GetHostApiInfo(info->hostApi)->name, info->name);
				/* printf("Default samplig rate: %lf\n", info->defaultSampleRate);
				printf("Max Input channels %d, Max Output channels %d\n", info->maxInputChannels, info->maxOutputChannels);
				printf("Default low input latency %lf, high input latency %lf\n", info->defaultLowInputLatency, info->defaultHighInputLatency);
				printf("Default low output latency %lf, high output latency %lf\n", info->defaultLowOutputLatency, info->defaultHighOutputLatency); */
			}
		}
		printf("==========================\n");
		printf("Type the number of the output port you want to activate: ");
		scanf("%d", &port);
		info = Pa_GetDeviceInfo( port );
		printf("\n...port %d - %s will be activated\n", port, info->name);
		outPortID = port;
		return outPortID; 
	}
	else if (port == -3) { //default port ID < -2 
		outPortID = Pa_GetDefaultOutputDevice();
		return outPortID;
	}
	else {
		fprintf(stderr, "ERROR! %d is an invalid output port\n", port);
		return -5;
	}

}

PaDeviceID SelectInPort(int port)
{
/* sets an audio IN device (optional).
   If not set, the default audio i/o ports are used.
    port = -1 --> No in port (for programs that use only audio output)
    port = -2 --> Choose port from a console list of available ports
    port <= -3 --> set the default port ID
*/
const PaDeviceInfo *info;
	int j;
	if(!isInitialized) {
		Pa_Initialize();
		isInitialized = 1;
	}
	if (port > -1) {
		info = Pa_GetDeviceInfo( port );
		printf("\n===== INPUT PORT %d =====\n", port);
		printf("Input port %d - [%s] %s will be activated\n", port, Pa_GetHostApiInfo(info->hostApi)->name, info->name);
		printf("==========================\n");
		inPortID = port;
		return inPortID;
	}
	else if (port == paNoDevice) { // -1 == No out port 
		inPortID = paNoDevice;
		return inPortID; 
	}
	else if (port == -2) { // -2 == choose port ID 
		printf("\n===== PLEASE, SELECT INPUT PORT NUMBER =====\n");
		for	(j = 0; j < Pa_GetDeviceCount(); j++) {
			info = Pa_GetDeviceInfo(j );
			if (info->maxInputChannels == 2 && Pa_GetHostApiInfo(info->hostApi)->type == paMME) { // only stereo ports allowed 
				printf("%d) Name: [%s] %s\n", j, Pa_GetHostApiInfo(info->hostApi)->name, info->name);
				/* printf("Default samplig rate: %lf\n", info->defaultSampleRate);
				printf("Max Input channels %d, Max Output channels %d\n", info->maxInputChannels, info->maxOutputChannels);
				printf("Default low input latency %lf, high input latency %lf\n", info->defaultLowInputLatency, info->defaultHighInputLatency);
				printf("Default low output latency %lf, high output latency %lf\n", info->defaultLowOutputLatency, info->defaultHighOutputLatency); */
			}
		}
		printf("==========================\n");
		printf("Type the number of the input port you want to activate: ");
		scanf("%d", &port);
		info = Pa_GetDeviceInfo( port );
		printf("\n...port %d - %s will be activated\n", port, info->name);
		inPortID = port;
		return inPortID; 
	}
	else if (port == -3) { // default port ID < -2 
		inPortID = Pa_GetDefaultInputDevice();
		return inPortID;
	}
	else {
		fprintf(stderr, "ERROR! %d is an invalid input port\n", port);
		return -5;
	}
}

void setOutPort(int port) { 
	outPortID = SelectOutPort(port); 
}

void setInPort(int port)  { 
	inPortID  = SelectInPort(port); 
}

