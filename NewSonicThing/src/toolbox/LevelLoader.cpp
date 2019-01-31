#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <AL/al.h>
#include <list>

#include <chrono>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "levelloader.h"
#include "../engineTester/main.h"
#include "../entities/stage.h"
#include "../entities/stagepass2.h"
#include "../entities/stagepass3.h"
#include "../entities/stagetransparent.h"
#include "../collision/collisionchecker.h"
#include "../collision/collisionmodel.h"
#include "../objLoader/objLoader.h"
#include "../audio/audioplayer.h"
#include "../entities/skysphere.h"
#include "../guis/guimanager.h"
#include "../renderEngine/skymanager.h"
#include "../toolbox/mainmenu.h"
#include "../toolbox/getline.h"
#include "split.h"
#include "input.h"
#include "../entities/car.h"
#include "../entities/boostpad.h"
#include "../entities/camera.h"
#include "../entities/checkpoint.h"
#include "../entities/jumpramp.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../entities/GreenForest/gfstagemanager.h"
#include "../entities/MetalHarbor/mhtank.h"
#include "../entities/MetalHarbor/mhrocketbase.h"
#include "../entities/MetalHarbor/mhgiantrocket.h"
#include "../entities/MetalHarbor/mhpathflat.h"
#include "../entities/MetalHarbor/mhpathdiagonal.h"
#include "../entities/MetalHarbor/mhpathflatsmall.h"
#include "../entities/rail.h"
#include "../entities/ring.h"
#include "../entities/dashpad.h"
#include "../entities/speedramp.h"
#include "../entities/point.h"

void LevelLoader::loadTitle()
{
	Stage::deleteModels();
	StagePass2::deleteStaticModels();
	StagePass3::deleteStaticModels();
	StageTransparent::deleteStaticModels();

	Global::levelName = "";

	freeAllStaticModels();

	CollisionChecker::deleteAllCollideModels();

	//SkyManager::setCenterObject(nullptr);
	Global::gameMainVehicle = nullptr;

	Main_deleteAllEntites();
	Main_deleteAllEntitesPass2();
	Main_deleteAllEntitesPass3();
	Main_deleteAllTransparentEntites();

	for (Checkpoint* check : Global::gameCheckpointList)
	{
		delete check; INCR_DEL
	}
	Global::gameCheckpointList.clear();

	AudioPlayer::stopBGM();
	AudioPlayer::deleteBuffersBGM();

	//Global::gameSkySphere->setVisible(false);

	Global::finishStageTimer = -1;
	Global::raceStartTimer = -1;

	Global::stageUsesWater = true;

	Global::gameRingCount = 0;
	Global::gameScore = 0;
	Global::gameLives = 4;
	GuiManager::setTimer(0);
	GuiManager::stopTimer();

	GuiManager::clearGuisToRender();

	MainMenu::loadResources();
	Global::gameState = STATE_TITLE;
	Global::gameIsNormalMode = false;
	Global::gameIsHardMode = false;
	Global::gameIsChaoMode = false;
	Global::gameIsRingMode = false;

	Global::spawnAtCheckpoint  = false;
	Global::checkpointX        = 0;
	Global::checkpointY        = 0;
	Global::checkpointZ        = 0;
	Global::checkpointRotY     = 0;
	Global::checkpointCamYaw   = 0;
	Global::checkpointCamPitch = 0;
	Global::checkpointTimeCen  = 0;
	Global::checkpointTimeSec  = 0;
	Global::checkpointTimeMin  = 0;
}

