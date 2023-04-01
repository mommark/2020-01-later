


#include <CoreFoundation/CoreFoundation.h>
#include <iostream>


#include "AudioPlayer.h"








int main() {


    const char *fn = "music.mp3";
    
    AudioPlayer* ap = AudioPlayer::file(fn);

    if(!ap) {
        std::cerr << "Error loading audio" << std::endl;
        return 1;
    }
    
    ap->play();
    
    char indic[] = { '|', '/', '-', '\\' };
    int i = 0;
    do {                                               // 5
//        std::cout << "Loop." << std::endl;
        CFRunLoopRunInMode (                           // 6
            kCFRunLoopDefaultMode,                     // 7
            0.25,                                      // 8
            false                                      // 9
        );
        
        std::cout << "\rPlaying.. " << indic[i];

        double d = ap->progress();
        double min = floor(d/60);
        double sec = floor(fmod(d,60));
        std::cout << "   " << min << ":" << sec;

        d = ap->duration();
        min = floor(d/60);
        sec = floor(fmod(d,60));
        std::cout << "   " << min << ":" << sec;

        std::cout << std::flush;
        