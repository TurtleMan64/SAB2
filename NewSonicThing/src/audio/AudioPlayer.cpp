#include <AL/al.h>
#include <vector>
#include <cstring>
#include <fstream>

#include "audioplayer.h"
#include "audiomaster.h"
#include "source.h"
#include "../engineTester/main.h"
#include "../toolbox/vector.h"
#include "../toolbox/split.h"



float AudioPlayer::soundLevelSE = 1.0f;
float AudioPlayer::soundLevelBGM = 1.0f;
std::vector<Source*> AudioPlayer::sources;
std::vector<ALuint> AudioPlayer::buffersSE;
std::vector<ALuint> AudioPlayer::buffersBGM;
ALuint AudioPlayer::bgmIntro;
ALuint AudioPlayer::bgmLoop;


void AudioPlayer::loadSoundEffects()
{
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boostpad.ogg"));           //0
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Healpad.ogg"));            //1
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boost/boost_falcon.ogg")); //2
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Intro321GO.ogg"));         //3
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/HitWall.ogg"));            //4
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Engine.ogg"));             //5
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Strafe.ogg"));             //6
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/SlipSlowdown.ogg"));       //7
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Danger.ogg"));             //8
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/AnnounceBoostPower.ogg")); //9
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/AnnounceFinalLap.ogg"));   //10
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/FallFemale.ogg"));         //11
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/FallMale.ogg"));           //12
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boost/boost_bull.ogg"));   //13
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boost/boost_falcon.ogg")); //14
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boost/boost_norita.ogg")); //15
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boost/boost_phantom.ogg"));//16
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/Boost/boost_wyvern.ogg")); //17
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/ExplosionBig.ogg"));       //18
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG("res/Audio/SFX/ExplosionSmall.ogg"));     //19
}

void AudioPlayer::loadBGM(char* fileName)
{
	AudioPlayer::buffersBGM.push_back(AudioMaster::loadOGG(fileName));
}

void AudioPlayer::deleteSources()
{
	for (Source* src : AudioPlayer::sources)
	{
		src->deleteMe();
		delete src; INCR_DEL
	}
	AudioPlayer::sources.clear();
	AudioPlayer::sources.shrink_to_fit();
}

void AudioPlayer::deleteBuffersSE()
{
	for (ALuint buff : AudioPlayer::buffersSE)
	{
		alDeleteBuffers(1, &buff);
	}
	AudioPlayer::buffersSE.clear();
	AudioPlayer::buffersSE.shrink_to_fit();
}

void AudioPlayer::deleteBuffersBGM()
{
	Source* src = AudioPlayer::sources[14];
	src->stop();

	for (ALuint buff : AudioPlayer::buffersBGM)
	{
		alDeleteBuffers(1, &buff);
	}
	AudioPlayer::buffersBGM.clear();
	AudioPlayer::buffersBGM.shrink_to_fit();
}

void AudioPlayer::createSources()
{
	//First 14 sources are for sound effects
	sources.push_back(new Source(1, 100, 600)); INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	sources.push_back(new Source(1, 100, 600));	INCR_NEW
	//Last source is dedicated to background music
	sources.push_back(new Source(0, 0, 0));     INCR_NEW
}

//with position
Source* AudioPlayer::play(int buffer, Vector3f* pos)
{
	return AudioPlayer::play(buffer, pos, 1.0f, false, 0, 0, 0);
}

//with position and pitch
Source* AudioPlayer::play(int buffer, Vector3f* pos, float pitch)
{
	return AudioPlayer::play(buffer, pos, pitch, false, 0, 0, 0);
}

//with position and pitch and loop
Source* AudioPlayer::play(int buffer, Vector3f* pos, float pitch, bool loop)
{
	return AudioPlayer::play(buffer, pos, pitch, loop, 0, 0, 0);
}

//with everything
Source* AudioPlayer::play(int buffer, Vector3f* pos, float pitch, bool loop, float xVel, float yVel, float zVel)
{
	if (buffer >= AudioPlayer::buffersSE.size())
	{
		std::fprintf(stderr, "Error: Index out of bounds on SE buffers\n");
		return nullptr;
	}

	for (int i = 0; i < 14; i++)
	{
		Source* src = AudioPlayer::sources[i];
		if (!src->isPlaying())
		{
			src->setVolume(soundLevelSE);
			src->setLooping(loop);
			src->setPosition(pos->x, pos->y, pos->z);
			src->setPitch(pitch);
			src->setVelocity(xVel, yVel, zVel);
			src->play(AudioPlayer::buffersSE[buffer]);
			return src;
		}
	}

	//no sources to play music
	return nullptr;
}

Source* AudioPlayer::playBGM(int bufferLoop)
{
	if (bufferLoop >= (int)AudioPlayer::buffersBGM.size() || bufferLoop < 0)
	{
		std::fprintf(stderr, "Error: Index out of bounds on BGM buffers\n");
		return nullptr;
	}

	return AudioPlayer::playBGM(AudioPlayer::buffersBGM[bufferLoop]);
}

