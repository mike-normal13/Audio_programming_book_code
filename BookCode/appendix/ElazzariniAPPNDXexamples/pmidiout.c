#include <stdio.h>
#include <portmidi.h>
#include <porttime.h>


#define MD_NOTEON 0x90
#define MD_NOTEOFF 0x80
#define MD_PRG  0xC0
#define SBYTE(mess,chan) mess | chan

int main() {

int cnt,i,dev;
PmError retval;  
const PmDeviceInfo *info;
PortMidiStream *mstream;

Pm_Initialize();
  
if(cnt = Pm_CountDevices()){

  for(i=0; i < cnt; i++){
   info = Pm_GetDeviceInfo(i);
   if(info->output)
    printf("%d: %s \n", i, info->name);
  }
  printf("choose device: ");
  scanf("%d", &dev); 
  Pt_Start(1, NULL, NULL);
  retval = Pm_OpenOutput(&mstream, dev, 
           NULL,512,NULL,NULL,0);
  
  if(retval != pmNoError)    
     printf("error: %s \n", Pm_GetErrorText(retval));
  else {
    char chan = 0;
    int  prg = 0;
    long time = 0;
    for(i=60; i < 72; prg+=4, i++){
      Pm_WriteShort(mstream, 0, 
         Pm_Message(SBYTE(MD_PRG,chan), prg, 0));
      time = Pt_Time(NULL);
      Pm_WriteShort(mstream, 0, 
         Pm_Message(SBYTE(MD_NOTEON,chan), i, 120));
      while(Pt_Time(NULL) - time < 1000);
      Pm_WriteShort(mstream, 0, 
         Pm_Message(SBYTE(MD_NOTEOFF,chan), i, 120));
    }
  }
  Pm_Close(mstream);  
} else printf("No available output devices\n");

Pm_Terminate();
return 0;
}