void LevelLoader::loadLevel(std::string levelFilename)
{
	std::string fname = levelFilename;

	if (fname == "")
	{
		return;
	}

	int stageFault = 0;

	if (!Global::isNewLevel)
	{
		if (Global::gameIsArcadeMode)
		{
			Global::gameLives--;
			if (Global::gameLives < 0)
			{
				LevelLoader::loadTitle();
				return;
			}
		}
	}

	if (Global::isNewLevel)
	{
		stageFault = 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		Global::spawnAtCheckpoint  = false;
		Global::checkpointX        = 0;
		Global::checkpointY        = 0;
		Global::checkpointZ        = 0;
		Global::checkpointRotY     = 0;
		Global::checkpointCamYaw   = 0;
		Global::checkpointCamPitch = 0;
		Global::checkpointTimeCen  = 0;
		Global::checkpointTimeSec  = 0;
		Global::checkpointTimeMin  = 0;
	}
	Global::isNewLevel = false;

	if (stageFault == 1)
	{
		freeAllStaticModels();
	}

	//SkyManager::setCenterObject(nullptr);
	Global::gameMainVehicle = nullptr;

	Main_deleteAllEntites();
	Main_deleteAllEntitesPass2();
	Main_deleteAllEntitesPass3();
	Main_deleteAllTransparentEntites();

	for (Checkpoint* check : Global::gameCheckpointList)
	{
		delete check; INCR_DEL
	}
	Global::gameCheckpointList.clear();

	if (stageFault == 1)
	{
		Stage::deleteModels(); //Only delete stage if loading a new stage
		StagePass2::deleteStaticModels();
		StagePass3::deleteStaticModels();
		StageTransparent::deleteStaticModels();
	}

	std::ifstream file("res/Levels/" + fname);
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file '%s'\n", ("res/Levels/" + fname).c_str());
		file.close();
		return;
	}

	AudioPlayer::stopBGM();
	//Delete existing bgm if loading a new stage
	if (stageFault == 1)
	{
		AudioPlayer::deleteBuffersBGM();
	}

	//Global::stageUsesWater = false;

	//Run through the header content

	std::string modelFLoc;
	getlineSafe(file, modelFLoc);

	std::string modelFName;
	getlineSafe(file, modelFName);

	std::string colFLoc;
	getlineSafe(file, colFLoc);

	std::string numChunksLine;
	getlineSafe(file, numChunksLine);

	int numChunks = stoi(numChunksLine);

	if (stageFault == 1) //We need to load in new collision
	{
		CollisionChecker::deleteAllCollideModels();

		while (numChunks > 0)
		{
			std::string line;
			getlineSafe(file, line);

			char lineBuf[128];
			memcpy(lineBuf, line.c_str(), line.size()+1);

			int splitLength = 0;
			char** lineSplit = split(lineBuf, ' ', &splitLength);



			CollisionModel* colModel = loadBinaryCollisionModel("Models/" + colFLoc + "/", lineSplit[0]);
			colModel->generateQuadTree(std::stoi(lineSplit[1]));
			CollisionChecker::addCollideModel(colModel);

			numChunks--;

			free(lineSplit);
		}
	}
	else //Keep the same quad tree collision
	{
		CollisionChecker::deleteAllCollideModelsExceptQuadTrees();

		while (numChunks > 0)
		{
			std::string line;
			getlineSafe(file, line);

			numChunks--;
		}
	}

	std::string sunColorDay;
	getlineSafe(file, sunColorDay);
	{
		char lineBuf[128];
		memcpy(lineBuf, sunColorDay.c_str(), sunColorDay.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		Vector3f newSunColour(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));
		SkyManager::setSunColorDay(&newSunColour);

		free(dat);
	}

	std::string sunColorNight;
	getlineSafe(file, sunColorNight);
	{
		char lineBuf[128];
		memcpy(lineBuf, sunColorNight.c_str(), sunColorNight.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		Vector3f newSunColour(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));
		SkyManager::setSunColorNight(&newSunColour);

		free(dat);
	}

	std::string moonColorDay;
	getlineSafe(file, moonColorDay);
	{
		char lineBuf[128];
		memcpy(lineBuf, moonColorDay.c_str(), moonColorDay.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		Vector3f newMoonColour(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));
		SkyManager::setMoonColorDay(&newMoonColour);

		free(dat);
	}

	std::string moonColorNight;
	getlineSafe(file, moonColorNight);
	{
		char lineBuf[128];
		memcpy(lineBuf, moonColorNight.c_str(), moonColorNight.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		Vector3f newMoonColour(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));
		SkyManager::setMoonColorNight(&newMoonColour);

		free(dat);
	}


	Vector3f fogDay;
	Vector3f fogNight;

	std::string fogColorDay;
	getlineSafe(file, fogColorDay);
	{
		char lineBuf[128];
		memcpy(lineBuf, fogColorDay.c_str(), fogColorDay.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		fogDay.set(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));

		free(dat);
	}

	std::string fogColorNight;
	getlineSafe(file, fogColorNight);
	{
		char lineBuf[128];
		memcpy(lineBuf, fogColorNight.c_str(), fogColorNight.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		fogNight.set(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));

		free(dat);
	}

	SkyManager::setFogColours(&fogDay, &fogNight);

	std::string fogVars;
	getlineSafe(file, fogVars);
	{
		char lineBuf[128];
		memcpy(lineBuf, fogVars.c_str(), fogVars.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		SkyManager::setFogVars(toFloat(dat[0]), toFloat(dat[1]));

		free(dat);
	}

	std::string timeOfDay;
	getlineSafe(file, timeOfDay);
	if (stageFault == 1)
	{
		char lineBuf[128];
		memcpy(lineBuf, timeOfDay.c_str(), timeOfDay.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		SkyManager::setTimeOfDay(toFloat(dat[0]));

		free(dat);
	}

	//Global::gameSkySphere->setVisible(false);


	std::string camOrientation;
	getlineSafe(file, camOrientation);
	{
		char lineBuf[128];
		memcpy(lineBuf, camOrientation.c_str(), camOrientation.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

		//Global::gameCamera->setYaw(toFloat(dat[0]));
		//Global::gameCamera->setPitch(toFloat(dat[1]));

		free(dat);
	}




	//Read in BGM

	std::string bgmHasLoopLine;
	getlineSafe(file, bgmHasLoopLine);

	int bgmHasLoop = stoi(bgmHasLoopLine);


	std::string numBGMLine;
	getlineSafe(file, numBGMLine);

	int numBGM = stoi(numBGMLine);

	while (numBGM > 0)
	{
		std::string line;
		getlineSafe(file, line);

		char* bgmFileName = (char*)line.c_str();

		if (stageFault == 1)
		{
			AudioPlayer::loadBGM(bgmFileName);
		}

		numBGM--;
	}


	//Finish the level positions and cam settings
	char finishBuf[512];

	std::string finishPosition;
	getlineSafe(file, finishPosition);
	memcpy(finishBuf, finishPosition.c_str(), finishPosition.size()+1);
	int finishLength = 0;
	char** finishSplit = split(finishBuf, ' ', &finishLength);
	Global::gameStage->finishPlayerPosition.x = toFloat(finishSplit[0]);
	Global::gameStage->finishPlayerPosition.y = toFloat(finishSplit[1]);
	Global::gameStage->finishPlayerPosition.z = toFloat(finishSplit[2]);
	free(finishSplit);

	std::string finishCamVars;
	getlineSafe(file, finishCamVars);
	memcpy(finishBuf, finishCamVars.c_str(), finishCamVars.size()+1);
	finishSplit = split(finishBuf, ' ', &finishLength);
	Global::gameStage->finishPlayerRotY  = toFloat(finishSplit[0]);
	Global::gameStage->finishCameraPitch = toFloat(finishSplit[1]);
	free(finishSplit);

	//Global death height
	std::string deathHeightLine;
	getlineSafe(file, deathHeightLine);
	Global::deathHeight = stof(deathHeightLine);

	//Does the stage have water?
	std::string waterEnabledLine;
	getlineSafe(file, waterEnabledLine);
	Global::stageUsesWater = false;
	if (waterEnabledLine == "water")
	{
		Global::stageUsesWater = true;
	}

	//Global water height
	std::string waterHeightLine;
	getlineSafe(file, waterHeightLine);
	Global::waterHeight = stof(waterHeightLine);

	GuiManager::clearGuisToRender();

	Global::gameIsNormalMode = false;
	Global::gameIsHardMode = false;
	Global::gameIsChaoMode = false;
	Global::gameIsRingMode = false;

	Level* currentLevel = &Global::gameLevelData[Global::levelID];
	std::string missionType = (currentLevel->missionData[Global::gameMissionNumber])[0];

	if (missionType == "Normal") Global::gameIsNormalMode = true;
	if (missionType == "Ring")   Global::gameIsRingMode   = true;
	if (missionType == "Chao")   Global::gameIsChaoMode   = true;
	if (missionType == "Hard")   Global::gameIsHardMode   = true;

	if (Global::gameIsRingMode)
	{
		Global::gameRingTarget = std::stoi((currentLevel->missionData[Global::gameMissionNumber])[2]);
	}

	//Now read through all the objects defined in the file

	std::string line;

	while (!file.eof())
	{
		getlineSafe(file, line);

		char lineBuf[512]; //Buffer to copy line into
		memcpy(lineBuf, line.c_str(), line.size()+1);

		int splitLength = 0;
		char** lineSplit = split(lineBuf, ' ', &splitLength);

		if (splitLength > 0)
		{
			Input::pollInputs();

			processLine(lineSplit, splitLength);
		}
		free(lineSplit);
	}
	file.close();

	if (stageFault == 1)
	{
		Stage::loadModels((char*)modelFLoc.c_str(), (char*)modelFName.c_str());
	}

	if (Global::gameMainVehicle != nullptr)
	{
		Global::gameMainVehicle->setCanMoveTimer(1.0f);
		//Global::bufferTime = 60;
	}

	Global::raceStartTimer = -1.0f;


	Global::gameRingCount = 0;
	Global::gameScore = 0;
	GuiManager::setTimer(0);
	GuiManager::stopTimer();
	GuiManager::startTimer();

	int maxNumber = -1;
	for (Checkpoint* check : Global::gameCheckpointList)
	{
		if (check->ID > maxNumber)
		{
			maxNumber = check->ID;
		}
	}
	Global::gameCheckpointLast = maxNumber;

	if (Global::spawnAtCheckpoint)
	{
		//GuiManager::setTimer(Global::checkpointTimeMin, Global::checkpointTimeSec, Global::checkpointTimeCen);
	}
	else
	{
		if (bgmHasLoop != 0)
		{
			//By default, first 2 buffers are the intro and loop, respectively
			AudioPlayer::playBGMWithIntro(0, 1);
		}
		else
		{
			AudioPlayer::playBGM(0);
		}
	}

	//GuiManager::addGuiToRender(GuiTextureResources::textureRing);

	Global::finishStageTimer = -1;

	//Vector3f partVel(0, 0, 0);
	//new Particle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), 1.0f, 400.0f, false);

	Global::gameState = STATE_RUNNING;

	glfwSetTime(0);
	extern double timeOld;
	timeOld = 0.0;
}


void LevelLoader::processLine(char** dat, int /*datLength*/)
{
	if (dat[0][0] == '#')
	{
		return;
	}

	int id = std::stoi(dat[0]);

	switch (id)
	{
		case 0: //Ring
		{
			Ring::loadStaticModels();
			Ring* ring = new Ring(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW
			Main_addEntity(ring);
			return;
		}

		case 2: //Stage Pass 2
		{
			StagePass2* pass2 = new StagePass2(dat[1], dat[2]); INCR_NEW
			Main_addEntityPass2(pass2);
			return;
		}

		case 3: //Stage Pass 3
		{
			StagePass3* pass3 = new StagePass3(dat[1], dat[2]); INCR_NEW
			Main_addEntityPass3(pass3);
			return;
		}

		case 4: //Stage Transparent
		{
			StageTransparent* trans = new StageTransparent(dat[1], dat[2]); INCR_NEW
			Main_addTransparentEntity(trans);
			return;
		}

		case 6: //Car
		{
			Car* car = new Car(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW
			Global::gameMainVehicle = car;
			Main_addEntity(car);
			return;
		}

		case 7: //Sky Sphere
		{
			SkySphere::loadModels(dat[1], dat[2], dat[3]);
			Global::gameSkySphere->setScale(toFloat(dat[4]));
			Global::gameSkySphere->setFollowsY((bool)toInt(dat[5]));
			Global::gameSkySphere->setVisible(true);
			return;
		}

		case 8: //Boostpad
		{
			Boostpad::loadStaticModels();
			Boostpad* pad = new Boostpad(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
				toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]),
				toFloat(dat[7]), toFloat(dat[8]), toFloat(dat[9])); INCR_NEW
			Main_addTransparentEntity(pad);
			return;
		}

		case 10: //Checkpoint
		{
			Checkpoint::loadStaticModels();
			Checkpoint* checkpoint = new Checkpoint(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
				toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]),
				toFloat(dat[7]), toInt(dat[8])); INCR_NEW
			Global::gameCheckpointList.push_back(checkpoint);
			return;
		}

		case 11: //JumpPad
		{
			JumpRamp::loadStaticModels();
			JumpRamp* ramp = new JumpRamp(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
				toFloat(dat[4])); INCR_NEW
			Main_addTransparentEntity(ramp);
			return;
		}

		case 91: //General Purpose Stage Manager
		{
			int id2 = std::stoi(dat[1]);
			switch (id2)
			{
				case 0:
				{
					GFStageManager::loadStaticModels();
					GFStageManager* gf = new GFStageManager; INCR_NEW
					Main_addEntity(gf);
					break;
				}

				//case 1:
				//	G* mh = new MH_Manager; INCR_NEW
				//	Main_addEntity(mh);
				//	break;

				default: break;
			}
			return;
		}

		case 92: //Rail
		{
			Rail* rail = new Rail(dat[1]); INCR_NEW
			Main_addEntity(rail);
			return;
		}

		case 93: //Metal Harbor Objects
		{
			int id2 = std::stoi(dat[1]);
			switch (id2)
			{
				case 0: //Tank
				{
					MH_Tank::loadStaticModels();
					MH_Tank* tank = new MH_Tank(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW //x, y, z
					Main_addEntity(tank);
					break;
				}
				case 1: //RocketBase
				{
					MH_RocketBase::loadStaticModels();
					MH_RocketBase* rocketBase = new MH_RocketBase(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW //x, y, z
					Main_addEntity(rocketBase);
					break;
				}
				case 2: //GiantRocket
				{
					MH_GiantRocket::loadStaticModels();
					MH_GiantRocket* giantRocket = new MH_GiantRocket(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW //x, y, z
					Main_addEntity(giantRocket);
					break;
				}
				case 3: //PathFlat
				{
					MH_PathFlat::loadStaticModels();
					MH_PathFlat* pathFlat = new MH_PathFlat(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), toFloat(dat[5])); INCR_NEW //x, y, z, rotY
					Main_addEntity(pathFlat);
					break;
				}case 4: //PathDiagonal
				{
					MH_PathDiagonal::loadStaticModels();
					MH_PathDiagonal* pathDiagonal = new MH_PathDiagonal(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), toFloat(dat[5])); INCR_NEW //x, y, z, rotY
					Main_addEntity(pathDiagonal);
					break;
				}
				case 5: //PathFlatSmall
				{
					MH_PathFlatSmall::loadStaticModels();
					MH_PathFlatSmall* pathFlatSmall = new MH_PathFlatSmall(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), toFloat(dat[5])); INCR_NEW //x, y, z, rotY
					Main_addEntity(pathFlatSmall);
					break;
				}
				default: break;
			}
			return;
		}
		
		case 94: //Dashpad
		{
			Dashpad::loadStaticModels();
			Dashpad* dashpad = new Dashpad(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), 			//position
				toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]), 			//rotation
				toFloat(dat[7]), toFloat(dat[8]), toFloat(dat[9])); INCR_NEW  //power, camYaw, time 
			
			Main_addEntity(dashpad);
			return;
		}

		case 95: //Speed Ramp
		{
			SpeedRamp::loadStaticModels();
			SpeedRamp* ramp = new SpeedRamp(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
				toFloat(dat[4]), toFloat(dat[5]),                  //rotation
				toFloat(dat[6]), toFloat(dat[7])); INCR_NEW          //power, time
			Main_addEntity(ramp);
			return;
		}

		case 96: //Point (for paths)
		{
			Point* point = new Point(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
				toInt(dat[4])); //point id

			INCR_NEW
			Main_addEntity(point);
			return;
		}
		//case 91: //Metal Harbor
		//{
		//	MH_Manager* mh = new MH_Manager;
		//	INCR_NEW
		//	Main_addEntity(mh);
		//	return;
		//}

		default:
		{
			return;
		}
	}
}

