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
#include "../menu/missionmenu.h"
#include "../toolbox/getline.h"
#include "../toolbox/maths.h"
#include "split.h"
#include "input.h"
#include "../entities/controllableplayer.h"
#include "../entities/dashpad.h"
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
#include "../menu/menumanager.h"
#include "../entities/SkyRail/srstagemanager.h"
#include "../entities/goalring.h"
#include "../entities/point.h"
#include "../entities/rocket.h"
#include "../entities/MetalHarbor/mhstagemanager.h"
#include "../entities/GreenHill/ghstagemanager.h"
#include "../entities/beetle.h"
#include "../entities/CityEscape/cestagemanager.h"
#include "../entities/spring.h"
#include "../water/watertile.h"
#include "../entities/playersonic.h"
#include "../entities/DelfinoPlaza/dpstagemanager.h"
#include "../entities/raceghost.h"
#include "../entities/help.h"
#include "../entities/Tutorial/tstagemanager.h"

int LevelLoader::numLevels = 0;

void LevelLoader::loadTitle()
{
	Stage::deleteModels();
	StagePass2::deleteStaticModels();
	StagePass3::deleteStaticModels();
	StageTransparent::deleteStaticModels();

	Global::levelName = "";

	freeAllStaticModels();

	CollisionChecker::deleteAllCollideModels();

    if (Global::gameMainPlayer != nullptr)
    {
        delete Global::gameMainPlayer; INCR_DEL("Entity");
    }
	Global::gameMainPlayer = nullptr;

	Main_deleteAllEntites();
	Main_deleteAllEntitesPass2();
	Main_deleteAllEntitesPass3();
	Main_deleteAllTransparentEntites();
	Main_deleteAllChunkedEntities();

	for (Checkpoint* check : Global::gameCheckpointList)
	{
		delete check; INCR_DEL("Entity");
	}
	Global::gameCheckpointList.clear();

	AudioPlayer::stopBGM();
	AudioPlayer::deleteBuffersBGM();

	Global::finishStageTimer = -1;
	Global::raceStartTimer = -1;

	Global::stageUsesWater = true;

	Global::gameRingCount = 0;
	Global::gameScore = 0;
	Global::gameLives = 4;

	GuiManager::clearGuisToRender();

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

    //use vsync on the title screen
    glfwSwapInterval(1); 
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

    if (Global::gameMainPlayer != nullptr)
    {
        delete Global::gameMainPlayer; INCR_DEL("Entity");
    }
	Global::gameMainPlayer = nullptr;

	Main_deleteAllEntites();
	Main_deleteAllEntitesPass2();
	Main_deleteAllEntitesPass3();
	Main_deleteAllTransparentEntites();
	Main_deleteAllChunkedEntities();

	for (Checkpoint* check : Global::gameCheckpointList)
	{
		delete check; INCR_DEL("Entity");
	}
	Global::gameCheckpointList.clear();

	if (stageFault == 1)
	{
		Stage::deleteModels(); //Only delete stage if loading a new stage
		StagePass2::deleteStaticModels();
		StagePass3::deleteStaticModels();
		StageTransparent::deleteStaticModels();
	}

	std::ifstream file(Global::pathToEXE + "res/Levels/" + fname);
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/Levels/" + fname).c_str());
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

	std::string numLevelChunksLine;
	getlineSafe(file, numLevelChunksLine);

	int numLevelChunks = stoi(numLevelChunksLine);

	if (stageFault == 1) //We need to load in new level chunks
	{
		Stage::deleteModels();

		std::vector<std::string> fnames;
		std::vector<std::vector<Vector3f>> mins;
		std::vector<std::vector<Vector3f>> maxs;

		while (numLevelChunks > 0)
		{
			std::string line;
			getlineSafe(file, line);

			char lineBuf[256];
			memcpy(lineBuf, line.c_str(), line.size()+1);
			int splitLength = 0;
			char** lineSplit = split(lineBuf, ' ', &splitLength);
			
			fnames.push_back(lineSplit[0]);

			std::vector<Vector3f> minList;
			std::vector<Vector3f> maxList;

			for (int c = 1; c < splitLength; c+=6)
			{
				Vector3f min(std::stof(lineSplit[c+0]), std::stof(lineSplit[c+1]), std::stof(lineSplit[c+2]));
				Vector3f max(std::stof(lineSplit[c+3]), std::stof(lineSplit[c+4]), std::stof(lineSplit[c+5]));
				minList.push_back(min);
				maxList.push_back(max);
			}

			mins.push_back(minList);
			maxs.push_back(maxList);

			numLevelChunks--;

			free(lineSplit);
		}

		Stage::loadModels(modelFLoc.c_str(), &fnames, &mins, &maxs);
	}
	else //Keep the same level chunks
	{
		while (numLevelChunks > 0)
		{
			std::string line;
			getlineSafe(file, line);

			numLevelChunks--;
		}
	}
	Stage::respawnChunks();

	//std::string modelFName;
	//getlineSafe(file, modelFName);

	std::string colFLoc;
	getlineSafe(file, colFLoc);

	std::string numCollChunksLine;
	getlineSafe(file, numCollChunksLine);

	int numCollChunks = stoi(numCollChunksLine);

	if (stageFault == 1) //We need to load in new collision
	{
		CollisionChecker::deleteAllCollideModels();

		while (numCollChunks > 0)
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

			numCollChunks--;

			free(lineSplit);
		}
	}
	else //Keep the same quad tree collision
	{
		CollisionChecker::deleteAllCollideModelsExceptQuadTrees();

		while (numCollChunks > 0)
		{
			std::string line;
			getlineSafe(file, line);

			numCollChunks--;
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

    Vector3f initialCamDir;
	std::string camOrientation;
	getlineSafe(file, camOrientation);
	{
		char lineBuf[128];
		memcpy(lineBuf, camOrientation.c_str(), camOrientation.size()+1);
		int splitLength = 0;
		char** dat = split(lineBuf, ' ', &splitLength);

        initialCamDir = Vector3f(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));
        initialCamDir.normalize();

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

    //Number of water tiles
	std::string waterTilesCountLine;
	getlineSafe(file, waterTilesCountLine);
    int waterTilesCount = stoi(waterTilesCountLine);
    for (WaterTile* tile : Global::gameWaterTiles)
    {
        delete tile; INCR_DEL("WaterTile")
    }
    Global::gameWaterTiles.clear();
    float waterTileOffset = -WaterTile::TILE_SIZE*(waterTilesCount-1);
    for (int r = 0; r < waterTilesCount; r++)
	{
		for (int c = 0; c < waterTilesCount; c++)
		{
			Global::gameWaterTiles.push_back(new WaterTile(r*WaterTile::TILE_SIZE*2 + waterTileOffset, c*WaterTile::TILE_SIZE*2 + waterTileOffset)); INCR_NEW("WaterTile");
		}
	}

    //Does the stage use backface culling?
    std::string backfaceCullingLine;
	getlineSafe(file, backfaceCullingLine);
    Global::renderWithCulling = (backfaceCullingLine == "true");

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

	std::list<Entity*> chunkedEntities;

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

			processLine(lineSplit, splitLength, &chunkedEntities);
		}
		free(lineSplit);
	}
	file.close();

	//sort the chunked entity stuff
	if (chunkedEntities.size() > 0)
	{
		bool first = true;
		//calc min and max values
		float minX = 0;
		float maxX = 0;
		float minZ = 0;
		float maxZ = 0;
		for (Entity* e : chunkedEntities)
		{
			if (first)
			{
				first = false;
				minX = e->getX();
				maxX = e->getX();
				minZ = e->getZ();
				maxZ = e->getZ();
				continue;
			}
			minX = std::fminf(minX, e->getX());
			maxX = std::fmaxf(maxX, e->getX());
			minZ = std::fminf(minZ, e->getZ());
			maxZ = std::fmaxf(maxZ, e->getZ());
		}

		Global::recalculateEntityChunks(minX, maxX, minZ, maxZ, 1000);

		for (Entity* e : chunkedEntities)
		{
			Main_addChunkedEntity(e);
		}
	}
	else //not many chunked entities, dont bother
	{
		for (Entity* e : chunkedEntities)
		{
			Main_addEntity(e);
		}
	}

	if (stageFault == 1)
	{
		//Stage::loadModels((char*)modelFLoc.c_str(), (char*)modelFName.c_str()); //doing this earlier now
	}

	if (Global::gameMainPlayer != nullptr)
	{
		Global::gameMainPlayer->canMoveTimer = 1.0f;
        Global::gameMainPlayer->camDir.set(&initialCamDir);
        Global::gameMainPlayer->camDirSmooth.set(&initialCamDir);
	}

	Global::raceStartTimer = 1.0f;


	Global::gameRingCount = 0;
	Global::gameScore = 0;

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

	Global::finishStageTimer = -1.0f;

    Global::raceLog.clear();

	//Vector3f partVel(0, 0, 0);
	//new Particle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &partVel, 0.0f,
	//	1.0f, 0.0f, 50.0f, 0.0f, true, false);

	Global::gameState = STATE_RUNNING;

	glfwSetTime(0);
	extern double timeOld;
	//extern double previousTime;
	timeOld = 0.0;
	//previousTime = 0.0;

    //unlock framerate during gameplay
    if (Global::framerateUnlock)
    {
        glfwSwapInterval(0);
    }
}


