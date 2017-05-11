#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <portaudio.h>

#define SR 44100.
#define BUF 512
#define END  60

typedef struct _mydata{
  float delay[(int)SR/2];
  int rp;
} mydata;


int audio_callback(const void *input, void *output,
               unsigned long frameCount,
               const PaStreamCallbackTimeInfo *timeInfo,
               PaStreamCallbackFlags statusFlags,
	       void *userData){


  mydata *p = (mydata *)userData;
  int i, rp = p->rp;
  float out, *delay = p->delay;
  float *inp = (float *) input, *outp = (float *) output;
  for(i=0; i < frameCount; i++){
    out = delay[rp];
    delay[rp++] = inp[i] + out*0.5;
    if(rp >= SR/2) rp = 0;
    outp[i] = out;
  }
  p->rp = rp;

  return paContinue;

}

int main(int agrc, char** argv){

  PaError err;
  PaDeviceIndex devin,devout, ndev;
  const PaDeviceInfo *info;
  PaStreamParameters inparam, outparam;
  PaStream *handle;
  int i;
  mydata *data = (mydata *) calloc(sizeof(mydata),1);

  err = Pa_Initialize();
  if( err == paNoError){
    ndev = Pa_GetDeviceCount();
    for(i=0; i<ndev; i++){ 
      info = Pa_GetDeviceInfo((PaDeviceIndex) i);
      if(info->maxOutputChannels > 0) printf("output device: ");
      if (info->maxInputChannels > 0) printf("input device: "); 
      printf("%d: %s\n", i, info->name);
    }

    printf("choose device for input: ");
    scanf("%d", &devin);
    printf("choose device for output: ");
    scanf("%d", &devout);

    memset(&inparam, 0, sizeof(PaStreamParameters));
    inparam.device = devin;
    inparam.channelCount = 1;
    inparam.sampleFormat = paFloat32;
    memset(&outparam, 0, sizeof(PaStreamParameters));
    outparam.device = (PaDeviceIndex) devout;
    outparam.channelCount = 1;
    outparam.sampleFormat = paFloat32;

    err = Pa_OpenStream(&handle,&inparam,&outparam,SR,BUF,paNoFlag, 
			audio_callback, data);

    if(err == paNoError){
      err = Pa_StartStream(handle);
      if(err == paNoError){
	while(Pa_GetStreamTime(handle) < END); 
	Pa_StopStream(handle);
      } else  printf("%s \n", Pa_GetErrorText(err));
      Pa_CloseStream(handle);
    } else  printf("%s \n", Pa_GetErrorText(err));
    Pa_Terminate();
  }  else  printf("%s \n", Pa_GetErrorText(err));

  free(data);
  return 0;
}
