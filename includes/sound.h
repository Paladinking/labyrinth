#pragma once
#include <vector>
#include <raylib.h>


class MySound {
public:
    MySound(std::vector<Sound> sounds);

    //Play the next sound if the previos is done
    void PlayMySound();
    void SetVolume(float volume);
private:
    std::vector<Sound> sounds;
    int index;
};