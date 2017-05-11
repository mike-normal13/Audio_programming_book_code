#include "Note.hpp"
#include <string>
#include <stdio.h>

using namespace std;

Note::Note(float _startTime, 
        float _duration, 
        float _pitch, 
        float _pitch2, 
        float _amp, 
        int _envType) {
            
    startTime = _startTime;
    duration = _duration;
    pitch = _pitch;
    pitch2 = _pitch2;
    amp = _amp;
    envType = _envType;
    isRest = false;
    
};

Note::Note(float _startTime,
        float _duration) {
    startTime = _startTime;
    duration = _duration;
    isRest = true;
}

string Note::getNote(int instrumentNum, float space) {
            
    if(isRest) {
        return "";   
    }
    
    char buffer[120];
        
    sprintf(buffer, 
            "i %i %f %f %f %f %f %f %i", 
            instrumentNum,
            startTime,
            duration,
            pitch,
            pitch2, 
            amp, 
            space,
            envType);
            
    string noteText(buffer);
    
    
    return noteText;    
}
