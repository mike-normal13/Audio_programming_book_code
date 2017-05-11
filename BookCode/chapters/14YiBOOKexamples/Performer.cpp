#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "Performer.hpp"

using namespace std;

float randFloat() {
    return (float) rand()/RAND_MAX;
}

Performer::Performer(int _instrumentNum,
                    float _baseAmp,
                    float _space,
                    string _name) {

    instrumentNum = _instrumentNum;
    baseAmp = _baseAmp;
    space = _space;
    name = _name;
}

string Performer::toString() {
    char buffer[120];

    sprintf(buffer,
            ";[P: %s] instrument: %i baseAmp: %f space: %f\n",
            name.c_str(),
            instrumentNum,
            baseAmp,
            space);

    return string(buffer);
}

string Performer::perform(vector<Note> noteList, float startTime) {
    string retVal = "";

    for(size_t i = 0; i < noteList.size(); i++) {
        Note n = noteList[i];

        float timeVariance = randFloat() * .01;
        float startVariance = randFloat() * timeVariance;

        n.startTime += startTime + startVariance;
        // adjust time of note to keep the note starts the same
        n.duration = n.duration - timeVariance;
        n.amp = n.amp * baseAmp;

        retVal += n.getNote(instrumentNum, space) + "\n";
        //start += timeVariance + n.duration;

    }

    return retVal;
}

string Performer::performAleatorically(vector<Note> noteList,
                                float timeVariance,
                                float ampVariance,
                                float pitchVariance,
                                float durationOfSection,
                                float startTime) {

    string retVal = "";
    float timeCounter = startTime;

    while (timeCounter < (startTime + durationOfSection)) {
        vector<Note> tempNoteList = noteList;

        for(size_t i = 0; i < tempNoteList.size(); i++) {
            tempNoteList[i].startTime = tempNoteList[i].startTime + timeCounter;
        }

        float timeAdj = 1 + (randFloat() * timeVariance );
        float ampAdj  = baseAmp - (randFloat() * ampVariance);

        for(size_t i = 0; i < tempNoteList.size(); i++) {
            tempNoteList[i].startTime = tempNoteList[i].startTime * timeAdj;
            tempNoteList[i].duration = tempNoteList[i].duration * timeAdj;
        }

        for(size_t i = 0; i < tempNoteList.size(); i++) {
            tempNoteList[i].amp = tempNoteList[i].amp * ampAdj;

            retVal += tempNoteList[i].getNote(instrumentNum, space) + "\n";

            timeCounter += tempNoteList[i].duration;
        }
    }

    return retVal;

}

