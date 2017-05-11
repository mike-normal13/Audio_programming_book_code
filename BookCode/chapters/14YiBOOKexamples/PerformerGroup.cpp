#include <cstdlib>
#include "PerformerGroup.hpp"
#include "Utilities.cpp"

using namespace std;

PerformerGroup::PerformerGroup(string _name) {
    name = _name;   
}

string PerformerGroup::toString() {
    string retVal = ";[PerformerGroup: " + name + "]\n";
        
    for(size_t i = 0; i < performers.size(); i++) {
        retVal += performers[i]->toString();   
    }
    
    retVal += ";---------------------\n\n";
    
    return retVal;
}

string PerformerGroup::perform(vector<Note> noteList, float startTime) {
    string retVal = "";
    
    for(size_t i = 0; i < performers.size(); i++) {
        retVal += performers[i]->perform(noteList, startTime);
    }    
    
    return retVal;
}

string PerformerGroup::performAleatorically(vector<Note> noteList,
                                float timeVariance, 
                                float ampVariance, 
                                float pitchVariance, 
                                float durationOfSection,
                                float startTime,
                                float startRange) {
                                         
    string retVal = "";
    
    for(size_t i = 0; i < performers.size(); i++) {
    	
    	float localStartTime = startTime + 
    		(startRange * ((float) rand()/RAND_MAX)); 
    	
        retVal += performers[i]->performAleatorically(noteList, 
                                                    timeVariance, 
                                                    ampVariance, 
                                                    pitchVariance,
                                                    durationOfSection,
                                                    localStartTime);   
    }    
    
    return retVal;
}


/**
    using y = mx + b to calculate the start and end points of the two surface
    edges
    
    y maps to pch, x maps to time
        
    note: currently does not check if the second set of point starts before the
    first and may cause unwanted results */
    
string PerformerGroup::performSurface(float amp, 
                                        int envType, 
                                        float time1a, float pch1a, 
                                        float time1b, float pch1b, 
                                        float time2a, float pch2a, 
                                        float time2b, float pch2b) {
 
    
    string retVal = "";
                                            
    float line1Dur = time1b - time1a;
    float line2Dur = time2b - time2a;

    float m1 = pchDiff(pch1a, pch1b) / (performers.size() - 1);
    float m2 = pchDiff(pch2a, pch2b) / (performers.size() - 1);
    float b1 = pch1a;
    float b2 = pch2a;
    
    for(int i = 0; i < performers.size(); i++) {
        float x = i / (float)(performers.size() - 1);
        
        float startTime = (x * line1Dur) + time1a;
        float endTime = (x * line2Dur) + time2a;
        float duration = endTime - startTime;
        
        float pch1 = pchAdd(pch1a, m1 * i);
        float pch2 = pchAdd(pch2a, m2 * i);

        vector<Note> noteList;
        noteList.push_back(Note(startTime, duration, pch1,  pch2, amp, envType));
        
        // use 0.0f for startTime as note start times are already calculated 
        // relative to the times designated for the surface 
        retVal += performers[i]->perform(noteList, 0.0f);
        
    
    }
        
    return retVal;
}
