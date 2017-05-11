#include <stdio.h>
#include <string.h>
#include <portaudio.h>

#define SR 44100.
#define BUF 512
#define END 60


int main(int agrc, char** argv){

  PaError err;
  PaDeviceIndex devin,devout, ndev;
  const PaDeviceInfo *info;
  PaStreamParameters inparam, outparam;
  PaStream *handle;
  int i, rp = 0;
  float buf[BUF], out = 0.f;
  float delay[(int)SR/2];

  memset(&delay,0,sizeof(float)*SR/2);
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
			NULL, NULL);
    if(err == paNoError){
      err = Pa_StartStream(handle);
      if(err == paNoError){
	while(Pa_GetStreamTime(handle) < END){
	  err = Pa_ReadStream(handle, buf, BUF);  
	  if(err == paNoError){
            for(i=0; i < BUF; i++) {
	              out = delay[rp];
                      delay[rp++] = buf[i] + out*0.5;
		      if (rp >= SR/2) rp = 0;
                      buf[i] = out;
	    }
	    err = (int) Pa_WriteStream(handle, buf, BUF);
	    if(err != paNoError)  printf("%s \n", Pa_GetErrorText(err));
	  } else  printf("%s \n", Pa_GetErrorText(err));
	}
	Pa_StopStream(handle);
      } else  printf("%s \n", Pa_GetErrorText(err));
      Pa_CloseStream(handle);
    } else  printf("%s \n", Pa_GetErrorText(err));
    Pa_Terminate();
  }  else  printf("%s \n", Pa_GetErrorText(err));

  return 0;
}
