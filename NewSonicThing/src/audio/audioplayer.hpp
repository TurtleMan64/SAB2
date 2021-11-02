#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <AL/al.h>
#include <vector>

class Source;
class Vector3f;

class AudioPlayer
{
private:
    static std::vector<Source*> sources;
    static std::vector<ALuint> buffersSFX;
    static std::vector<ALuint> buffersBGM;

    static bool listenerIsUnderwater;

public:
    static float soundLevelSFX; //The master SFX volume
    static float soundLevelBGM; //The master BGM volume
    static ALuint bgmIntro;
    static ALuint bgmLoop;

public:
    static void loadSettings();

    static void loadSoundEffects();

    static void loadBGM(char* fileName);

    static void deleteSources();

    static void deleteBuffersSFX();

    static void deleteBuffersBGM();

    static void createSources();

    //with position
    static Source* play(int buffer, Vector3f* pos);

    //with position and pitch
    static Source* play(int buffer, Vector3f* pos, float pitch);

    //with position and pitch and loop
    static Source* play(int buffer, Vector3f* pos, float pitch, bool loop);

    //with everything
    static Source* play(int buffer, Vector3f* pos, float pitch, bool loop, float xVel, float yVel, float zVel);

    static Source* playBGMWithIntro(int bufferIntro, int bufferLoop);

    static Source* playBGMWithIntro(ALuint bufferIntro, ALuint bufferLoop);

    static Source* playBGM(int bufferLoop);

    static Source* playBGM(ALuint bufferLoop);

    static Source* getSource(int i);

    //Needs to be called to set the looping buffer to loop
    static void refreshBGM();

    static void stopBGM();

    static void stopAllSFX();

    static ALuint getSFXBuffer(int i);

    //percent should be 0 ... 1
    //this is then multiplied by soundLevelBGM
    static void setBGMVolume(float percent);

    static void setListenerIsUnderwater(bool newisUnderwater);
};
#endif
