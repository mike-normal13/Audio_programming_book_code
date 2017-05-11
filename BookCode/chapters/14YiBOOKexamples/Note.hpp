#ifndef NOTE_H
#define NOTE_H

#include <string>

using namespace std;

class Note {   
  public:
    float startTime; 
    float duration; 
    float pitch;
    float pitch2; 
    float amp;
    int envType;
    bool isRest;
    
    Note(float startTime, float duration, float pitch, float pitch2, float amp, int envType);
    
    // This constructor is used to denote a Rest and sets isRest to true
    Note(float startTime, float duration);
           
    string getNote(int instrumentNum, float space);
};


#endif
