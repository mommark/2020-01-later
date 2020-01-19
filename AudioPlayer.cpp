
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

#include "AudioPlayer.h"

#include <iostream>

#define checkStatus(status) checkStatus_(status, __FILE__, __L