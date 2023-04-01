


#include <CoreFoundation/CoreFoundation.h>
#include <iostream>


#include "AudioPlayer.h"








int main() {


    const char *fn = "music.mp3";
    
    AudioPlayer* ap = AudioPlayer::file(fn);