void LevelLoader::processLine(char** dat, int datLength, std::list<Entity*>* chunkedEntities)
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
			Ring* ring = new Ring(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW("Entity");
			chunkedEntities->push_back(ring);
			return;
		}

		case 2: //Stage Pass 2
		{
			StagePass2* pass2 = new StagePass2(dat[1], dat[2]); INCR_NEW("Entity");
			Main_addEntityPass2(pass2);
			return;
		}

		case 3: //Stage Pass 3
		{
			StagePass3* pass3 = new StagePass3(dat[1], dat[2]); INCR_NEW("Entity");
			Main_addEntityPass3(pass3);
			return;
		}

		case 4: //Stage Transparent
		{
			StageTransparent* trans = new StageTransparent(dat[1], dat[2]); INCR_NEW("Entity");
			Main_addTransparentEntity(trans);
			return;
		}

		case 6: //Sonic
		{
			PlayerSonic* player = new PlayerSonic(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW("Entity");
            if (Global::gameMainPlayer != nullptr)
            {
                delete Global::gameMainPlayer; INCR_DEL("Entity");
            }
			Global::gameMainPlayer = player;
			//Main_addEntity(car);
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
			Dashpad::loadStaticModels();
			Dashpad* pad = new Dashpad(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
				toFloat(dat[4]), toFloat(dat[5]),
				toFloat(dat[6]), toFloat(dat[7]), toFloat(dat[8])); INCR_NEW("Entity");
			chunkedEntities->push_back(pad);
			return;
		}

		case 10: //Checkpoint
		{
			//Checkpoint::loadStaticModels();
			//Checkpoint* checkpoint = new Checkpoint(
			//	toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
			//	toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]),
			//	toFloat(dat[7]), toInt(dat[8])); INCR_NEW("Entity");
			//Global::gameCheckpointList.push_back(checkpoint);
			return;
		}

		case 11: //JumpPad
		{
			JumpRamp::loadStaticModels();
			JumpRamp* ramp = new JumpRamp(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
				toFloat(dat[4])); INCR_NEW("Entity");
			Main_addTransparentEntity(ramp);
			return;
		}

        case 12: //Spring
        {
            Spring::loadStaticModels();
			Spring* spring = new Spring(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
				toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]), //rotation direction
				toFloat(dat[7]), toFloat(dat[8]));                 //power, time
			INCR_NEW("Entity");
			chunkedEntities->push_back(spring);
			return;
        }

        case 28: //Beetle
		{
			Beetle::loadStaticModels();
			Beetle* beetle = new Beetle(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW("Entity");
			chunkedEntities->push_back(beetle);
			return;
		}

        case 32: //NPC
		{
			Help::loadStaticModels();

			std::string message = "";
			for (int i = 4; i < datLength-1; i++)
			{
				message = message + dat[i] + " ";
			}
			message = message + dat[datLength-1];

			Help* help = new Help(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
				message); //message
			INCR_NEW("Entity");
			chunkedEntities->push_back(help);
			return;
		}

        case 69: //GoalRing
		{
			GoalRing::loadStaticModels();
			GoalRing* goal = new GoalRing(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW("Entity");
			Main_addEntity(goal);
			return;
		}

        case 70: //Race Ghost
		{
			RaceGhost::loadStaticModels();
			RaceGhost* raceGhost = new RaceGhost(dat[1]); INCR_NEW("Entity");
			Main_addEntity(raceGhost);
			return;
		}

		case 91: //General Purpose Stage Manager
		{
			int id2 = std::stoi(dat[1]);
			switch (id2)
			{
				case 0:
				{
					GF_StageManager::loadStaticModels();
					GF_StageManager* gf = new GF_StageManager; INCR_NEW("Entity");
					Main_addEntity(gf);
					break;
				}

				case 1:
                {
                    MH_StageManager::loadStaticModels();
					MH_StageManager* mh = new MH_StageManager; INCR_NEW("Entity");
					Main_addEntity(mh);
					break;
                }

                case 2:
                {
                    SR_StageManager::loadStaticModels();
					SR_StageManager* sr = new SR_StageManager; INCR_NEW("Entity");
					Main_addEntity(sr);
					break;
                }

                case 3:
                {
                    GH_StageManager::loadStaticModels();
					GH_StageManager* gh = new GH_StageManager; INCR_NEW("Entity");
					Main_addEntity(gh);
					break;
                }

                case 4:
                {
                    CE_StageManager::loadStaticModels();
					CE_StageManager* ce = new CE_StageManager; INCR_NEW("Entity");
					Main_addEntity(ce);
					break;
                }

                case 5:
                {
                    DP_StageManager::loadStaticModels();
					DP_StageManager* dp = new DP_StageManager; INCR_NEW("Entity");
					Main_addEntity(dp);
					break;
                }

                case 6:
                {
                    T_StageManager::loadStaticModels();
					T_StageManager* tut = new T_StageManager; INCR_NEW("Entity");
					Main_addEntity(tut);
					break;
                }

				default: break;
			}
			return;
		}

		case 92: //Rail
		{
			Rail* rail = new Rail(dat[1]); INCR_NEW("Entity");
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
					MH_Tank* tank = new MH_Tank(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW("Entity"); //x, y, z
					chunkedEntities->push_back(tank);
					break;
				}
				case 1: //RocketBase
				{
					MH_RocketBase::loadStaticModels();
					MH_RocketBase* rocketBase = new MH_RocketBase(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW("Entity"); //x, y, z
					chunkedEntities->push_back(rocketBase);
					break;
				}
				case 2: //GiantRocket
				{
					MH_GiantRocket::loadStaticModels();
					MH_GiantRocket* giantRocket = new MH_GiantRocket(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW("Entity"); //x, y, z
					chunkedEntities->push_back(giantRocket);
					break;
				}
				case 3: //PathFlat
				{
					MH_PathFlat::loadStaticModels();
					MH_PathFlat* pathFlat = new MH_PathFlat(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), toFloat(dat[5])); INCR_NEW("Entity"); //x, y, z, rotY
					chunkedEntities->push_back(pathFlat);
					break;
				}
				case 4: //PathDiagonal
				{
					MH_PathDiagonal::loadStaticModels();
					MH_PathDiagonal* pathDiagonal = new MH_PathDiagonal(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), toFloat(dat[5])); INCR_NEW("Entity"); //x, y, z, rotY
					chunkedEntities->push_back(pathDiagonal);
					break;
				}
				case 5: //PathFlatSmall
				{
					MH_PathFlatSmall::loadStaticModels();
					MH_PathFlatSmall* pathFlatSmall = new MH_PathFlatSmall(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), toFloat(dat[5])); INCR_NEW("Entity"); //x, y, z, rotY
					chunkedEntities->push_back(pathFlatSmall);
					break;
				}
				default: break;
			}
			return;
		}

        case 96: //Point (for paths)
		{
			Point* point = new Point(
				toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
				toInt(dat[4])); //point id
			INCR_NEW("Entity");
			Main_addEntity(point);
			return;
		}

		case 97: //Rocket
		{
			Rocket::loadStaticModels();
			Rocket* rocket = new Rocket(toInt(dat[1]), toInt(dat[2])); //Point IDs for start and end of path, position of rocket initialized to where point ID 1 is
			INCR_NEW("Entity");
			Main_addEntity(rocket);
			return;
		}

        case 98: //Line of rings
        {
            Ring::loadStaticModels();
			Vector3f pos1(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]));
			Vector3f pos2(toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]));
			int numRings = toInt(dat[7]);

			float xDiff = pos2.x - pos1.x;
			float yDiff = pos2.y - pos1.y;
			float zDiff = pos2.z - pos1.z;

			if (numRings > 1)
			{
				for (int i = 0; i < numRings; i++)
				{
					Ring* ring = new Ring(pos1.x + i*(xDiff / (numRings - 1)), 
										  pos1.y + i*(yDiff / (numRings - 1)),
										  pos1.z + i*(zDiff / (numRings - 1))); INCR_NEW("Entity");
                    chunkedEntities->push_back(ring);
				}
			}
			else
			{
				Ring* ring = new Ring(pos1.x, pos1.y, pos1.z); INCR_NEW("Entity");
                chunkedEntities->push_back(ring);
			}
            return;
        }

        case 99: //Circle of rings
        {
            Ring::loadStaticModels();
			Vector3f centerPos(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]));
			float ringRadius = toFloat(dat[4]);
			int numRings = toInt(dat[5]);

			if (numRings > 1)
			{
				float degreeSegment = 360.0f / numRings;
				Vector3f newPoint(0, centerPos.y, 0);

				for (int i = 0; i < numRings; i++)
				{
					newPoint.x = centerPos.x + ringRadius*cosf(Maths::toRadians(degreeSegment*i));
					newPoint.z = centerPos.z + ringRadius*sinf(Maths::toRadians(degreeSegment*i));
					Ring* ring = new Ring(newPoint.x, newPoint.y, newPoint.z); INCR_NEW("Entity");
					chunkedEntities->push_back(ring);
				}
			}
			else
			{
				Ring* ring = new Ring(centerPos.x, centerPos.y, centerPos.z); INCR_NEW("Entity");
				chunkedEntities->push_back(ring);
			}
            return;
        }

		default:
		{
			return;
		}
	}
}

