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
#include "../toolbox/getline.h"



float AudioPlayer::soundLevelSE = 0.05f;
float AudioPlayer::soundLevelBGM = 0.05f;
std::vector<Source*> AudioPlayer::sources;
std::vector<ALuint> AudioPlayer::buffersSE;
std::vector<ALuint> AudioPlayer::buffersBGM;
ALuint AudioPlayer::bgmIntro;
ALuint AudioPlayer::bgmLoop;


void AudioPlayer::loadSoundEffects()
{
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/BigDestroy.ogg").c_str()));           //0
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Dashpad.ogg").c_str()));              //1
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Goal.ogg").c_str()));                 //2
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/ItemCapsule.ogg").c_str()));          //3
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Ring.ogg").c_str()));                 //4
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Splash.ogg").c_str()));               //5
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Spring.ogg").c_str()));               //6
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/UnlockSomething.ogg").c_str()));      //7
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Bounce.ogg").c_str()));                //8
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Death.ogg").c_str()));                 //9
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/GetHit.ogg").c_str()));                //10
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/HomingAttack.ogg").c_str()));          //11
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Jump.ogg").c_str()));                  //12
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Skid.ogg").c_str()));                  //13
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/SpindashCharge.ogg").c_str()));        //14
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/SpindashRelease.ogg").c_str()));       //15
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/StompInit.ogg").c_str()));             //16
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/StompLand.ogg").c_str()));             //17
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/CantStick.ogg").c_str()));             //18
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/DockBreak.ogg").c_str()));            //19
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Seagull.ogg").c_str()));              //20
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/SpeedRamp.ogg").c_str()));            //21
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/SpeedHighway/CranePlatform.ogg").c_str()));   //22
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/SpeedHighway/ElevatorPlatform.ogg").c_str()));//23
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/ActClear.ogg").c_str()));             //24
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/ExitStage.ogg").c_str()));            //25
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/CapsuleClick.ogg").c_str()));         //26
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Fly.ogg").c_str()));                   //27
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/FlyTired.ogg").c_str()));              //28
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Climb.ogg").c_str()));                 //29
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Dig.ogg").c_str()));                   //30
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/DrillDive.ogg").c_str()));             //31
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Glide.ogg").c_str()));                 //32
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Punch.ogg").c_str()));                 //33
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/RadarPing.ogg").c_str()));            //34
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/1Up.ogg").c_str()));                  //35
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/MenuBleep.ogg").c_str()));            //36
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/MenuWoosh.ogg").c_str()));            //37
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/MenuAccept.ogg").c_str()));           //38
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/PeelCharge.ogg").c_str()));            //39
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/PeelRelease.ogg").c_str()));           //40
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Player/Roll.ogg").c_str()));                  //41
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Snowhead/BabyGoron_Doze.ogg").c_str()));      //42
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Dig.ogg").c_str()));                  //43
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/RankShow.ogg").c_str()));             //44
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/Checkpoint.ogg").c_str()));           //45
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/BreakDamage.ogg").c_str()));             //46
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/ChaosSpear.ogg").c_str()));              //47
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/Damage.ogg").c_str()));                  //48
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/Electric.ogg").c_str()));                //49
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/MetallicExplode.ogg").c_str()));         //50
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/RunFastLoop.ogg").c_str()));             //51
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/ChargeShot.ogg").c_str()));              //52
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/Boss/LaserLoop.ogg").c_str()));               //53
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/RocketAppear.ogg").c_str()));         //54
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/RocketTakeOffStart.ogg").c_str()));   //55
	AudioPlayer::buffersSE.push_back(AudioMaster::loadOGG((Global::pathToEXE+"res/Audio/SFX/General/RocketTakeOffLoop.ogg").c_str()));    //56
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
		delete src; INCR_DEL("Source");
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
	sources.push_back(new Source(1, 100, 600)); INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	sources.push_back(new Source(1, 100, 600));	INCR_NEW("Source");
	//Last source is dedicated to background music
	sources.push_back(new Source(0, 0, 0));     INCR_NEW("Source");
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
	if (buffer >= (int)AudioPlayer::buffersSE.size())
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
	std::ifstream file(Global::pathToEXE+"Settings/AudioSettings.ini");
	if (!file.is_open())
	{
		std::fprintf(stderr, "Error: Cannot load file '%s'\n", (Global::pathToEXE+"Settings/AudioSettings.ini").c_str());
		file.close();
	}
	else
	{
		std::string line;

		while (!file.eof())
		{
			getlineSafe(file, line);

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
