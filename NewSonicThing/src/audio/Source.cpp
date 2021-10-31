#include <AL/al.h>
#include <AL/efx.h>

#include "source.h"
#include "../engineTester/main.h"

ALuint Source::filterLowpass = AL_NONE;

Source::Source(float rolloff, float referencedist, float max)
{
    alGenSources(1, &sourceId);
    alSourcef(sourceId, AL_ROLLOFF_FACTOR, rolloff);
    alSourcef(sourceId, AL_REFERENCE_DISTANCE, referencedist);
    alSourcef(sourceId, AL_MAX_DISTANCE, max);

    //Background music shouldn't pan in 3D space
    if (rolloff == 0)
    {
        alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_TRUE);
    }

    bufferId = AL_NONE;
}

void Source::play(ALuint buffer)
{
    stop();
    alSourcei(sourceId, AL_BUFFER, buffer);
    bufferId = buffer;
    continuePlaying();
}

void Source::deleteMe()
{
    stop();
    alDeleteSources(1, &sourceId);
}

void Source::pause()
{
    alSourcePause(sourceId);
}

void Source::continuePlaying()
{
    alSourcePlay(sourceId);
}

void Source::stop()
{
    //if (this->isPlaying())
    {
        //This generates an AL_INVALId_OPERATION sometimes and I have no idea why.
        //Supposedly can only generate this error due to "There is no current context."
        //alGetError();
        alSourceStop(sourceId);
    }
}

void Source::setVelocity(float x, float y, float z)
{
    ALfloat sourceVel[] = { x, y, z };
    alSourcefv(sourceId, AL_VELOCITY, sourceVel);
}

void Source::setLooping(bool loop)
{
    alSourcei(sourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

bool Source::isPlaying()
{
    ALint value;
    alGetSourcei(sourceId, AL_SOURCE_STATE, &value);
    return value == AL_PLAYING;
}

void Source::setVolume(float volume)
{
    alSourcef(sourceId, AL_GAIN, volume);
}

void Source::setPitch(float pitch)
{
    alSourcef(sourceId, AL_PITCH, pitch);
}

void Source::setPosition(float x, float y, float z)
{
    ALfloat sourcePos[] = { x, y, z };
    alSourcefv(sourceId, AL_POSITION, sourcePos);
}

void Source::setIsUnderwater(bool isUnderwater)
{
    if (isUnderwater)
    {
        alSourcei(sourceId, AL_DIRECT_FILTER, Source::filterLowpass);
    }
    else
    {
        alSourcei(sourceId, AL_DIRECT_FILTER, AL_FILTER_NULL);
    }
}

ALuint Source::getSourceId()
{
    return sourceId;
}

ALuint Source::getLastPlayedBufferId()
{
    return bufferId;
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

    alGetBufferi(bufferId, AL_SIZE, &sizeInBytes);

    if (sizeInBytes == 0)
    {
        return 0;
    }

    //alGetBufferi(bufferId, AL_CHANNELS, &channels);
    //alGetBufferi(bufferId, AL_BITS, &bits);

    //float lengthInSamples = sizeInBytes * 8 / (channels * bits);

    float pos;
    alGetSourcef(sourceId, AL_BYTE_OFFSET, &pos);
    float normalizedPos = pos/sizeInBytes;

    return normalizedPos;
}