void LevelLoader::loadLevelData()
{
	Global::gameLevelData.clear();

	std::ifstream file("res/Levels/LevelData.dat");
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file 'res/Levels/LevelData.dat'\n");
		file.close();
	}
	else
	{
		std::string line;
		getlineSafe(file, line);

		int levelCount = std::stoi(line.c_str());
		getlineSafe(file, line);

		while (levelCount > 0)
		{
			Level newLevel;

			getlineSafe(file, line);
			newLevel.fileName = line;

			getlineSafe(file, line);
			newLevel.displayName = line;

			getlineSafe(file, line);
			int missionCount = std::stoi(line);
			newLevel.numMissions = missionCount;

			while (missionCount > 0)
			{
				std::vector<std::string> missionData;

				getlineSafe(file, line);

				char lineBuf[256];
				memcpy(lineBuf, line.c_str(), line.size()+1);

				int splitLength = 0;
				char** lineSplit = split(lineBuf, ';', &splitLength);

				for (int i = 0; i < splitLength; i++)
				{
					missionData.push_back(lineSplit[i]);
				}

				free(lineSplit);

				newLevel.missionData.push_back(missionData);

				missionCount--;
			}

			Global::gameLevelData.push_back(newLevel);

			getlineSafe(file, line);

			levelCount--;
		}

		file.close();
	}
}

float LevelLoader::toFloat(char* input)
{
	return std::stof(input);
}

int LevelLoader::toInt(char* input)
{
	return std::stoi(input);
}

void LevelLoader::freeAllStaticModels()
{
	SkySphere::deleteModels();
	Boostpad::deleteStaticModels();
	Car::deleteStaticModels();
	Checkpoint::deleteStaticModels();
	JumpRamp::deleteStaticModels();
	GFStageManager::deleteStaticModels();
	Ring::deleteStaticModels();
	MH_GiantRocket::deleteStaticModels();
	MH_PathDiagonal::deleteStaticModels();
	MH_PathFlat::deleteStaticModels();
	MH_PathFlatSmall::deleteStaticModels();
	MH_RocketBase::deleteStaticModels();
	MH_Tank::deleteStaticModels();
}
