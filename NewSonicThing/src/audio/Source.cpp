#include <AL/al.h>
#include <AL/efx.h>

#include "source.h"
#include "../engineTester/main.h"

ALuint Source::filterLowpass = AL_NONE;

Source::Source(float rolloff, float referencedist, float max)
{
    alGenSources(1, &sourceID);
    alSourcef(sourceID, AL_ROLLOFF_FACTOR, rolloff);
    alSourcef(sourceID, AL_REFERENCE_DISTANCE, referencedist);
    alSourcef(sourceID, AL_MAX_DISTANCE, max);

    //Background music shouldn't pan in 3D space
    if (rolloff == 0)
    {
        alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
    }

    bufferID = AL_NONE;
}

void Source::play(ALuint buffer)
{
    stop();
    alSourcei(sourceID, AL_BUFFER, buffer);
    bufferID = buffer;
    continuePlaying();
}

void Source::deleteMe()
{
    stop();
    alDeleteSources(1, &sourceID);
}

void Source::pause()
{
    alSourcePause(sourceID);
}

void Source::continuePlaying()
{
    alSourcePlay(sourceID);
}

void Source::stop()
{
    //if (this->isPlaying())
    {
        //This generates an AL_INVALID_OPERATION sometimes and I have no idea why.
        //Supposedly can only generate this error due to "There is no current context."
        //alGetError();
        alSourceStop(sourceID);
    }
}

void Source::setVelocity(float x, float y, float z)
{
    ALfloat sourceVel[] = { x, y, z };
    alSourcefv(sourceID, AL_VELOCITY, sourceVel);
}

void Source::setLooping(bool loop)
{
    alSourcei(sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

bool Source::isPlaying()
{
    ALint value;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &value);
    return value == AL_PLAYING;
}

void Source::setVolume(float volume)
{
    alSourcef(sourceID, AL_GAIN, volume);
}

void Source::setPitch(float pitch)
{
    alSourcef(sourceID, AL_PITCH, pitch);
}

void Source::setPosition(float x, float y, float z)
{
    ALfloat sourcePos[] = { x, y, z };
    alSourcefv(sourceID, AL_POSITION, sourcePos);
}

void Source::setIsUnderwater(bool isUnderwater)
{
    if (isUnderwater)
    {
        alSourcei(sourceID, AL_DIRECT_FILTER, Source::filterLowpass);
    }
    else
    {
        alSourcei(sourceID, AL_DIRECT_FILTER, AL_FILTER_NULL);
    }
}

ALuint Source::getSourceID()
{
    return sourceID;
}

ALuint Source::getLastPlayedBufferID()
{
    return bufferID;
}

float Source::getSoundCompletion()
{
    if (!isPlaying())
    {
        return 0;
    }

    ALint sizeInBytes;
    //ALint channels;
    //ALint bits;

    alGetBufferi(bufferID, AL_SIZE, &sizeInBytes);

    if (sizeInBytes == 0)
    {
        return 0;
    }

    //alGetBufferi(bufferID, AL_CHANNELS, &channels);
    //alGetBufferi(bufferID, AL_BITS, &bits);

    //float lengthInSamples = sizeInBytes * 8 / (channels * bits);

    float pos;
    alGetSourcef(sourceID, AL_BYTE_OFFSET, &pos);
    float normalizedPos = pos/sizeInBytes;

    return normalizedPos;
}
