#ifndef SOURCE_H
#define SOURCE_H

#include <AL/al.h>

class Source
{
private:
    ALuint sourceId;
    ALuint bufferId; //Buffer that we are going to play

public:
    static ALuint filterLowpass;

    Source(float rolloff, float referencedist, float max);

    void play(ALuint buffer);

    void deleteMe();

    void pause();

    void continuePlaying();

    void stop();

    void setVelocity(float x, float y, float z);

    void setLooping(bool loop);

    bool isPlaying();

    void setVolume(float volume);

    void setPitch(float pitch);

    void setPosition(float x, float y, float z);

    void setIsUnderwater(bool isUnderwater);

    //Returns how much percentage the sound is to being completed.
    // So 0 if the sound just started, 0.5 if the sound is halfway done, 1 if sound is finished.
    float getSoundCompletion();

    void setSoundCompletion(float percentage);

    ALuint getSourceId();

    ALuint getLastPlayedBufferId();
};
#endif
