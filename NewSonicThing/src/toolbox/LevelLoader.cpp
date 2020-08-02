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
#include "../entities/speedramp.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../entities/GreenForest/gfstagemanager.h"
#include "../entities/MetalHarbor/mhstaticobjects.h"
#include "../entities/MetalHarbor/mhaircraftcarriercart.h"
#include "../entities/MetalHarbor/mhcrateplatform.h"
#include "../entities/rocket.h"
#include "../entities/pulley.h"
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
#include "../entities/itemcapsule.h"
#include "../entities/springtriple.h"
#include "../menu/timer.h"
#include "../entities/RadicalHighway/rhstagemanager.h"
#include "../entities/PyramidCave/pcstaticobjects.h"
#include "../entities/lostchao.h"
#include "../entities/rhinospike.h"
#include "../entities/hunter.h"
#include "../entities/bullet.h"
#include "../entities/PyramidCave/pcstagemanager.h"
#include "../entities/GreenForest/gfvine.h"
#include "../entities/woodbox.h"
#include "../entities/npc.h"
#include "../entities/light.h"
#include "../entities/playertails.h"
#include "../entities/SacredSky/ssstagemanager.h"
#include "../entities/playerknuckles.h"
#include "../entities/DryLagoon/dlstagemanager.h"
#include "../entities/emeraldmanager.h"
#include "../entities/emeraldpiece.h"
#include "../entities/hintmonitor.h"
#include "../entities/DryLagoon/dlplant1.h"
#include "../entities/DryLagoon/dlplant2.h"
#include "../entities/DryLagoon/dltree.h"
#include "../entities/DryLagoon/dlbluebox.h"
#include "../entities/DryLagoon/dlpot.h"
#include "../entities/DryLagoon/dlturtle.h"
#include "../entities/DryLagoon/dldigteleport.h"
#include "../entities/RadicalHighway/rhnights.h"
#include "../entities/RadicalHighway/rhblimp.h"
#include "../entities/RadicalHighway/rhramp.h"
#include "../entities/TwinkleCircuit/tckart.h"
#include "../entities/TwinkleCircuit/tcstagemanager.h"
#include "../entities/TwinkleCircuit/tcdash.h"
#include "../entities/MetalHarbor/mhmissle.h"
#include "../entities/CloudTemple/ctstagemanager.h"

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

    if (Global::gameStageManager != nullptr)
    {
        delete Global::gameStageManager; INCR_DEL("Entity");
    }
    Global::gameStageManager = nullptr;

    Global::deleteAllEntites();
    Global::deleteAllChunkedEntities();

    AudioPlayer::stopBGM();
    AudioPlayer::deleteBuffersBGM();

    Global::finishStageTimer = -1;
    Global::startStageTimer = -1;

    Global::stageUsesWater = true;

    Global::gameRingCount = 0;
    Global::gameScore = 0;
    Global::gameLives = 4;

    GuiManager::clearGuisToRender();

    Global::gameState = STATE_TITLE;
    Global::levelID = -1;
    Global::gameIsNormalMode = false;
    Global::gameIsHardMode = false;
    Global::gameIsChaoMode = false;
    Global::gameIsRingMode = false;
    Global::gameIsRaceMode = false;

    Global::spawnAtCheckpoint  = false;
    Global::checkpointPlayerPos.set(0,0,0);
    Global::checkpointPlayerDir.set(1,0,0);
    Global::checkpointCamDir.set(1,0,0);
    Global::checkpointTime = 0.0f;

    Global::raceLogSize = 0;

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
                MenuManager::playerFailedArcadeMode = true;
                return;
            }
        }
    }

    std::chrono::high_resolution_clock::time_point timeStart = std::chrono::high_resolution_clock::now();
    bool waitForSomeTime = false;

    if (Global::isNewLevel)
    {
        stageFault = 1;
        waitForSomeTime = true;

        //Set the Global levelID based on the name of the level
        Level* currLvl = nullptr;
        for (int i = 0; i < (int)Global::gameLevelData.size(); i++)
        {
            Level* t = &Global::gameLevelData[i];
            if (t->fileName == fname)
            {
                currLvl = t;
                break;
            }
        }
        if (currLvl != nullptr)
        {
            if      (currLvl->displayName == "Tutorial")        Global::levelID = LVL_TUTORIAL;
            else if (currLvl->displayName == "City Escape")     Global::levelID = LVL_CITY_ESCAPE;
            else if (currLvl->displayName == "Seaside Hill")    Global::levelID = LVL_SEASIDE_HILL;
            else if (currLvl->displayName == "Green Forest")    Global::levelID = LVL_GREEN_FOREST;
            else if (currLvl->displayName == "Metal Harbor")    Global::levelID = LVL_METAL_HARBOR;
            else if (currLvl->displayName == "Pyramid Cave")    Global::levelID = LVL_PYRAMID_CAVE;
            else if (currLvl->displayName == "Speed Highway")   Global::levelID = LVL_SPEED_HIGHWAY;
            else if (currLvl->displayName == "Radical Highway") Global::levelID = LVL_RADICAL_HIGHWAY;
            else if (currLvl->displayName == "Frog Forest")     Global::levelID = LVL_FROG_FOREST;
            else if (currLvl->displayName == "Epic Test Level") Global::levelID = LVL_TEST;
            else if (currLvl->displayName == "Sky Rail")        Global::levelID = LVL_SKY_RAIL;
            else if (currLvl->displayName == "Noki Bay")        Global::levelID = LVL_NOKI_BAY;
            else if (currLvl->displayName == "Green Hill Zone") Global::levelID = LVL_GREEN_HILL_ZONE;
            else if (currLvl->displayName == "Windy Valley")    Global::levelID = LVL_WINDY_VALLEY;
            else if (currLvl->displayName == "Delfino Plaza")   Global::levelID = LVL_DELFINO_PLAZA;
            else if (currLvl->displayName == "Sacred Sky")      Global::levelID = LVL_SACRED_SKY;
            else if (currLvl->displayName == "Dry Lagoon")      Global::levelID = LVL_DRY_LAGOON;
            else if (currLvl->displayName == "Twinkle Circuit") Global::levelID = LVL_TWINKLE_CIRCUIT;
            else if (currLvl->displayName == "Cloud Temple")    Global::levelID = LVL_CLOUD_TEMPLE;
        }

        Global::spawnAtCheckpoint  = false;
        Global::checkpointPlayerPos.set(0,0,0);
        Global::checkpointPlayerDir.set(1,0,0);
        Global::checkpointCamDir.set(1,0,0);
        Global::checkpointTime = 0.0f;
    }
    Global::isNewLevel = false;

    if (stageFault == 1)
    {
        freeAllStaticModels();
    }

    if (Global::gameMainPlayer != nullptr)
    {
        delete Global::gameMainPlayer; INCR_DEL("Entity");
        Global::gameMainPlayer = nullptr;
    }

    if (Global::gameStageManager != nullptr)
    {
        delete Global::gameStageManager; INCR_DEL("Entity");
        Global::gameStageManager = nullptr;
    }

    Global::deleteAllEntites();
    Global::deleteAllChunkedEntities();

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

            //std::chrono::high_resolution_clock::time_point colTimeStart = std::chrono::high_resolution_clock::now();
            //printf("About to load collision.\n");
            //try to load a pre generated quad tree first
            CollisionModel* colModel = nullptr;//loadBinaryQuadTree("Models/" + colFLoc + "/", lineSplit[0]);
            if (colModel == nullptr)
            {
                //if there is none, load the .bincol and generate a quad tree from scratch
                colModel = ObjLoader::loadBinaryCollisionModel("Models/" + colFLoc + "/", lineSplit[0]);
                colModel->generateQuadTree(std::stoi(lineSplit[1]));
            }
            //printf("Done loading collision.\n");
            //std::chrono::high_resolution_clock::time_point colTimeEnd = std::chrono::high_resolution_clock::now();
            //std::chrono::duration<double, std::milli> colTimeSpan = colTimeEnd - colTimeStart;
            //double colDurationMillis = colTimeSpan.count();
            //printf("Loading collision took %f milliseconds.\n", (float)(colDurationMillis));

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

    std::string botFogVars;
    getlineSafe(file, botFogVars);
    {
        char lineBuf[128];
        memcpy(lineBuf, botFogVars.c_str(), botFogVars.size()+1);
        int splitLength = 0;
        char** dat = split(lineBuf, ' ', &splitLength);

        SkyManager::fogBottomPosition = toFloat(dat[0]);
        SkyManager::fogBottomThickness = toFloat(dat[1]);

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

    std::string sunDirection;
    getlineSafe(file, sunDirection);
    if (stageFault == 1)
    {
        char lineBuf[128];
        memcpy(lineBuf, sunDirection.c_str(), sunDirection.size()+1);
        int splitLength = 0;
        char** dat = split(lineBuf, ' ', &splitLength);

        Global::gameLightSun->direction.set(toFloat(dat[0]), toFloat(dat[1]), toFloat(dat[2]));
        Global::gameLightSun->direction.normalize();
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

    std::string finishPlayerDir;
    getlineSafe(file, finishPlayerDir);
    memcpy(finishBuf, finishPlayerDir.c_str(), finishPlayerDir.size()+1);
    finishLength = 0;
    finishSplit = split(finishBuf, ' ', &finishLength);
    Global::gameStage->finishPlayerDir.x = toFloat(finishSplit[0]);
    Global::gameStage->finishPlayerDir.y = toFloat(finishSplit[1]);
    Global::gameStage->finishPlayerDir.z = toFloat(finishSplit[2]);
    free(finishSplit);

    std::string finishPlayerUp;
    getlineSafe(file, finishPlayerUp);
    memcpy(finishBuf, finishPlayerUp.c_str(), finishPlayerUp.size()+1);
    finishLength = 0;
    finishSplit = split(finishBuf, ' ', &finishLength);
    Global::gameStage->finishPlayerUp.x = toFloat(finishSplit[0]);
    Global::gameStage->finishPlayerUp.y = toFloat(finishSplit[1]);
    Global::gameStage->finishPlayerUp.z = toFloat(finishSplit[2]);
    free(finishSplit);

    std::string finishCameraDir;
    getlineSafe(file, finishCameraDir);
    memcpy(finishBuf, finishCameraDir.c_str(), finishCameraDir.size()+1);
    finishLength = 0;
    finishSplit = split(finishBuf, ' ', &finishLength);
    Global::gameStage->finishCameraDir.x = toFloat(finishSplit[0]);
    Global::gameStage->finishCameraDir.y = toFloat(finishSplit[1]);
    Global::gameStage->finishCameraDir.z = toFloat(finishSplit[2]);
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

    //Underwater water color
    std::string stageWaterColorLine;
    getlineSafe(file, stageWaterColorLine);
    memcpy(finishBuf, stageWaterColorLine.c_str(), stageWaterColorLine.size()+1);
    finishLength = 0;
    finishSplit = split(finishBuf, ' ', &finishLength);
    Global::stageWaterColor.x = toFloat(finishSplit[0]);
    Global::stageWaterColor.y = toFloat(finishSplit[1]);
    Global::stageWaterColor.z = toFloat(finishSplit[2]);
    free(finishSplit);

    //Underwater water color blend amount
    std::string stageWaterBlendAmountLine;
    getlineSafe(file, stageWaterBlendAmountLine);
    Global::stageWaterBlendAmount = toFloat((char*)stageWaterBlendAmountLine.c_str());

    //Does the stage use backface culling?
    std::string backfaceCullingLine;
    getlineSafe(file, backfaceCullingLine);
    Global::renderWithCulling = (backfaceCullingLine == "true");

    //the chunked entity chunk size for this level
    std::string chunkSize;
    getlineSafe(file, chunkSize);
    float newChunkSize = std::stof(chunkSize);


    GuiManager::clearGuisToRender();

    Global::gameIsNormalMode = false;
    Global::gameIsHardMode = false;
    Global::gameIsChaoMode = false;
    Global::gameIsRingMode = false;
    Global::gameIsRaceMode = false;

    Level* currentLevel = &Global::gameLevelData[Global::levelID];
    std::string missionType = (currentLevel->missionData[Global::gameMissionNumber])[0];

    if (missionType == "Normal") Global::gameIsNormalMode = true;
    if (missionType == "Ring")   Global::gameIsRingMode   = true;
    if (missionType == "Chao")   Global::gameIsChaoMode   = true;
    if (missionType == "Hard")   Global::gameIsHardMode   = true;
    if (missionType == "Race")   Global::gameIsRaceMode   = true;

    if (Global::gameIsRingMode)
    {
        Global::gameRingTarget = std::stoi((currentLevel->missionData[Global::gameMissionNumber])[2]);
    }

    if (Global::gameIsRaceMode)
    {
        Global::gameRaceTimeLimit = std::stof((currentLevel->missionData[Global::gameMissionNumber])[2]);
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

    //Add the player's ghost
    RaceGhost::loadStaticModels();
    RaceGhost* playerGhost = new RaceGhost(
        ("res/SaveData/" + 
        Global::characterNames[Global::currentCharacterType] + "_" +
        std::to_string(Global::levelID) + "_" +
        std::to_string(Global::gameMissionNumber) +
        ".ghost").c_str(),
        -1); INCR_NEW("Entity");
    Global::addEntity(playerGhost);

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

        Global::recalculateEntityChunks(minX, maxX, minZ, maxZ, newChunkSize);

        for (Entity* e : chunkedEntities)
        {
            Global::addChunkedEntity(e);
        }
    }

    if (stageFault == 1)
    {
        //Stage::ObjLoader::loadModels((char*)modelFLoc.c_str(), (char*)modelFName.c_str()); //doing this earlier now
    }

    if (Global::gameMainPlayer != nullptr)
    {
        Global::gameMainPlayer->canMoveTimer = 1.0f;
        Global::gameMainPlayer->camDir.set(&initialCamDir);
        Global::gameMainPlayer->camDirSmooth.set(&initialCamDir);
        Vector3f startingDir = initialCamDir;
        startingDir.y = 0;
        startingDir.setLength(0.001f);
        Global::gameMainPlayer->vel.set(&startingDir);
    }

    if (waitForSomeTime)
    {
        int waitTargetMillis = 1; //how long loading screen should show at least (in milliseconds)

        std::chrono::high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> time_span = timeEnd - timeStart;
        double durationMillis = time_span.count();

        int waitForMs = waitTargetMillis - (int)durationMillis;
        if (waitForMs > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(waitForMs));
        }
    }

    Global::startStageTimer = 1.0f;


    Global::gameRingCount = 0;
    Global::gameScore = 0;

    if (Global::spawnAtCheckpoint)
    {
        if (Global::mainHudTimer != nullptr)
        {
            Global::mainHudTimer->totalTime = Global::checkpointTime;
            Global::mainHudTimer->freeze(true);
        }

        if (Global::gameMainPlayer != nullptr)
        {
            Global::gameMainPlayer->position = Global::checkpointPlayerPos;
            Global::gameMainPlayer->vel = Global::checkpointPlayerDir;
            Global::gameMainPlayer->camDir = Global::checkpointPlayerDir;
            Global::gameMainPlayer->camDirSmooth = Global::checkpointPlayerDir;
            Global::gameMainPlayer->camDir.normalize();
            Global::gameMainPlayer->camDirSmooth.normalize();
        }

        //erase the points after the checkpoint
        for (int i = 0; i < Global::raceLogSize; i++)
        {
            if (Global::raceLog[i].time >= Global::checkpointTime)
            {
                Global::raceLogSize = i;
                break;
            }
        }
    }
    else
    {
        Global::raceLogSize = 0;

        if (Global::mainHudTimer != nullptr)
        {
            Global::mainHudTimer->totalTime = 0.0f;
            Global::mainHudTimer->freeze(true);
        }

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

    //Vector3f partVel(0, 0, 0);
    //new Particle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &partVel, 0.0f,
    //    1.0f, 0.0f, 50.0f, 0.0f, true, false);

    Global::gameState = STATE_RUNNING;

    glfwSetTime(0);
    extern double timeOld;
    //extern double previousTime;
    timeOld = 0.0;
    //previousTime = 0.0;

    extern float VFOV_ADDITION;
    VFOV_ADDITION = 0;

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
            Global::addEntity(pass2);
            return;
        }

        case 3: //Stage Pass 3
        {
            StagePass3* pass3 = new StagePass3(dat[1], dat[2]); INCR_NEW("Entity");
            Global::addEntity(pass3);
            return;
        }

        case 4: //Stage Transparent
        {
            StageTransparent* trans = new StageTransparent(dat[1], dat[2]); INCR_NEW("Entity");
            Global::addEntity(trans);
            return;
        }

        case 6: //Player
        {
            if (Global::gameMainPlayer != nullptr)
            {
                delete Global::gameMainPlayer; INCR_DEL("Entity");
                Global::gameMainPlayer = nullptr;
            }

            Vector3f pos(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]));
            switch (Global::currentCharacterType)
            {
                case Global::PlayableCharacter::Sonic:
                    Global::gameMainPlayer = new PlayerSonic(pos.x, pos.y, pos.z); INCR_NEW("Entity");
                    break;

                case Global::PlayableCharacter::Tails:
                    Global::gameMainPlayer = new PlayerTails(pos.x, pos.y, pos.z); INCR_NEW("Entity");
                    break;

                case Global::PlayableCharacter::Knuckles:
                    Global::gameMainPlayer = new PlayerKnuckles(pos.x, pos.y, pos.z); INCR_NEW("Entity");
                    break;

                default:
                    break;
            }

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
            Checkpoint::loadStaticModels();
            Checkpoint* checkpoint = new Checkpoint(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
                toFloat(dat[4])); INCR_NEW("Entity");
            chunkedEntities->push_back(checkpoint);
            return;
        }

        case 11: //JumpPad
        {
            SpeedRamp::loadStaticModels();
            SpeedRamp* ramp = new SpeedRamp(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
                toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]), //rotation direction
                toFloat(dat[7]), toFloat(dat[8]));                 //power, input lock time
            INCR_NEW("Entity");
            chunkedEntities->push_back(ramp);
            return;
        }

        case 12: //Spring
        {
            Spring::loadStaticModels();
            Spring* spring = new Spring(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),      //position
                toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]),      //rotation direction
                toFloat(dat[7]), toFloat(dat[8]), (bool)toInt(dat[9])); //power, time, reset camera
            INCR_NEW("Entity");
            chunkedEntities->push_back(spring);
            return;
        }

        case 13: //SpringTriple
        {
            SpringTriple::loadStaticModels();
            SpringTriple* spring = new SpringTriple(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
                toFloat(dat[4]), toFloat(dat[5]),                  //rotation direction
                toFloat(dat[6]), toFloat(dat[7]));                 //power, time
            INCR_NEW("Entity");
            chunkedEntities->push_back(spring);
            return;
        }

        case 27: //Item Capsule
        {
            ItemCapsule::loadStaticModels();
            ItemCapsule* capsule = new ItemCapsule(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),   //position
                toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]),   //relative up direction
                toInt(dat[7]), toInt(dat[8]), chunkedEntities);      //item type, box type
            INCR_NEW("Entity");
            chunkedEntities->push_back(capsule);
            return;
        }

        case 28: //Beetle
        {
            Beetle::loadStaticModels();
            Beetle* beetle = new Beetle(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), chunkedEntities); INCR_NEW("Entity");
            chunkedEntities->push_back(beetle);
            return;
        }

        case 29: //RhinoSpike
        {
            RhinoSpike::loadStaticModels();
            RhinoSpike* rhino = new RhinoSpike(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW("Entity");
            chunkedEntities->push_back(rhino);
            return;
        }

        case 30: //Hunter
        {
            Hunter::loadStaticModels();
            Bullet::loadStaticModels();
            Hunter* hunter = new Hunter(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
                                        toFloat(dat[4]), toFloat(dat[5]),
                                        chunkedEntities); INCR_NEW("Entity");
            chunkedEntities->push_back(hunter);
            return;
        }

        case 32: //Help
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

        case 33: //NPC
        {
            NPC::loadStaticModels();

            std::string message = "";
            for (int i = 7; i < datLength-1; i++)
            {
                message = message + dat[i] + " ";
            }
            message = message + dat[datLength-1];

            NPC* npc = new NPC(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
                toFloat(dat[4]), toFloat(dat[5]),                  //rotation direction
                toInt(dat[6]),                                     //id
                message);                                          //message
            INCR_NEW("Entity");
            Global::addEntity(npc);
            return;
        }

        case 69: //GoalRing
        {
            GoalRing::loadStaticModels();
            GoalRing* goal = new GoalRing(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3])); INCR_NEW("Entity");
            Global::addEntity(goal);
            return;
        }

        case 70: //Race Ghost
        {
            RaceGhost::loadStaticModels();
            RaceGhost* raceGhost = new RaceGhost(dat[1], toInt(dat[2])); INCR_NEW("Entity");
            Global::addEntity(raceGhost);
            return;
        }

        case 73: //Emerald Piece
        {
            EmeraldPiece::loadStaticModels();
            EmeraldPiece* piece = new EmeraldPiece(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
                toInt(dat[4]), toInt(dat[5]), //piece number, diggable,
                toInt(dat[6])); //is a hard mode piece
            INCR_NEW("Entity");
            Global::addEntity(piece);
            return;
        }

        case 74: //Emerald Manager
        {
            EmeraldManager* manager = new EmeraldManager; INCR_NEW("Entity");
            Global::addEntity(manager);
            return;
        }

        case 75: //Hint Monitor
        {
            HintMonitor::loadStaticModels();
            HintMonitor* monitor = new HintMonitor(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]),
                toFloat(dat[4])); INCR_NEW("Entity");
            Global::addEntity(monitor);
            return;
        }

        case 81: //Lost Chao
        {
            LostChao::loadStaticModels();
            LostChao* chao = new LostChao(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); INCR_NEW("Entity");
            Global::addEntity(chao);
            return;
        }

        case 91: //General Purpose Stage Manager
        {
            if (Global::gameStageManager != nullptr)
            {
                delete Global::gameStageManager; INCR_DEL("Entity");
                Global::gameStageManager = nullptr;
            }

            int id2 = std::stoi(dat[1]);
            switch (id2)
            {
                case 0:
                    GF_StageManager::loadStaticModels();
                    Global::gameStageManager = new GF_StageManager; INCR_NEW("Entity");
                    break;

                case 1:
                    MH_StageManager::loadStaticModels();
                    Global::gameStageManager = new MH_StageManager; INCR_NEW("Entity");
                    break;

                case 2:
                    SR_StageManager::loadStaticModels();
                    Global::gameStageManager = new SR_StageManager; INCR_NEW("Entity");
                    break;

                case 3:
                    GH_StageManager::loadStaticModels();
                    Global::gameStageManager = new GH_StageManager; INCR_NEW("Entity");
                    break;

                case 4:
                    CE_StageManager::loadStaticModels();
                    Global::gameStageManager = new CE_StageManager; INCR_NEW("Entity");
                    break;

                case 5:
                    DP_StageManager::loadStaticModels();
                    Global::gameStageManager = new DP_StageManager; INCR_NEW("Entity");
                    break;

                case 6:
                    T_StageManager::loadStaticModels();
                    Global::gameStageManager = new T_StageManager; INCR_NEW("Entity");
                    break;

                case 7:
                    RH_StageManager::loadStaticModels();
                    Global::gameStageManager = new RH_StageManager; INCR_NEW("Entity");
                    break;

                case 8:
                    PC_StageManager::loadStaticModels();
                    Global::gameStageManager = new PC_StageManager; INCR_NEW("Entity");
                    break;

                case 9:
                    SS_StageManager::loadStaticModels();
                    Global::gameStageManager = new SS_StageManager; INCR_NEW("Entity");
                    break;

                case 10:
                    DL_StageManager::loadStaticModels();
                    Global::gameStageManager = new DL_StageManager; INCR_NEW("Entity");
                    break;

                case 11:
                    TC_StageManager::loadStaticModels();
                    Global::gameStageManager = new TC_StageManager; INCR_NEW("Entity");
                    break;

                case 12:
                    CT_StageManager::loadStaticModels();
                    Global::gameStageManager = new CT_StageManager; INCR_NEW("Entity");
                    break;

                default: break;
            }
            return;
        }

        case 92: //Rail
        {
            Rail* rail = new Rail(dat[1]); INCR_NEW("Entity");
            Global::addEntity(rail);
            return;
        }

        case 93: //Metal Harbor Specific Objects
        {
            switch (toInt(dat[1]))
            {
                case 0: //Static Objects (environment stuff that never moves)
                {
                    MH_StaticObjects::loadStaticModels();
                    MH_StaticObjects* staticObjects = new MH_StaticObjects(); INCR_NEW("Entity");
                    Global::addEntity(staticObjects);
                    return;
                }

                case 1: //Aircraft Carrier Cart
                {
                    MH_AircraftCarrierCart::loadStaticModels();
                    MH_AircraftCarrierCart* yellowMovingPlatform = new MH_AircraftCarrierCart(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5]), toFloat(dat[6]),                  //dirX, dirZ
                        toFloat(dat[7]), toFloat(dat[8]));                 //displacementMax, speed
                    INCR_NEW("Entity");
                    Global::addEntity(yellowMovingPlatform);
                    return;
                }

                case 2: //Crate Platform
                {
                    MH_CratePlatform::loadStaticModels();
                    MH_CratePlatform* cratePlatform = new MH_CratePlatform(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]),  //position
                        toFloat(dat[5]), toFloat(dat[6]),                   //dirX, dirZ
                        toFloat(dat[7]), toFloat(dat[8]),                   //displacementMax, speed
                        toInt(dat[9]));                                     //Type: 0: long, 1: with box, 2: without box
                    INCR_NEW("Entity");
                    Global::addEntity(cratePlatform);
                    return;
                }

                case 3: //Missle
                {
                    MH_Missle::loadStaticModels();
                    MH_Missle* missle = new MH_Missle(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]));
                    INCR_NEW("Entity");
                    Global::addEntity(missle);
                    return;
                }

                default:
                    return;
            }
        }

        case 96: //Point (for paths)
        {
            Point* point = new Point(
                toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position
                toInt(dat[4])); //point id
            INCR_NEW("Entity");
            Global::addEntity(point);
            return;
        }

        case 97: //Rocket
        {
            Rocket::loadStaticModels();
            Vector3f pos1(toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]));
            Vector3f pos2(toFloat(dat[4]), toFloat(dat[5]), toFloat(dat[6]));
            Rocket* rocket = new Rocket(&pos1, &pos2);
            INCR_NEW("Entity");
            Global::addEntity(rocket);
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

        case 100: //Pyramid Cave Specific
        {
            switch (toInt(dat[1]))
            {
                case 0: //Static Objects
                {
                    PC_StaticObjects::loadStaticModels();
                    PC_StaticObjects* staticObjects = new PC_StaticObjects; INCR_NEW("Entity");
                    Global::addEntity(staticObjects);
                    return;
                }

                default:
                    return;
            }
        }

        case 101: //Pulley
        {
            Pulley::loadStaticModels();
            Pulley* pulley = new Pulley(
                    toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position x,y,z
                    toFloat(dat[4]), toFloat(dat[5]));                 //y rotation, handle vertical displacement
            INCR_NEW("Entity");
            chunkedEntities->push_back(pulley);
            return;
        }

        case 102: //Green Forest Specific
        {
            switch (toInt(dat[1]))
            {
                case 1: //Vine
                {
                    GF_Vine::loadStaticModels();
                    GF_Vine* vine = new GF_Vine(
                        toFloat(dat[2]),  toFloat(dat[3]),  toFloat(dat[4]),  //position
                        toFloat(dat[5]),  toFloat(dat[6]),  toFloat(dat[7]),  //vine end position
                        toFloat(dat[8]),  toFloat(dat[9]),  toFloat(dat[10]), //rotation axis
                        toFloat(dat[11]),                                     //number of rotations until done
                        toFloat(dat[12]), toFloat(dat[13]), toFloat(dat[14]), //launch direction
                        toFloat(dat[15]),                                     //rotate speed
                        toFloat(dat[16]),                                     //launch speed
                        toFloat(dat[17]));                                    //input lock time

                    INCR_NEW("Entity");
                    Global::addEntity(vine);
                    return;
                }

                default:
                    return;
            }
        }

        case 103: //Wood Box
        {
            WoodBox::loadStaticModels();
            WoodBox* box = new WoodBox(
                    toFloat(dat[1]), toFloat(dat[2]), toFloat(dat[3]), //position x,y,z
                    toFloat(dat[4]));                                  //y rotation
            INCR_NEW("Entity");
            chunkedEntities->push_back(box);
            return;
        }

        case 104: //Dry Lagoon Specific Objects
        {
            switch (toInt(dat[1]))
            {
                case 0: //Plant 1
                {
                    DL_Plant1::loadStaticModels();
                    DL_Plant1* plant1 = new DL_Plant1(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5]), toFloat(dat[6]), toFloat(dat[7]), //rotation
                        toFloat(dat[8])); INCR_NEW("Entity"); //scale
                    chunkedEntities->push_back(plant1);
                    return;
                }

                case 1: //Plant 2
                {
                    DL_Plant2::loadStaticModels();
                    DL_Plant2* plant2 = new DL_Plant2(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5]), toFloat(dat[6]), toFloat(dat[7]), //rotation
                        toFloat(dat[8])); INCR_NEW("Entity"); //scale
                    chunkedEntities->push_back(plant2);
                    return;
                }

                case 2: //Tree
                {
                    DL_Tree::loadStaticModels();
                    DL_Tree* tree = new DL_Tree(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5]), toFloat(dat[6]), toFloat(dat[7]), //rotation
                        toFloat(dat[8])); INCR_NEW("Entity"); //scale
                    Global::addEntity(tree);
                    return;
                }

                case 3: //Blue Box
                {
                    DL_BlueBox::loadStaticModels();
                    DL_BlueBox* box = new DL_BlueBox(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5])); INCR_NEW("Entity"); //rotation
                    chunkedEntities->push_back(box);
                    return;
                }

                case 4: //Pot
                {
                    DL_Pot::loadStaticModels();
                    DL_Pot* pot = new DL_Pot(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5])); INCR_NEW("Entity"); //rotation
                    chunkedEntities->push_back(pot);
                    return;
                }

                case 5: //Turtle
                {
                    DL_Turtle::loadStaticModels();
                    DL_Turtle* pot = new DL_Turtle(
                        toFloat(dat[2]), toFloat(dat[3]),  toFloat(dat[4]), //position
                        toFloat(dat[5]),                                    //rotation
                        toFloat(dat[6]), toFloat(dat[7]),  toFloat(dat[8]), //teleport position
                        toFloat(dat[9]), toFloat(dat[10]), toFloat(dat[11])); INCR_NEW("Entity"); //teleport cam
                    chunkedEntities->push_back(pot);
                    return;
                }

                case 6: //Teleport
                {
                    DL_DigTeleport::loadStaticModels();
                    DL_DigTeleport* warp = new DL_DigTeleport(
                        toFloat(dat[2]), toFloat(dat[3]),  toFloat(dat[4]),   //position
                        toFloat(dat[5]),                                      //y rotation
                        toFloat(dat[6]), toFloat(dat[7]),  toFloat(dat[8]),   //teleport position
                        toFloat(dat[9]), toFloat(dat[10]), toFloat(dat[11])); //color
                    INCR_NEW("Entity");
                    chunkedEntities->push_back(warp);
                    return;
                }

                default:
                    return;
            }
        }

        case 105: //Radical Highway Specific Objects
        {
            switch (toInt(dat[1]))
            {
                case 0: //Nights spinning head
                {
                    RH_Nights::loadStaticModels();
                    RH_Nights* nights = new RH_Nights(toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4])); //position
                    INCR_NEW("Entity");
                    Global::addEntity(nights);
                    return;
                }

                case 1: //Blimp
                {
                    RH_Blimp::loadStaticModels();
                    RH_Blimp* blimp = new RH_Blimp(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5])); //rot y
                    INCR_NEW("Entity");
                    Global::addEntity(blimp);
                    return;
                }

                case 2: //Ramp
                {
                    RH_Ramp::loadStaticModels();
                    RH_Ramp* ramp = new RH_Ramp(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]), //position
                        toFloat(dat[5]), toFloat(dat[6])); //rot y, rot z
                    INCR_NEW("Entity");
                    Global::addEntity(ramp);
                    return;
                }

                default:
                    return;
            }
        }

        case 106: //Twinkle Circuit Specific Objects
        {
            switch (toInt(dat[1]))
            {
                case 0: //Kart
                {
                    TC_Kart* kart = new TC_Kart(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[4]),  //position
                        toFloat(dat[5]), toFloat(dat[6]), toFloat(dat[7])); //direction
                    INCR_NEW("Entity");
                    Global::gameKart = kart;
                    Global::addEntity(kart);
                    return;
                }

                case 1: //Dash
                {
                    TC_Dash::loadStaticModels();
                    TC_Dash* dash = new TC_Dash(
                        toFloat(dat[2]), toFloat(dat[3]), toFloat(dat[ 4]),  //position
                        toFloat(dat[5]), toFloat(dat[6]), toFloat(dat[ 7]),  //at
                        toFloat(dat[8]), toFloat(dat[9]), toFloat(dat[10])); //up
                    INCR_NEW("Entity");
                    Global::addChunkedEntity(dash);
                    return;
                }

                default:
                    return;
            }
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
    SpeedRamp::deleteStaticModels();
    GF_StageManager::deleteStaticModels();
    Ring::deleteStaticModels();
    MH_StageManager::deleteStaticModels();
    MH_StaticObjects::deleteStaticModels();
    MH_AircraftCarrierCart::deleteStaticModels();
    MH_CratePlatform::deleteStaticModels();
    Pulley::deleteStaticModels();
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
    ItemCapsule::deleteStaticModels();
    SpringTriple::deleteStaticModels();
    RH_StageManager::deleteStaticModels();
    PC_StaticObjects::deleteStaticModels();
    LostChao::deleteStaticModels();
    RhinoSpike::deleteStaticModels();
    Hunter::deleteStaticModels();
    Bullet::deleteStaticModels();
    PC_StageManager::deleteStaticModels();
    GF_Vine::deleteStaticModels();
    WoodBox::deleteStaticModels();
    NPC::deleteStaticModels();
    PlayerTails::deleteStaticModels();
    PlayerKnuckles::deleteStaticModels();
    SS_StageManager::deleteStaticModels();
    DL_StageManager::deleteStaticModels();
    EmeraldPiece::deleteStaticModels();
    HintMonitor::deleteStaticModels();
    DL_Plant1::deleteStaticModels();
    DL_Plant2::deleteStaticModels();
    DL_Tree::deleteStaticModels();
    DL_BlueBox::deleteStaticModels();
    DL_Pot::deleteStaticModels();
    DL_Turtle::deleteStaticModels();
    DL_DigTeleport::deleteStaticModels();
    RH_Nights::deleteStaticModels();
    RH_Blimp::deleteStaticModels();
    RH_Ramp::deleteStaticModels();
    TC_Kart::deleteStaticModels();
    TC_StageManager::deleteStaticModels();
    TC_Dash::deleteStaticModels();
    MH_Missle::deleteStaticModels();
    CT_StageManager::deleteStaticModels();
}

int LevelLoader::getNumLevels()
{
    return LevelLoader::numLevels;
}
