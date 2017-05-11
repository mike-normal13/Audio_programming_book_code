#include <stdio.h>
#include "myplug.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
  fprintf(stderr,"My Plugin... a pitch shifter. \n");
  return new MyPlug (audioMaster);
}

 
MyPlug::MyPlug (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, PROGS, PARAMS)
{
  int i;
  programs = new MyPlugProgram[numPrograms];
  gain  = 0.5;
  pitch = 1.0;
  fdb = 0.0;
  dsize = 2000;
  taps = 2;
  sr = getSampleRate();
  delay = new float[(int)sr];
  env = new float[(int)sr];
  for(i=0; i < dsize/2; i++) env[i] = i*2./dsize;
  for(i=dsize/2; i >= 0; i--) env[dsize-i-1] = i*2./dsize;
  if(programs)setProgram(0);
  setNumInputs(1);	// 1 channel
  setNumOutputs(1);	// input & output
  setUniqueID('MpLg');	// this should be unique
  resume ();		// flush buffer
}


MyPlug::~MyPlug ()
{
  if (delay) delete[] delay;
  if (env) delete[] env;
  if (programs) delete[] programs;
}


void MyPlug::setDsize(float d){

  dsize = (int)(sr*d);
   int i;
  for(i=0; i < dsize/2; i++) env[i] = i*2./dsize;
  for(i=dsize/2; i >= 0; i--) env[dsize-i-1] = i*2./dsize;


}

void MyPlug::setProgram (VstInt32 program)
{
  MyPlugProgram* ap = &programs[program];

  curProgram = program;
  setParameter (GAIN,ap->gain);	
  setParameter (PITCH,ap->pitch);
  setParameter (FDB,ap->fdb);
	
}

void MyPlug::setProgramName (char *name)
{
  strcpy (programs[curProgram].name, name);
}

void MyPlug::resume ()
{
  wp = 0;
  rp = 0.;
  memset (delay, 0, (int)(sr * sizeof (float)));
  AudioEffectX::resume ();
}
void MyPlug::getProgramName (char *name)
{
  if (!strcmp (programs[curProgram].name, "Init"))
    sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
  else
    strcpy (name, programs[curProgram].name);
}


bool MyPlug::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
  if (index < PROGS)
    {
      strcpy (text, programs[index].name);
      return true;
    }
  return false;
}



void MyPlug::setParameter (VstInt32 index, float value)
{
  MyPlugProgram &ap = programs[curProgram];

  switch (index)
    {
    case PITCH:    
      setPitch(ap.pitch = value);    
      break;
    case GAIN:     
      setGain(ap.gain = value); 
      break;
    case DSIZE:     
      setDsize(ap.dsize = value); 
      break;
    case FDB:     
      setFdb(ap.fdb = value);    
    }
}


float MyPlug::getParameter (VstInt32 index)
{
  float param = 0;

  switch (index)
    {
    case GAIN:    
      param = gain;   
      break;
    case PITCH:   
      param = pitch;  
      break;
    case FDB:
      param = fdb;
      break;
    case DSIZE:
      param = dsize/sr;
    }
  return param;
}


void MyPlug::getParameterName (VstInt32 index, char *label)
{
  switch (index)
    {
    case GAIN: 
      strcpy (label, "Gain");  
      break;
    case PITCH: 
      strcpy (label, "Pitch");
      break;
    case DSIZE: 
      strcpy (label, "Delay");
      break;
    case FDB: 
      strcpy (label, "Feedback");
       
    }
}


void MyPlug::getParameterDisplay (VstInt32 index, char *text)
{
  switch (index)
    {
    case PITCH:    
      float2string ((pitch*1.5+0.5), text, kVstMaxParamStrLen); 
      break;
    case GAIN:     
      dB2string (gain, text, kVstMaxParamStrLen); 
      break;
    case DSIZE:    
     float2string (dsize/sr, text, kVstMaxParamStrLen);
     break;
    case FDB:    
     float2string (fdb, text, kVstMaxParamStrLen);
    }
}

void MyPlug::getParameterLabel (VstInt32 index, char *label)
{
  switch (index)
    {
    case PITCH:    
      strcpy (label, "ratio");  
      break;
    case GAIN: 
      strcpy (label, "dB");	   
      break;
    case DSIZE: 
      strcpy (label, "secs");	   
      break;
    case FDB:    
      strcpy (label, "gain"); 
    }
}

bool MyPlug::getEffectName (char* name)
{
  strcpy (name, "PitchShift");
  return true;
}

bool MyPlug::getProductString (char* text)
{
  strcpy (text, "PitchShift");
  return true;
}


bool MyPlug::getVendorString (char* text)
{
  strcpy (text, "Acme Audio Ltd");
  return true;
}

void MyPlug::processReplacing (float** inputs, float** outputs, VstInt32 vecframes)
{
  float* in = inputs[0];
  float* out = outputs[0];
  int rpi, ep;
  float s=0.f,rpf, frac, next, p = (pitch*1.5) + 0.5;  
       
  for(int i=0; i < vecframes; i++){

    for(int j=0; j < taps; j++){
      rpf = rp + j*dsize/taps;
      rpf = rpf < dsize ? rpf : rpf - dsize;
      rpi = (int) rpf;  
      frac = rpf - rpi;
      next = (rpi != dsize-1 ? delay[rpi+1] : delay[0]);
      ep = rpi - wp;
      if(ep < 0) ep += dsize;
      s +=  (delay[rpi] + frac*(next - delay[rpi]))*env[ep]; 
      }  

    rp += p;
    rp = rp < dsize ? rp : rp - dsize;
    delay[wp] = in[i] + s*fdb;
    out[i] = (s/taps)*gain;
    s = 0.f;
    wp = (wp < dsize ? wp+1 : 0);

  }

}
