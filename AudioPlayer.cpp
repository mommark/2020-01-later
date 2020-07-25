
#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

#include "AudioPlayer.h"

#include <iostream>

#define checkStatus(status) checkStatus_(status, __FILE__, __LINE__)

static const int kNumberBuffers = 3;                              // 1
struct AQPlayerState {
    AudioStreamBasicDescription   mDataFormat;                    // 2
    AudioQueueRef                 mQueue;                         // 3
    AudioQueueBufferRef           mBuffers[kNumberBuffers];       // 4
    AudioFileID                   mAudioFile;                     // 5
    UInt32                        bufferByteSize;                 // 6
    SInt64                        mCurrentPacket;                 // 7
    UInt32                        mNumPacketsToRead;              // 8
    AudioStreamPacketDescription  *mPacketDescs;                  // 9
    bool                          mIsRunning;                     // 10
};




static void HandleOutputBuffer (
    void                *aqData,
    AudioQueueRef       inAQ,
    AudioQueueBufferRef inBuffer
) {
    
//    std::cout << "cb" << std::endl;
    
    OSStatus status;
    
    AQPlayerState *pAqData = (AQPlayerState *) aqData;        // 1
    if (pAqData->mIsRunning == 0) return;                     // 2
    UInt32 numBytesReadFromFile;                              // 3
    UInt32 numPackets = pAqData->mNumPacketsToRead;           // 4
    status = AudioFileReadPackets (
        pAqData->mAudioFile,
        false,
        &numBytesReadFromFile,
        pAqData->mPacketDescs, 
        pAqData->mCurrentPacket,
        &numPackets,
        inBuffer->mAudioData 
    );
//    checkStatus(status);
    if (numPackets > 0) {                                     // 5
        inBuffer->mAudioDataByteSize = numBytesReadFromFile;  // 6
       status = AudioQueueEnqueueBuffer ( 
            pAqData->mQueue,
            inBuffer,
            (pAqData->mPacketDescs ? numPackets : 0),
            pAqData->mPacketDescs
        );
//        checkStatus(status);
        pAqData->mCurrentPacket += numPackets;                // 7 
    } else {
        status = AudioQueueStop (
            pAqData->mQueue,
            false
        );
//        checkStatus(status);
        pAqData->mIsRunning = false; 
    }
}





class AudioPlayerOsx : public AudioPlayer {
public:
    
    bool load(CFURLRef url) {

        OSStatus status;
        memset(&aqData,0,sizeof(aqData));
        timeBase = 0;
        
        status = AudioFileOpenURL(url,kAudioFileReadPermission,0,&aqData.mAudioFile);
        checkStatus(status);
        if( status != noErr ) return false;
        
        
        UInt32 dataFormatSize = sizeof (aqData.mDataFormat);    // 1

        status = AudioFileGetProperty (                                  // 2
            aqData.mAudioFile,                                  // 3
            kAudioFilePropertyDataFormat,                       // 4
            &dataF