Source* AudioPlayer::playBGM(ALuint bufferLoop)
{
	Source* src = AudioPlayer::sources[14];
	src->stop();
	src->setLooping(false);

	alSourcei(src->getSourceID(), AL_BUFFER, AL_NONE);

	src->setLooping(true);
	src->setVolume(AudioPlayer::soundLevelBGM);

	AudioPlayer::bgmIntro = AL_NONE;
	AudioPlayer::bgmLoop = bufferLoop;

	src->play(AudioPlayer::bgmLoop);

	return src;
}

Source* AudioPlayer::playBGMWithIntro(int bufferIntro, int bufferLoop)
{
	if (bufferIntro >= (int)AudioPlayer::buffersBGM.size() ||
		bufferLoop  >= (int)AudioPlayer::buffersBGM.size() ||
		bufferIntro < 0 ||
		bufferLoop  < 0)
	{
		std::fprintf(stderr, "Error: Index out of bounds on BGM buffers\n");
		return nullptr;
	}

	return AudioPlayer::playBGMWithIntro(AudioPlayer::buffersBGM[bufferIntro], AudioPlayer::buffersBGM[bufferLoop]);
}

Source* AudioPlayer::playBGMWithIntro(ALuint bufferIntro, ALuint bufferLoop)
{
	Source* src = AudioPlayer::sources[14];
	src->stop();
	src->setVolume(AudioPlayer::soundLevelBGM);
	src->setLooping(false);


	alSourcei(src->getSourceID(), AL_BUFFER, AL_NONE); //Get rid of queued buffers 

	AudioPlayer::bgmIntro = bufferIntro;
	AudioPlayer::bgmLoop  = bufferLoop;

	alSourceQueueBuffers(src->getSourceID(), 1, &AudioPlayer::bgmIntro);
	alSourceQueueBuffers(src->getSourceID(), 1, &AudioPlayer::bgmLoop);
	alSourcePlay(src->getSourceID());

	//if (!src->isPlaying() || src->getLastPlayedBufferID() != AudioPlayer::buffersBGM[buffer])
	{
		//src->setLooping(true);
		//src->setVolume(AudioPlayer::soundLevelBGM);
		//src->play(AudioPlayer::buffersBGM[buffer]);
	}

	return src;
}

//Gets rid of the intro buffer, so that just the loop buffer loops
void AudioPlayer::refreshBGM()
{
	Source* src = AudioPlayer::sources[14];

	if (AudioPlayer::bgmIntro != AL_NONE)
	{
		ALint currentBufferIndex;
		alGetSourcei(src->getSourceID(), AL_BUFFERS_PROCESSED, &currentBufferIndex);
		if (currentBufferIndex == 1)
		{
			alSourceUnqueueBuffers(src->getSourceID(), 1, &AudioPlayer::bgmIntro);
			AudioPlayer::bgmIntro = AL_NONE;
			src->setLooping(true);
		}
	}
}

void AudioPlayer::stopBGM()
{
	Source* src = AudioPlayer::sources[14];
	src->stop();
	src->setLooping(false);

	alSourcei(src->getSourceID(), AL_BUFFER, AL_NONE); //Get rid of queued buffers 

	AudioPlayer::bgmIntro = AL_NONE;
	AudioPlayer::bgmLoop  = AL_NONE;
}

Source* AudioPlayer::getSource(int i)
{
	return AudioPlayer::sources[i];
}

ALuint AudioPlayer::getSEBuffer(int i)
{
	return AudioPlayer::buffersSE[i];
}

void AudioPlayer::setBGMVolume(float percent)
{
	Source* src = AudioPlayer::sources[14];
	src->setVolume(percent*soundLevelBGM);
}

void AudioPlayer::loadSettings()
{
	std::ifstream file("Settings/AudioSettings.ini");
	if (!file.is_open())
	{
		std::fprintf(stderr, "Error: Cannot load file 'Settings/AudioSettings.ini'\n");
		file.close();
	}
	else
	{
		std::string line;

		while (!file.eof())
		{
			getline(file, line);

			char lineBuf[512];
			memcpy(lineBuf, line.c_str(), line.size()+1);

			int splitLength = 0;
			char** lineSplit = split(lineBuf, ' ', &splitLength);

			if (splitLength == 2)
			{
				if (strcmp(lineSplit[0], "SFX_Volume") == 0)
				{
					AudioPlayer::soundLevelSE = std::stof(lineSplit[1], nullptr);
					AudioPlayer::soundLevelSE = fmaxf(0.0f, fminf(AudioPlayer::soundLevelSE, 1.0f));
				}
				else if (strcmp(lineSplit[0], "Music_Volume") == 0)
				{
					AudioPlayer::soundLevelBGM = std::stof(lineSplit[1], nullptr);
					AudioPlayer::soundLevelBGM = fmaxf(0.0f, fminf(AudioPlayer::soundLevelBGM, 1.0f));
				}
			}

			free(lineSplit);
		}
		file.close();
	}
}

void AudioPlayer::stopAllSFX()
{
	for (int i = 0; i < 14; i++)
	{
		if (AudioPlayer::sources[i]->isPlaying())
		{
			AudioPlayer::sources[i]->stop();
		}
	}
}