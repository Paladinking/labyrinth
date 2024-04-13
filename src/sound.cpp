#include <iostream>
#include "sound.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

MySound::MySound(std::vector<Sound> sounds) : sounds(sounds), index(0) {};


//Determines if the previos sound is done playing
bool IsPrevPlaying(std::vector<Sound> v, int s) {
    if(s < 1) {
        return IsSoundPlaying(v[v.size() - 1]);
    }else {
        return IsSoundPlaying(v[s - 1]);
    }
}


void MySound::PlayMySound() {
    if(IsPrevPlaying(sounds, index)) {
        return;
    }
    PlaySound(sounds[index]);
    if(index >= sounds.size() - 1) {
        index = 0;
    }else {
        index++;
    }

}

void MySound::SetVolume(float volume) {
    for (size_t i = 0; i < sounds.size(); i++)
    {
        SetSoundVolume(sounds[i], volume);
    }
    
}


