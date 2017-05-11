#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>

int main() {

  int cnt,i,dev;
  PmError retval;  
  const PmDeviceInfo *info;
  PmEvent msg[32];
  PortMidiStream *mstream;
  Pm_Initialize();
  cnt = Pm_CountDevices();
  if(cnt) {
    for(i=0; i < cnt; i++){
      info = Pm_GetDeviceInfo(i);
      if(info->input)
	printf("%d: %s \n", i, info->name);
    }
    printf("choose device: ");
    scanf("%d", &dev); 
    Pt_Start(1, NULL, NULL);
    retval = Pm_OpenInput(&mstream, dev, NULL, 512L, NULL,NULL);
  
    if(retval != pmNoError)    
      printf("error: %s \n", Pm_GetErrorText(retval));
    else {
      while(Pt_Time(NULL) < 10000){
	if(Pm_Poll(mstream)) {
	  cnt = Pm_Read(mstream, msg, 32);
	  for(i=0; i<cnt; i++) {
	    printf("status:%d, byte1=%d, byte2=%d, time=%.3f\n", 
		   Pm_MessageStatus(msg[i].message),
		   Pm_MessageData1(msg[i].message),
		   Pm_MessageData2(msg[i].message),
		   msg[i].timestamp/1000.);
	  }
	}
      }

    }
    Pm_Close(mstream);
  
  }
  else printf("No MIDI devices found\n");
  Pm_Terminate();
  return 0;
}