void LevelLoader::loadLevelData()
{
	Global::gameLevelData.clear();

	LevelLoader::numLevels = 0;

	std::ifstream file(Global::pathToEXE + "res/Levels/LevelData.dat");
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "res/Levels/LevelData.dat").c_str());
		file.close();
	}
	else
	{
		std::string line;
		getlineSafe(file, line);

		LevelLoader::numLevels = std::stoi(line.c_str());
		int levelCount = LevelLoader::numLevels;
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
	Dashpad::deleteStaticModels();
	PlayerSonic::deleteStaticModels();
	Checkpoint::deleteStaticModels();
	JumpRamp::deleteStaticModels();
	GF_StageManager::deleteStaticModels();
	Ring::deleteStaticModels();
    MH_StageManager::deleteStaticModels();
	MH_GiantRocket::deleteStaticModels();
	MH_PathDiagonal::deleteStaticModels();
	MH_PathFlat::deleteStaticModels();
	MH_PathFlatSmall::deleteStaticModels();
	MH_RocketBase::deleteStaticModels();
	MH_Tank::deleteStaticModels();
    SR_StageManager::deleteStaticModels();
    GoalRing::deleteStaticModels();
    Rocket::deleteStaticModels();
    GH_StageManager::deleteStaticModels();
    Beetle::deleteStaticModels();
    CE_StageManager::deleteStaticModels();
    Spring::deleteStaticModels();
    DP_StageManager::deleteStaticModels();
    RaceGhost::deleteStaticModels();
    Help::deleteStaticModels();
    T_StageManager::deleteStaticModels();
}

int LevelLoader::getNumLevels()
{
	return LevelLoader::numLevels;
}
