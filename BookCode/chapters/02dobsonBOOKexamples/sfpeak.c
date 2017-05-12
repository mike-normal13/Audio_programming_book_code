// find peak data for a wav or aiff file.
// we look for the max amp value regardless of channel.

#include <stdio.h>
#include <stdlib.h>
#include <portsf.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define FRAMEBUF 1024

int getSampleType(PSF_PROPS* props);
double getFramePeak(float* buf, int blockSize);

int main(int argc, char* argv[])
{
	// frame number containing the max amp
	int peakIndex = 0;
	// value of max amp
	float peakValue = 0;
	// channel containing the peak value
	int peakChannel = 0;

	// file descriptor for user provided sound file
	int ifd = 0;

	// user provided sound file's properties
	PSF_PROPS props;

	int framesRead = 0;

	// number of channels, extracted from the user supplied file's header.
	int nChannels = 0;

	// sample type of user provided sound file
	int sampleType = 0;

	// frame buffers,
	//	will be employed depending on the users supplied file's sample type.
	char* byteBuffer;
	short* shortBuffer;
	// for now,
	//	used for both 24 and 32 bit sample types
	int* intBuffer;
	float* floatBuffer;

	clock_t start;
	clock_t span;

	if(argc < 2)
	{
		printf("insufficient number of arguments.\n");
		printf("USAGE: sfpeak infile(wav or aiff file).\n");
		return 1;
	}

	// start portsf
	if(psf_init())
	{
		printf("failed to start portsf, bye!\n");
		return 1;
	}

	// attempt to open user provided sound file
	ifd = psf_sndOpen(argv[1], &props, 0);

	if(ifd < 0)
	{
		printf("Error opening sound file\n");
		return 1;
	}

	nChannels = props.chans;
	if(nChannels < 1)
	{
		printf("Error: the supplied file appears to have no audio channels");
		return 1;
	}

	sampleType = getSampleType(&props);
	if(sampleType == -1)
	{
		printf("error: sound file has unknown sample type.\n");
		return 1;
	}
	// SOOOOOOO.. portsf only has read methods for float and double,
	//			so there is no reason to distinguish between 8, 16, 24, and 32 bit data types here.....
	// else if the sample type is float...
	else /* if(sampleType == 0) */
	{
		// allocate the float buffer
		floatBuffer = (float*) malloc(FRAMEBUF * nChannels * sizeof(float));
	}
	// // else allocate int buffer based on sample type
	// else if(sampleType == 8){ 	byteBuffer = (char*) malloc(FRAMEBUF * nChannels * sampleType);}
	// else if(sampleType == 16){shortBuffer = (short*) malloc(FRAMEBUF * nChannels * sampleType);}
	// else 
	//{
	// 	assert(sampleType == 24 || sampleType == 32);
	// 	intBuffer = (int*) malloc(FRAMEBUF * nChannels * sampleType); // SUSPECT!!!!!
	// }
	
	
	do
	{
		float tempPeak = 0;

		framesRead = psf_sndReadFloatFrames(ifd, floatBuffer, FRAMEBUF);

		tempPeak = getFramePeak(floatBuffer, FRAMEBUF);

		if(tempPeak > peakValue)
		{
			peakValue = tempPeak;
		}
	}
	while(framesRead > 0);

	printf("found peak value: %f\n", peakValue);
}

double getFramePeak(float* buf, int blockSize)
{
	double max = 0;

	// scan the block
	for(int i; i < blockSize; i++)
	{
		if(fabs(buf[i]) > max)
		{
			max = fabs(buf[i]);
		}
	}

	return max;
}

// returns -1 if the sample type is unknown
// returns 0 if the sample type is PSF_IEEE_FLOAT,
//	otherwise returns 8, 16, 24, or 32 based upon sample type retreived from props arg.
int getSampleType(PSF_PROPS* props)
{
	printf("infile's sample type: ");
	switch(props->samptype)
	{
		case(PSF_SAMP_UNKNOWN):
			printf("PSF_SAMP_UNKOWN\n");
			return -1;
		case(PSF_SAMP_8):
			printf("PSF_SAMP_8\n");
			return 8;
		case(PSF_SAMP_16):
			printf("PSF_SAMP_16\n");
			return 16;
		case(PSF_SAMP_24):
			printf("PSF_SAMP_24\n");
			return 24;
		case(PSF_SAMP_32):
			printf("PSF_SAMP_32\n");
			return 32;
		case(PSF_SAMP_IEEE_FLOAT):
			printf("PSF_SAMP_IEEE_FLOAT\n");
			return 0;
	}
}