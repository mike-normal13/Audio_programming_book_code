#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>

#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0

int main() {

  int cnt,i,dev[2], j, outcnt=0, end=60000, chan=0;
  PmError retval[2];  
  const PmDeviceInfo *info;
  PmEvent msg[128], msgout[1024];
  PortMidiStream *mstream[2];
  Pm_Initialize();
  cnt = Pm_CountDevices();
  if(cnt) {
    for(j=0; j<2; j++){
      	if(j==0) printf("input devices:\n");
	else printf("output devices:\n");
      for(i=0; i < cnt; i++){
	info = Pm_GetDeviceInfo(i);
	if(j==0 && info->input)
	    printf("%d: %s \n", i, info->name);
	if(j==1 && info->output)
	    printf("%d: %s \n", i, info->name);
	}
      printf("choose device: ");
      scanf("%d", &dev[j]); 
    }

    Pt_Start(1, NULL, NULL);
    retval[0] = Pm_OpenInput(&mstream[0], dev[0], NULL, 512L, NULL,NULL);
    retval[1] = Pm_OpenOutput(&mstream[1], dev[1], NULL, 512L, NULL,NULL,0L);

    if(retval[0] != pmNoError && retval[0] != pmNoError)    
      printf("error in input device (%s) or in output device (%s) \n", 
	     Pm_GetErrorText(retval[0]), Pm_GetErrorText(retval[1]) );
    else {
      while(Pt_Time(NULL) < end){
	/* Poll for inputs */
       if(Pm_Poll(mstream[0])) {
	 /* if there are MIDI events waiting, read them */
        cnt = Pm_Read(mstream[0], msg, 32);
	for(i=0; i<cnt; i++) {
          /* now create the arpeggios in the msgout buffer */
           if((Pm_MessageStatus(msg[i].message) & 0xF0) == MD_NOTEON) {
	     for(j=0;j<4; j++){
               /* NOTE ONs with correct timestamp */
               msgout[outcnt].message = Pm_Message(MD_NOTEON | chan,
				  Pm_MessageData1(msg[i].message)+j*4,
				  Pm_MessageData2(msg[i].message));
               msgout[outcnt].timestamp = msg[i].timestamp+1000*j;
	       if (++outcnt == 1024) outcnt = 0; /* increment count, wrap around */
               /* NOTE OFFs */
               msgout[outcnt].message = Pm_Message(MD_NOTEOFF | chan,
				  Pm_MessageData1(msg[i].message)+j*4,
				  Pm_MessageData2(msg[i].message));
               msgout[outcnt].timestamp = msg[i].timestamp+1000*j+1000;
               if (++outcnt == 1024) outcnt = 0; /* increment count, wrap around */
	     }
	   }
 	 }
       }
       /* now msgout might have MIDI events, write them if they are due */
       for(i=0; i < outcnt; i++){
         /* check if they are due by looking at their timestamp */
	 if(Pt_Time(NULL) >= msgout[i].timestamp && msgout[i].timestamp >= 0){
	   Pm_WriteShort(mstream[1], 0, msgout[i].message);
           msgout[i].timestamp = -1; /* mark them as read */
	 }	  
       }
      }	
    }
    Pm_Close(mstream[0]);
    Pm_Close(mstream[1]);
  }
  else printf("No MIDI devices found\n");
  Pm_Terminate();
  return 0;
}
