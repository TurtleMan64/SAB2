#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include <string>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include <chrono>
#include <ctime>
#include <random>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "main.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../models/models.h"
#include "../shaders/shaderprogram.h"
#include "../textures/modeltexture.h"
#include "../entities/entity.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../objLoader/objLoader.h"
#include "../entities/light.h"
#include "../entities/stage.h"
#include "../toolbox/levelloader.h"
#include "../collision/collisionchecker.h"
#include "../entities/skysphere.h"
#include "../renderEngine/skymanager.h"
#include "../animation/animationresources.h"
#include "../fontRendering/textmaster.h"
#include "../fontMeshCreator/fonttype.h"
#include "../fontMeshCreator/guitext.h"
#include "../guis/guimanager.h"
#include "../audio/audiomaster.h"
#include "../audio/audioplayer.h"
#include "../particles/particlemaster.h"
#include "../particles/particleresources.h"
#include "../toolbox/split.h"
#include "../shadows/shadowmapmasterrenderer.h"
#include "../shadows2/shadowmapmasterrenderer2.h"
#include "../postProcessing/postprocessing.h"
#include "../postProcessing/fbo.h"
#include "../guis/guirenderer.h"
#include "../guis/guitextureresources.h"
#include "../toolbox/level.h"
#include "../guis/guitexture.h"
#include "../particles/particle.h"
#include "../entities/skysphere.h"
#include "../fontMeshCreator/guinumber.h"
#include "../entities/controllableplayer.h"
#include "../entities/checkpoint.h"
#include "../water/waterframebuffers.h"
#include "../water/watershader.h"
#include "../water/waterrenderer.h"
#include "../water/watertile.h"
#include "../toolbox/getline.h"
#include "../menu/menumanager.h"
#include "../menu/mainmenu.h"
#include "../menu/timer.h"
#include "../menu/hud.h"
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

std::string Global::pathToEXE;

std::unordered_set<Entity*> gameEntities;
std::list<Entity*> gameEntitiesToAdd;
std::list<Entity*> gameEntitiesToDelete;

//vector that we treat as a 2D array. 
std::vector<std::unordered_set<Entity*>> gameChunkedEntities;
std::list<Entity*> gameChunkedEntitiesToAdd;
std::list<Entity*> gameChunkedEntitiesToDelete;
float chunkedEntitiesMinX = 0;
float chunkedEntitiesMinZ = 1;
float chunkedEntitiesChunkSize = 1;
int chunkedEntitiesWidth = 1;
int chunkedEntitiesHeight = 1;


float Global::waterHeight = 0.0f;
WaterRenderer* Global::gameWaterRenderer = nullptr;
WaterFrameBuffers* Global::gameWaterFBOs = nullptr;
std::vector<WaterTile*> Global::gameWaterTiles;

float dt = 0;
double timeOld = 0;
double timeNew = 0;
Camera*             Global::gameCamera      = nullptr;
ControllablePlayer* Global::gameMainPlayer  = nullptr;
Stage*              Global::gameStage       = nullptr;
SkySphere*          Global::gameSkySphere   = nullptr;
Light*              Global::gameLightSun    = nullptr;
Light*              Global::gameLightMoon   = nullptr;

float Global::finishStageTimer = -1;

Fbo* Global::gameMultisampleFbo = nullptr;
Fbo* Global::gameOutputFbo = nullptr;
Fbo* Global::gameOutputFbo2 = nullptr;

bool Global::debugDisplay = false;
bool Global::frozen = false;
bool Global::step = false;

bool Global::useHighQualityWater = true;
unsigned Global::HQWaterReflectionWidth = 1280;
unsigned Global::HQWaterReflectionHeight = 720;
unsigned Global::HQWaterRefractionWidth = 1280;
unsigned Global::HQWaterRefractionHeight = 720;

bool Global::renderParticles = true;

bool Global::renderBloom = false;

bool Global::framerateUnlock = false;

bool Global::useFullscreen = false;

bool Global::renderShadowsFar = false;
bool Global::renderShadowsClose = false;
int Global::shadowsFarQuality = 0;


//extern bool INPUT_JUMP;
//extern bool INPUT_ACTION;
//extern bool INPUT_ACTION2;
//
//extern bool INPUT_PREVIOUS_JUMP;
//extern bool INPUT_PREVIOUS_ACTION;
//extern bool INPUT_PREVIOUS_ACTION2;

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

int Global::countNew = 0;
int Global::countDelete = 0;
int Global::gameState = 0;
int Global::levelID = 0;
float Global::startStageTimer = -1;
bool Global::shouldLoadLevel = false;
bool Global::isNewLevel = false;
bool Global::isAutoCam = true;
std::string Global::levelName = "";
std::string Global::levelNameDisplay = "";
int Global::gameRingCount = 0;
int Global::gameScore = 0;
int Global::gameLives = 4;
float Global::gameClock = 0.0f;
int Global::gameTotalPlaytime = 0;
float Global::gameArcadePlaytime = 0;
float Global::deathHeight = -100.0f;
int Global::gameMainVehicleSpeed = 0;

int Global::gameMissionNumber = 0;
std::string Global::gameMissionDescription = "";
bool Global::gameIsNormalMode = false;
bool Global::gameIsHardMode = false;
bool Global::gameIsChaoMode = false;
bool Global::gameIsRingMode = false;
bool Global::gameIsRaceMode = false;
float Global::gameRaceTimeLimit = 0.0f;
int  Global::gameRingTarget = 100;
bool Global::gameIsArcadeMode = false;
std::vector<Level> Global::gameLevelData;
std::unordered_map<std::string, std::string> Global::gameSaveData;
bool Global::stageUsesWater = true;
FontType* Global::fontVipnagorgialla = nullptr;
bool Global::renderWithCulling = true;
bool Global::displayFPS = true;
//float Global::fpsTarget = 120.0f;
float Global::fpsLimit = 60.0f;
int Global::currentCalculatedFPS = 0;
int Global::renderCount = 0;
int Global::displaySizeChanged = 0;

int Global::gameArcadeIndex = 0;
std::vector<int> Global::gameArcadeLevelIds;

//std::list<std::string> Global::raceLog;
bool Global::shouldLogRace = false;

int Global::raceLogSize;
GhostFrame Global::raceLog[Global::raceLogSizeMax]; //enough for 10 minutes at 720fps

bool Global::spawnAtCheckpoint = false;
Vector3f Global::checkpointPlayerPos;
Vector3f Global::checkpointPlayerDir;
Vector3f Global::checkpointCamDir;
float Global::checkpointTime = 0;

GUIText* Global::titleCardLevelName          = nullptr;
GUIText* Global::titleCardMission            = nullptr;
GUIText* Global::titleCardMissionDescription = nullptr;

bool Global::unlockedSonicDoll = true;
bool Global::unlockedMechaSonic = true;
bool Global::unlockedDage4 = true;
bool Global::unlockedManiaSonic = true;
bool Global::unlockedAmy = true;

std::vector<std::string> Global::npcList;

void increaseProcessPriority();

void doListenThread();

void listen();

MenuManager Global::menuManager;
Timer* Global::mainHudTimer = nullptr;

int main(int argc, char** argv)
{
    if (argc > 0)
    {
        Global::pathToEXE = argv[0];

        #ifdef _WIN32
        int idx = (int)Global::pathToEXE.find_last_of('\\', Global::pathToEXE.size());
        Global::pathToEXE = Global::pathToEXE.substr(0, idx+1);
        #else
        int idx = (int)Global::pathToEXE.find_last_of('/', Global::pathToEXE.size());
        Global::pathToEXE = Global::pathToEXE.substr(0, idx+1);
        #endif
    }

    #ifdef DEV_MODE
    std::thread listenThread(doListenThread);
    #endif

    increaseProcessPriority();

    Global::countNew = 0;
    Global::countDelete = 0;

    srand(0);

    createDisplay();

    Global::loadSaveData();

    //The levels you play in arcade mode, in order
    Global::gameArcadeLevelIds.push_back(LVL_TUTORIAL);
    Global::gameArcadeLevelIds.push_back(LVL_METAL_HARBOR);
    Global::gameArcadeLevelIds.push_back(LVL_RADICAL_HIGHWAY);
    Global::gameArcadeLevelIds.push_back(LVL_GREEN_FOREST);
    Global::gameArcadeLevelIds.push_back(LVL_SKY_RAIL);

    #if !defined(DEV_MODE) && defined(_WIN32)
    FreeConsole();
    #endif

    Input::init();

    //This camera is never deleted.
    Camera cam;
    Global::gameCamera = &cam;

    Master_init();

    LevelLoader::loadLevelData();

    AudioMaster::init();

    Global::fontVipnagorgialla = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt"); INCR_NEW("FontType");

    TextMaster::init();

    GuiManager::init();

    Global::menuManager.push(new MainMenu); INCR_NEW("MainMenu");


    if (Global::renderParticles)
    {
        ParticleResources::loadParticles();
    }

    GuiTextureResources::loadGuiTextures();

    CollisionChecker::initChecker();
    AnimationResources::createAnimations();

    //This light never gets deleted.
    Light lightSun;
    Global::gameLightSun = &lightSun;

    //This light never gets deleted.
    Light lightMoon;
    Global::gameLightMoon = &lightMoon;

    //This stage never gets deleted.
    Stage stage;
    Global::gameStage = &stage;

    //This sky sphere never gets deleted.
    SkySphere skySphere;
    Global::gameSkySphere = &skySphere;


    SkyManager::initSkyManager(nullptr, nullptr);
    SkyManager::setTimeOfDay(155.0f);

    lightSun.getPosition()->x = 0;
    lightSun.getPosition()->y = 0;
    lightSun.getPosition()->z = 0;
    lightMoon.getPosition()->y = -100000;

    if (Global::useHighQualityWater)
    {
        Global::gameWaterFBOs     = new WaterFrameBuffers; INCR_NEW("WaterFrameBuffers");
        WaterShader* waterShader  = new WaterShader; INCR_NEW("WaterShader");
        Global::gameWaterRenderer = new WaterRenderer(waterShader, Master_getProjectionMatrix(), Global::gameWaterFBOs, Master_getShadowRenderer()); INCR_NEW("WaterRenderer");
        for (int r = -1; r <= 2; r++)
        {
            for (int c = -1; c <= 2; c++)
            {
                Global::gameWaterTiles.push_back(new WaterTile(r*WaterTile::TILE_SIZE*2-WaterTile::TILE_SIZE, c*WaterTile::TILE_SIZE*2-WaterTile::TILE_SIZE)); INCR_NEW("WaterTile");
            }
        }
    }

    if (Global::renderBloom)
    {
        Global::gameMultisampleFbo = new Fbo(SCR_WIDTH, SCR_HEIGHT); INCR_NEW("Fbo");
        Global::gameOutputFbo      = new Fbo(SCR_WIDTH, SCR_HEIGHT, Fbo::DEPTH_TEXTURE); INCR_NEW("Fbo");
        Global::gameOutputFbo2     = new Fbo(SCR_WIDTH, SCR_HEIGHT, Fbo::DEPTH_TEXTURE); INCR_NEW("Fbo");
        PostProcessing::init();
    }

    ParticleMaster::init(Master_getProjectionMatrix());

    long long secSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    glfwSetTime(0);

    int frameCount = 0;
    double previousTime = 0;

    Global::gameState = STATE_TITLE;

    GuiTexture* rankDisplay = nullptr;

    std::list<std::unordered_set<Entity*>*> entityChunkedList;

    while (Global::gameState != STATE_EXITING && displayWantsToClose() == 0)
    {
        timeNew = glfwGetTime();

        #ifndef WIN32
        //spin lock to meet the target fps, and gives extremely consistent dt's.
        // also of course uses a ton of cpu.
        //if (Global::gameState == STATE_RUNNING && Global::framerateUnlock)
        //{
        //    double dtFrameNeedsToTake = 1.0/((double)Global::fpsLimit);
        //    while ((timeNew - timeOld) < dtFrameNeedsToTake)
        //    {
        //        timeNew = glfwGetTime();
        //    }
        //}
        #else
        //another idea: windows only. if you put the thread/process into a above normal priority,
        // and call Sleep, it will actually sleep and return pretty consistently close
        // to the amount you slept for. in my testing, it would never sleep for more
        // than 2 milliseconds longer than what it was given (max was 1.5323 ms more than
        // the sleep amount). So, we can sleep for 2ms less than the time needed, and then
        // busy wait loop for the remaining time.
        // This actually works really well but also has the same problem as the busy wait loop,
        // which is the video looks choppy at bad fps targets. For example, if you set the target to 
        // 60fps on a 60fps monitor, then it looks fine. But, if you set the target to 90fps, then
        // it looks very choppy.
        if (Global::gameState == STATE_RUNNING && Global::framerateUnlock)
        {
            double dtFrameNeedsToTake = 1.0/((double)Global::fpsLimit);
            timeNew = glfwGetTime();
        
            const double sleepBuffer = 0.00175; //sleep will hopefully never take longer than this to return
            double sleepTime = (dtFrameNeedsToTake - (timeNew - timeOld)) - sleepBuffer;
            int msToSleep = (int)(sleepTime*1000);
            if (msToSleep >= 1)
            {
                Sleep(msToSleep);
            }
        
            timeNew = glfwGetTime();
            while ((timeNew - timeOld) < dtFrameNeedsToTake)
            {
                timeNew = glfwGetTime();
            }
        }
        #endif

        dt = (float)(timeNew - timeOld);
        dt = std::fminf(dt, 0.04f); //Anything lower than 25fps will slow the gameplay down
        timeOld = timeNew;

        Input::pollInputs();

        frameCount++;
        Global::renderCount++;

        long long nextSecSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int epocSecDiff = (int)(nextSecSinceEpoch - secSinceEpoch);
        secSinceEpoch = nextSecSinceEpoch;
        Global::gameTotalPlaytime+=epocSecDiff;

        if (Global::gameIsArcadeMode)
        {
            Global::gameArcadePlaytime+=dt;
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::fprintf(stderr, "########  GL ERROR  ########\n");
            std::fprintf(stderr, "%d\n", err);
        }

        ALenum erral = alGetError();
        if (erral != AL_NO_ERROR)
        {
            std::fprintf(stderr, "########  AL ERROR  ########\n");
            std::fprintf(stderr, "%d\n", erral);
        }

        //long double thisTime = std::time(0);
        //std::fprintf(stdout, "time: %f time\n", thisTime);

        //entities managment
        for (auto entityToAdd : gameEntitiesToAdd)
        {
            gameEntities.insert(entityToAdd);
        }
        gameEntitiesToAdd.clear();

        for (auto entityToDelete : gameEntitiesToDelete)
        {
            gameEntities.erase(entityToDelete);
            delete entityToDelete; INCR_DEL("Entity");
        }
        gameEntitiesToDelete.clear();

        //chunked entities mamanegement
        for (auto entityToAdd : gameChunkedEntitiesToAdd)
        {
            int realIndex = Global::getChunkIndex(entityToAdd->getX(), entityToAdd->getZ());
            gameChunkedEntities[realIndex].insert(entityToAdd);
        }
        gameChunkedEntitiesToAdd.clear();

        for (auto entityToDelete : gameChunkedEntitiesToDelete)
        {
            int realIndex = Global::getChunkIndex(entityToDelete->getX(), entityToDelete->getZ());
            size_t numDeleted = gameChunkedEntities[realIndex].erase(entityToDelete);
            if (numDeleted == 0)
            {
                for (int i = 0; i < (int)gameChunkedEntities.size(); i++)
                {
                    numDeleted = gameChunkedEntities[i].erase(entityToDelete);
                    if (numDeleted > 0)
                    {
                        break;
                    }
                }

                if (numDeleted == 0)
                {
                    std::fprintf(stdout, "Error: Tried to delete a chunked entity that wasn't in the lists.\n");
                }
                else
                {
                    delete entityToDelete; INCR_DEL("Entity");
                }
            }
            else
            {
                delete entityToDelete; INCR_DEL("Entity");
            }
        }
        gameChunkedEntitiesToDelete.clear();

        switch (Global::gameState)
        {
            case STATE_RUNNING:
            {
                //game logic

                //unlock framerate during gameplay
                if (Global::framerateUnlock)
                {
                    //idea to make it possible to render at 120, 180, 240fps, etc.
                    // works, but frame dt's are so different that it doesn't feel good.
                    // example: at 180fps on 60hz monitor, youd maybe get dt's of 2ms, 2ms, and then 12.66ms to finish off the next monitor refresh.
                    // 3 frames were rendered per 1 monitor refresh, but they wont feel good.
                    //if (Global::renderCount%(Global::syncToDisplayEveryXFrames) == 0)
                    //{
                    //    glfwSwapInterval(1);
                    //}
                    //else
                    //{
                        glfwSwapInterval(0);
                    //}
                }
                else
                {
                    glfwSwapInterval(1);
                }

                if (Global::startStageTimer >= 0)
                {
                    Global::startStageTimer -= dt;
                    if (Global::startStageTimer < 0)
                    {
                        Global::mainHudTimer->freeze(false);
                    }
                }

                if (Global::gameMainPlayer != nullptr)
                {
                    Global::gameMainPlayer->step();
                }
                for (Entity* e : gameEntities)
                {
                    e->step();
                }
                if (gameChunkedEntities.size() > 0)
                {
                    Global::getNearbyEntities(cam.eye.x, cam.eye.z, 2, &entityChunkedList);
                    for (std::unordered_set<Entity*>* entitySet : entityChunkedList)
                    {
                        for (Entity* e : (*entitySet))
                        {
                            e->step();
                        }
                    }
                }

                skySphere.step();
                ModelTexture::updateAnimations(dt);
                Global::gameCamera->refresh();
                ParticleMaster::update(Global::gameCamera);
                Global::gameClock+=dt;

                if (Global::debugDisplay && Global::frozen)
                {
                    Global::gameState = STATE_DEBUG;
                }

                if (Global::gameIsRingMode)
                {
                    if (Global::gameRingCount >= Global::gameRingTarget && Global::finishStageTimer < 0)
                    {
                        Global::finishStageTimer = 0;
                        Global::mainHudTimer->freeze(true);
                    }
                }
                break;
            }

            case STATE_PAUSED:
            {
                //vsync during pausing. no need to stress the system.
                glfwSwapInterval(1);
                break;
            }

            case STATE_CUTSCENE:
            {
                glfwSwapInterval(1);
                Global::gameCamera->refresh();
                break;
            }

            case STATE_TITLE:
            {
                //vsync during title. no need to stress the system.
                glfwSwapInterval(1);
                Global::gameCamera->refresh();
                ParticleMaster::update(Global::gameCamera);
                break;
            }

            case STATE_DEBUG:
            {
                glfwSwapInterval(1);

                if (Global::gameMainPlayer != nullptr)
                {
                    //Global::gamePlayer->debugAdjustCamera();
                }

                Input::pollInputs();
                if (Global::step)
                {
                    Global::gameState = STATE_RUNNING;
                    Global::step = false;
                }
                
                if (Global::debugDisplay == false || Global::frozen == false)
                {
                    Global::gameState = STATE_RUNNING;
                }
                break;
            }

            default:
                break;
        }

        Global::menuManager.step();

        Stage::updateVisibleChunks();
        SkyManager::calculateValues();

        //prepare entities to render
        for (Entity* e : gameEntities)
        {
            Master_processEntity(e);
        }
        if (gameChunkedEntities.size() > 0)
        {
            for (std::unordered_set<Entity*>* entitySet : entityChunkedList)
            {
                for (Entity* e : (*entitySet))
                {
                    Master_processEntity(e);
                }
            }
        }
        
        Master_processEntity(&stage);
        Master_renderShadowMaps(&lightSun);
        Master_processEntity(&skySphere);

        glEnable(GL_CLIP_DISTANCE1);
        if (Global::useHighQualityWater && Global::stageUsesWater)
        {
            glEnable(GL_CLIP_DISTANCE0);
            bool aboveWater = (cam.eye.y > Global::waterHeight);

            const float offsetWater = 0.3f;

            //reflection render
            Global::gameWaterFBOs->bindReflectionFrameBuffer();
            cam.mirrorForWater();
            if (aboveWater)
            {
                Master_render(&cam, 0, 1, 0, offsetWater - Global::waterHeight);
                if (Global::renderParticles)
                {
                    ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), 1);
                }
            }
            else
            {
                Master_render(&cam, 0, -1, 0, offsetWater + Global::waterHeight);
                if (Global::renderParticles)
                {
                    ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), -1);
                }
            }
            cam.mirrorForWater();
            Global::gameWaterFBOs->unbindCurrentFrameBuffer();

            //refraction render
            Global::gameWaterFBOs->bindRefractionFrameBuffer();
            if (aboveWater)
            {
                Master_render(&cam, 0, -1, 0, offsetWater + Global::waterHeight);
                if (Global::renderParticles)
                {
                    ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), -1);
                }
            }
            else
            {
                Master_render(&cam, 0, 1, 0, offsetWater - Global::waterHeight);
                if (Global::renderParticles)
                {
                    ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), 1);
                }
            }
            Global::gameWaterFBOs->unbindCurrentFrameBuffer();

            glDisable(GL_CLIP_DISTANCE0);
        }

        Vector3f camVel = cam.vel.scaleCopy(0.016666f);
        AudioMaster::updateListenerData(&cam.eye, &cam.target, &cam.up, &camVel);

        if (Global::renderBloom)
        {
            Global::gameMultisampleFbo->bindFrameBuffer();
        }
        Master_render(&cam, 0, 0, 0, 0);
        glDisable(GL_CLIP_DISTANCE1);

        if (Global::useHighQualityWater && Global::stageUsesWater)
        {
            Global::gameWaterRenderer->render(&Global::gameWaterTiles, &cam, &lightSun);
        }

        ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), 0);

        if (Global::renderBloom)
        {
            Global::gameMultisampleFbo->unbindFrameBuffer();
            Global::gameMultisampleFbo->resolveToFbo(GL_COLOR_ATTACHMENT0, Global::gameOutputFbo);
            Global::gameMultisampleFbo->resolveToFbo(GL_COLOR_ATTACHMENT1, Global::gameOutputFbo2);
            PostProcessing::doPostProcessing(Global::gameOutputFbo->getColourTexture(), Global::gameOutputFbo2->getColourTexture());
        }

        Master_clearAllEntities();

        if (rankDisplay != nullptr)
        {
            GuiManager::addGuiToRender(rankDisplay);
        }

        GuiManager::refresh();
        GuiManager::clearGuisToRender();
        TextMaster::render();

        updateDisplay();

        AudioPlayer::refreshBGM();

        Global::clearTitleCard();

        if (Global::shouldLoadLevel)
        {
            Global::shouldLoadLevel = false;
            LevelLoader::loadLevel(Global::levelName);
        }

        if (Global::finishStageTimer >= 0)
        {
            float finishTimerBefore = Global::finishStageTimer;
            Global::finishStageTimer += dt;

            //Stage finished stuff
            if (finishTimerBefore < 0.0166f && Global::finishStageTimer >= 0.0166f)
            {
                Vector3f partVel(0, 0, 0);
                ParticleMaster::createParticle(ParticleResources::textureWhiteFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 0, 900, 0, true, false, 1.0f, false);
            }

            if (finishTimerBefore < 1.0f && Global::finishStageTimer >= 1.0f)
            {
                AudioPlayer::stopBGM();
                AudioPlayer::play(24, &Global::gameMainPlayer->position);

                //Add score based on timer
                float currTime = 0;
                if (Global::mainHudTimer != nullptr)
                {
                    currTime = Global::mainHudTimer->totalTime;
                }
                Global::gameScore += std::max(0, 11200 - 20*((int)currTime)); //20 per second
            }

            if (finishTimerBefore < 8.166f && Global::finishStageTimer >= 8.166f)
            {
                Vector3f partVel(0, 0, 0);
                ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 0, 900, 0, true, false, 1.0f, false);

                AudioPlayer::play(25, &Global::gameMainPlayer->position);
            }

            if (Global::finishStageTimer >= 0 &&
                Global::finishStageTimer < 1)
            {
                AudioPlayer::setBGMVolume(1 - Global::finishStageTimer);
            }

            if (finishTimerBefore < 9.166f && Global::finishStageTimer >= 9.166f)
            {
                if (Global::gameIsArcadeMode)
                {
                    Global::gameArcadeIndex+=1;

                    if (Global::gameArcadeIndex < (int)Global::gameArcadeLevelIds.size())
                    {
                        Global::levelID = Global::gameArcadeLevelIds[Global::gameArcadeIndex];
                        Level* currentLevel = &Global::gameLevelData[Global::levelID];
                        Global::shouldLoadLevel = true;
                        Global::isNewLevel = true;
                        Global::levelName = currentLevel->fileName;
                        Global::levelNameDisplay = currentLevel->displayName;
                        Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];
                    
                        Global::createTitleCard();
                    }
                    else
                    {
                        if (Global::gameSaveData.find("BestArcadeClearTime") == Global::gameSaveData.end())
                        {
                            Global::gameSaveData["BestArcadeClearTime"] = std::to_string(Global::gameArcadePlaytime);
                            Global::saveSaveData();
                        }
                        else
                        {
                            float currentPB = std::stof(Global::gameSaveData["BestArcadeClearTime"]);
                            if (Global::gameArcadePlaytime < currentPB)
                            {
                                Global::gameSaveData["BestArcadeClearTime"] = std::to_string(Global::gameArcadePlaytime);
                                Global::saveSaveData();
                            }
                        }

                        AudioPlayer::play(7, Global::gameCamera->getFadePosition1());

                        LevelLoader::loadTitle();
                        Global::gameIsArcadeMode = false;
                    }
                }
                else
                {
                    LevelLoader::loadTitle();
                }
            }

            if (finishTimerBefore < 9.133f && Global::finishStageTimer >= 9.133f)
            {
                GuiManager::clearGuisToRender();
                Global::gameScore = 0;
                Global::gameRingCount = 0;
            }

            if (finishTimerBefore < 8.5f && Global::finishStageTimer >= 8.5f)
            {
                rankDisplay = nullptr;
            }

            if (finishTimerBefore < 6.166f && Global::finishStageTimer >= 6.166f)
            {
                if (Global::gameIsArcadeMode && Global::gameArcadeIndex+1 >= (int)Global::gameArcadeLevelIds.size())
                {
                    MenuManager::arcadeModeIsDone = true;
                }
                int rank = Global::calculateRankAndUpdate();
                rankDisplay = nullptr;
                switch (rank)
                {
                    case 0: rankDisplay = GuiTextureResources::textureRankE; break;
                    case 1: rankDisplay = GuiTextureResources::textureRankD; break;
                    case 2: rankDisplay = GuiTextureResources::textureRankC; break;
                    case 3: rankDisplay = GuiTextureResources::textureRankB; break;
                    case 4: rankDisplay = GuiTextureResources::textureRankA; break;
                    default: break;
                }
                AudioPlayer::play(44, &Global::gameMainPlayer->position);

                Global::saveGhostData();
            }
        }

        if (previousTime > timeNew)
        {
            previousTime = timeNew;
        }

        if (timeNew - previousTime >= 1.0)
        {
            Global::currentCalculatedFPS = (int)(std::round(frameCount/(timeNew - previousTime)));
            //std::fprintf(stdout, "fps: %f\n", frameCount / (timeNew - previousTime));
            //std::fprintf(stdout, "diff: %d\n", Global::countNew - Global::countDelete);
            //Loader::printInfo();
            //std::fprintf(stdout, "entity counts: %d %d %d\n", gameEntities.size(), gameEntitiesPass2.size(), gameTransparentEntities.size());
            frameCount = 0;
            previousTime = timeNew;
        }

        Global::displaySizeChanged = std::max(0, Global::displaySizeChanged - 1);
        if (Global::displaySizeChanged == 1)
        {
            //recreate all fbos and other things to the new size of the window
            //if (Global::renderBloom)
            {
                //Global::gameMultisampleFbo->resize(SCR_WIDTH, SCR_HEIGHT); //memory leaks
                //Global::gameOutputFbo->resize(SCR_WIDTH, SCR_HEIGHT);
                //Global::gameOutputFbo2->resize(SCR_WIDTH, SCR_HEIGHT);
            }
        }
        //std::fprintf(stdout, "dt: %f\n", dt);
    }

    Global::saveSaveData();

    #ifdef DEV_MODE
    listenThread.detach();
    #endif

    Master_cleanUp();
    Loader::cleanUp();
    TextMaster::cleanUp();
    AudioMaster::cleanUp();
    GuiRenderer::cleanUp();
    closeDisplay();

    return 0;
}

//The newEntity should be created with the new keyword, as it will be deleted later
void Main_addEntity(Entity* entityToAdd)
{
    gameEntitiesToAdd.push_back(entityToAdd);
}

void Main_deleteEntity(Entity* entityToDelete)
{
    gameEntitiesToDelete.push_back(entityToDelete);
}

void Main_deleteAllEntites()
{
    //Make sure no entities get left behind in transition
    for (Entity* entityToAdd : gameEntitiesToAdd)
    {
        gameEntities.insert(entityToAdd);
    }
    gameEntitiesToAdd.clear();

    for (Entity* entityToDelete : gameEntitiesToDelete)
    {
        gameEntities.erase(entityToDelete);
        delete entityToDelete; INCR_DEL("Entity");
    }
    gameEntitiesToDelete.clear();


    //Delete all the rest
    for (Entity* entityToDelete : gameEntities)
    {
        delete entityToDelete; INCR_DEL("Entity");
    }
    gameEntities.clear();

    if (Global::gameMainPlayer != nullptr)
    {
        delete Global::gameMainPlayer; INCR_DEL("Entity");
        Global::gameMainPlayer = nullptr;
    }
}

void increaseProcessPriority()
{
    #ifdef _WIN32
    DWORD dwError;

    
    if (!SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS))
    {
        dwError = GetLastError();
        std::fprintf(stdout, "Failed to enter above normal mode (%d)\n", (int)dwError);
    }

    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL))
    {
        dwError = GetLastError();
        std::fprintf(stdout, "Failed to enter above normal mode (%d)\n", (int)dwError);
    }
    

    
    //if (!SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS))
    //{
    //    dwError = GetLastError();
    //    _tprintf(TEXT("Failed to enter below normal mode (%d)\n"), (int)dwError);
    //}
    //
    //if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE))
    //{
    //    dwError = GetLastError();
    //    _tprintf(TEXT("Failed to enter below normal mode (%d)\n"), (int)dwError);
    //}
    

    #endif
}

void Global::checkErrorAL(const char* description)
{
    ALenum erral = alGetError();
    if (erral != AL_NO_ERROR)
    {
        fprintf(stdout, "########  AL ERROR  ########\n");
        fprintf(stdout, "%s     %d\n", description, erral);
    }
}

void Global::loadSaveData()
{
    Global::gameSaveData.clear();

    std::ifstream file(Global::pathToEXE+"res/SaveData/SaveData.sav");
    if (!file.is_open())
    {
        std::fprintf(stdout, "No save data found. Creating new save file...\n");
        file.close();
        Global::saveSaveData();
    }
    else
    {
        std::string line;
        getlineSafe(file, line);

        while (!file.eof())
        {
            char lineBuf[512];
            memcpy(lineBuf, line.c_str(), line.size()+1);

            int splitLength = 0;
            char** lineSplit = split(lineBuf, ';', &splitLength);

            if (splitLength == 2)
            {
                Global::gameSaveData[lineSplit[0]] = lineSplit[1];
            }

            free(lineSplit);

            getlineSafe(file, line);
        }

        file.close();
    }

    if (Global::gameSaveData.find("PLAYTIME") != Global::gameSaveData.end())
    {
        Global::gameTotalPlaytime = std::stoi(Global::gameSaveData["PLAYTIME"]);
    }

    if (Global::gameSaveData.find("CAMERA") != Global::gameSaveData.end())
    {
        if (Global::gameSaveData["CAMERA"] == "AUTO")
        {
            Global::isAutoCam = true;
        }
        else
        {
            Global::isAutoCam = false;
        }
    }
}

void Global::saveGhostData()
{
    #ifdef _WIN32
    _mkdir((Global::pathToEXE + "res/SaveData").c_str());
    #else
    mkdir((Global::pathToEXE + "res/SaveData").c_str(), 0777);
    #endif

    bool newTimeIsFaster = false;

    std::string ghostFilename = Global::pathToEXE + "res/SaveData/" + std::to_string(Global::levelID) + "_" + std::to_string(Global::gameMissionNumber) + ".ghost";

    //Check if we got a faster time than the existing ghost
    std::ifstream filein(ghostFilename);
    if (!filein.is_open())
    {
        filein.close();
        newTimeIsFaster = true;
    }
    else
    {
        float oldTime = 0.0f;
        std::string line;

        while (!filein.eof())
        {
            getlineSafe(filein, line);

            char lineBuf[512];
            memcpy(lineBuf, line.c_str(), line.size()+1);

            int splitLength = 0;
            char** lineSplit = split(lineBuf, ' ', &splitLength);

            if (splitLength == 13)
            {
                oldTime = std::stof(lineSplit[0]);
            }
            free(lineSplit);
        }
        filein.close();

        float newTime = 100000000.0f;

        if (Global::mainHudTimer != nullptr)
        {
            newTime = Global::mainHudTimer->totalTime;
        }

        if (newTime < oldTime)
        {
            newTimeIsFaster = true;
        }
    }

    if (newTimeIsFaster)
    {
        if (Global::raceLogSize > 0)
        {
            std::ofstream raceLogFile;
            raceLogFile.open(ghostFilename, std::ios::out | std::ios::trunc);
            if (!raceLogFile.is_open())
            {
                std::fprintf(stderr, "Error: Failed to create/access '%s'\n", (ghostFilename).c_str());
            }
            else
            {
                for (int i = 0; i < Global::raceLogSize; i++)
                {
                    std::string line = Global::raceLog[i].toString();
                    raceLogFile << line << "\n";
                }
            }
            raceLogFile.close();
            Global::raceLogSize = 0;
        }
    }
}

void Global::saveSaveData()
{
    #ifdef _WIN32
    _mkdir((Global::pathToEXE + "res/SaveData").c_str());
    #else
    mkdir((Global::pathToEXE + "res/SaveData").c_str(), 0777);
    #endif

    std::ofstream file;
    file.open((Global::pathToEXE + "res/SaveData/SaveData.sav").c_str(), std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::fprintf(stderr, "Error: Failed to create/access '%s'\n", (Global::pathToEXE + "res/SaveData/SaveData.sav").c_str());
        file.close();
    }
    else
    {
        Global::gameSaveData["PLAYTIME"] = std::to_string(Global::gameTotalPlaytime);

        if (Global::isAutoCam)
        {
            Global::gameSaveData["CAMERA"] = "AUTO";
        }
        else
        {
            Global::gameSaveData["CAMERA"] = "FREE";
        }

        std::unordered_map<std::string, std::string>::iterator it = Global::gameSaveData.begin();
 
        while (it != Global::gameSaveData.end())
        {
            file << it->first+";"+it->second+"\n";
            
            it++;
        }

        file.close();
    }
}

void Global::increaseRingCount(int rings)
{
    int before = Global::gameRingCount/100;
    Global::gameRingCount += rings;
    int after = Global::gameRingCount/100;

    if (before != after)
    {
        Global::gameLives++;
        AudioPlayer::play(35, Global::gameCamera->getFadePosition1());
    }
}

int Global::calculateRankAndUpdate()
{
    int newRank = 0; //0 = E, 4 = A

    Level* currentLevel = &Global::gameLevelData[Global::levelID];

    if (Global::gameMissionNumber < currentLevel->numMissions)
    {
        std::string missionType = (currentLevel->missionData[Global::gameMissionNumber])[0];

        std::string missionRankString = "ERROR";
        switch (Global::gameMissionNumber)
        {
            case 0: missionRankString = "_M1_RANK"; break;
            case 1: missionRankString = "_M2_RANK"; break;
            case 2: missionRankString = "_M3_RANK"; break;
            case 3: missionRankString = "_M4_RANK"; break;
            default: break;
        }

        std::string missionScoreString = "ERROR";
        switch (Global::gameMissionNumber)
        {
            case 0: missionScoreString = "_M1_SCORE"; break;
            case 1: missionScoreString = "_M2_SCORE"; break;
            case 2: missionScoreString = "_M3_SCORE"; break;
            case 3: missionScoreString = "_M4_SCORE"; break;
            default: break;
        }

        std::string missionTimeString = "ERROR";
        switch (Global::gameMissionNumber)
        {
            case 0: missionTimeString = "_M1_TIME"; break;
            case 1: missionTimeString = "_M2_TIME"; break;
            case 2: missionTimeString = "_M3_TIME"; break;
            case 3: missionTimeString = "_M4_TIME"; break;
            default: break;
        }

        int newScore = Global::gameScore;
        int savedScore = -1;

        if (Global::gameSaveData.find(currentLevel->displayName+missionScoreString) != Global::gameSaveData.end())
        {
            std::string savedScoreString = Global::gameSaveData[currentLevel->displayName+missionScoreString];
            savedScore = std::stoi(savedScoreString);
        }

        if (newScore > savedScore)
        {
            std::string newScoreString = std::to_string(newScore);
            Global::gameSaveData[currentLevel->displayName+missionScoreString] = newScoreString;
            Global::saveSaveData();
        }

        float newTime = 60000.0f;
        if (Global::mainHudTimer != nullptr)
        {
            newTime = Global::mainHudTimer->totalTime;
        }
        float savedTime = 600000.0f;

        if (Global::gameSaveData.find(currentLevel->displayName+missionTimeString) != Global::gameSaveData.end())
        {
            std::string savedTimeString = Global::gameSaveData[currentLevel->displayName+missionTimeString];
            savedTime = std::stof(savedTimeString);
        }

        if (newTime < savedTime)
        {
            std::string newTimeString = std::to_string(newTime);
            Global::gameSaveData[currentLevel->displayName+missionTimeString] = newTimeString;
            Global::saveSaveData();
        }

        if (missionType == "Normal" || missionType == "Hard")
        {
            int scoreForRankA = std::stoi((currentLevel->missionData[Global::gameMissionNumber])[1]);
            int scoreForRankB = (3*scoreForRankA)/4;
            int scoreForRankC = (2*scoreForRankA)/3;
            int scoreForRankD = (1*scoreForRankA)/2;

            if      (newScore >= scoreForRankA) newRank = 4;
            else if (newScore >= scoreForRankB) newRank = 3;
            else if (newScore >= scoreForRankC) newRank = 2;
            else if (newScore >= scoreForRankD) newRank = 1;

            if (newScore > savedScore)
            {
                std::string newRankString = "ERROR";
                switch (newRank)
                {
                    case 0: newRankString = "E"; break;
                    case 1: newRankString = "D"; break;
                    case 2: newRankString = "C"; break;
                    case 3: newRankString = "B"; break;
                    case 4: newRankString = "A"; break;
                    default: break;
                }

                Global::gameSaveData[currentLevel->displayName+missionRankString]  = newRankString;

                Global::saveSaveData();
            }
        }
        else if (missionType == "Ring" || missionType == "Chao" || missionType == "Race")
        {
            float timeForRankA = std::stof((currentLevel->missionData[Global::gameMissionNumber])[1]);
            float timeForRankB = (4*timeForRankA)/3;
            float timeForRankC = (3*timeForRankA)/2;
            float timeForRankD = (2*timeForRankA)/1;

            if      (newTime <= timeForRankA) newRank = 4;
            else if (newTime <= timeForRankB) newRank = 3;
            else if (newTime <= timeForRankC) newRank = 2;
            else if (newTime <= timeForRankD) newRank = 1;

            if (newTime < savedTime)
            {
                std::string newRankString = "ERROR";
                switch (newRank)
                {
                    case 0: newRankString = "E"; break;
                    case 1: newRankString = "D"; break;
                    case 2: newRankString = "C"; break;
                    case 3: newRankString = "B"; break;
                    case 4: newRankString = "A"; break;
                    default: break;
                }

                Global::gameSaveData[currentLevel->displayName+missionRankString] = newRankString;

                Global::saveSaveData();
            }
        }
    }

    return newRank;
}

void doListenThread()
{
#ifdef _WIN32
    DWORD dwError;

    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL))
    {
        dwError = GetLastError();
        _tprintf(TEXT("Failed to enter above normal mode (%d)\n"), (int)dwError);
    }
#endif

    listen();
}

//listen on stdin for coordinates
void listen()
{
    int loop = 1;
    std::string input;

    while (loop == 1)
    {
        getlineSafe(std::cin, input);

        if (input == "exit")
        {
            loop = 0;
        }
        else if (input.size() > 1)
        {
            fprintf(stdout, "input = '%s'\n", input.c_str());
            //Global::gamePlayer->setGroundSpeed(0, 0);
            //Global::gamePlayer->setxVelAir(0);
            //Global::gamePlayer->setxVelAir(0);
            //Global::gamePlayer->setyVel(0);
            if (input.c_str()[0] == 'w')
            {
                const char* data = &input.c_str()[1];
                Global::waterHeight = std::stof(data);
            }
            //if (input == "goff")
            //{
            //    Global::gamePlayer->setGravity(0);
            //}
            //else if (input == "gon")
            //{
            //    Global::gamePlayer->setGravity(0.08f);
            //}
            //else if (input.c_str()[0] == 'x')
            //{
            //    const char* data = &input.c_str()[1];
            //    Global::gamePlayer->setX(std::stof(data));
            //    Global::gamePlayer->setGravity(0);
            //}
            //else if (input.c_str()[0] == 'y')
            //{
            //    const char* data = &input.c_str()[1];
            //    Global::gamePlayer->setY(std::stof(data));
            //    Global::gamePlayer->setGravity(0);
            //}
            //else if (input.c_str()[0] == 'z')
            //{
            //    const char* data = &input.c_str()[1];
            //    Global::gamePlayer->setZ(std::stof(data));
            //    Global::gamePlayer->setGravity(0);
            //}
            //else
            //{
            //    char lineBuf[256];
            //    memcpy(lineBuf, input.c_str(), input.size()+1);
            //
            //    int splitLength = 0;
            //    char** lineSplit = split(lineBuf, ' ', &splitLength);
            //
            //    if (splitLength == 3)
            //    {
            //        Global::gamePlayer->setX(std::stof(lineSplit[0]));
            //        Global::gamePlayer->setY(std::stof(lineSplit[1]));
            //        Global::gamePlayer->setZ(std::stof(lineSplit[2]));
            //        Global::gamePlayer->setGravity(0);
            //    }
            //    free(lineSplit);
            //}
        }
    }
}

//Returns the index of 'gameChunkedEntities' for the (x, z) location
int Global::getChunkIndex(float x, float z)
{
    float relativeX = x - chunkedEntitiesMinX;
    float relativeZ = z - chunkedEntitiesMinZ;

    //const float DIVIDE_OFFSET = 1.0f;

    //ensure the coords arent out of bounds
    //relativeX = std::fmaxf(0.0f, relativeX);
    //relativeX = std::fminf(relativeX, chunkedEntitiesWidth*chunkedEntitiesChunkSize - DIVIDE_OFFSET);

    //relativeZ = std::fmaxf(0.0f, relativeZ);
    //relativeZ = std::fminf(relativeZ, chunkedEntitiesHeight*chunkedEntitiesChunkSize - DIVIDE_OFFSET);

    //calculate 2d array indices
    int iX = (int)(relativeX/chunkedEntitiesChunkSize);
    int iZ = (int)(relativeZ/chunkedEntitiesChunkSize);

    iX = std::max(0, iX);
    iX = std::min(iX, chunkedEntitiesWidth-1);

    iZ = std::max(0, iZ);
    iZ = std::min(iZ, chunkedEntitiesHeight-1);

    //calculate index in gameNearbyEntities that corresponds to iX, iZ
    int realIndex = iX + iZ*chunkedEntitiesWidth;

    if (realIndex >= (int)gameChunkedEntities.size())
    {
        std::fprintf(stderr, "Error: Index out of bounds on gameNearbyEntities. THIS IS VERY BAD.\n");
        std::fprintf(stdout, "    x = %f       z = %f\n", x, z);
        std::fprintf(stdout, "    relativeX = %f      relativeZ = %f\n", relativeX, relativeZ);
        std::fprintf(stdout, "    iX = %d        iZ = %d\n", iX, iZ);
        std::fprintf(stdout, "    chunkedEntitiesWidth = %d             chunkedEntitiesHeight = %d\n", chunkedEntitiesWidth, chunkedEntitiesHeight);
        std::fprintf(stdout, "    chunkedEntitiesChunkSize = %f\n", chunkedEntitiesChunkSize);
        std::fprintf(stdout, "    realIndex = %d          gameChunkedEntities.size() = %d\n", realIndex, (int)gameChunkedEntities.size());
        return 0;
    }

    return realIndex;
}

void Global::getNearbyEntities(float x, float z, int renderDistance, std::list<std::unordered_set<Entity*>*>* list)
{
    list->clear();

    switch (renderDistance)
    {
        case 0:
        {
            list->push_back(&gameChunkedEntities[Global::getChunkIndex(x, z)]);
            break;
        }

        case 1:
        {
            std::unordered_set<int> chunkIdxs;
            float w = chunkedEntitiesChunkSize/2;
            chunkIdxs.insert(Global::getChunkIndex(x-w, z-w));
            chunkIdxs.insert(Global::getChunkIndex(x+w, z-w));
            chunkIdxs.insert(Global::getChunkIndex(x-w, z+w));
            chunkIdxs.insert(Global::getChunkIndex(x+w, z+w));
            for (int i : chunkIdxs)
            {
                list->push_back(&gameChunkedEntities[i]);
            }
            break;
        }

        case 2:
        {
            std::unordered_set<int> chunkIdxs;
            float w = chunkedEntitiesChunkSize;
            chunkIdxs.insert(Global::getChunkIndex(x-w, z-w));
            chunkIdxs.insert(Global::getChunkIndex(x+0, z-w));
            chunkIdxs.insert(Global::getChunkIndex(x+w, z-w));
            chunkIdxs.insert(Global::getChunkIndex(x-w, z+0));
            chunkIdxs.insert(Global::getChunkIndex(x+0, z+0));
            chunkIdxs.insert(Global::getChunkIndex(x+w, z+0));
            chunkIdxs.insert(Global::getChunkIndex(x-w, z+w));
            chunkIdxs.insert(Global::getChunkIndex(x+0, z+w));
            chunkIdxs.insert(Global::getChunkIndex(x+w, z+w));
            for (int i : chunkIdxs)
            {
                list->push_back(&gameChunkedEntities[i]);
            }
            break;
        }

        default:
        {
            std::fprintf(stderr, "Error: Render distance not out of range.\n");
            break;
        }
    }
}

//Return a list of nearby entity sets. Returns either 1 chunk, 2 chunks, or 4 chunks,
// depending on the minDistance value.
void Global::getNearbyEntities(float x, float z, std::list<std::unordered_set<Entity*>*>* list, float minDistance)
{
    list->clear();

    std::unordered_set<int> chunkIdxs;
    float w = minDistance;
    chunkIdxs.insert(Global::getChunkIndex(x-w, z-w));
    chunkIdxs.insert(Global::getChunkIndex(x+w, z-w));
    chunkIdxs.insert(Global::getChunkIndex(x-w, z+w));
    chunkIdxs.insert(Global::getChunkIndex(x+w, z+w));
    for (int i : chunkIdxs)
    {
        list->push_back(&gameChunkedEntities[i]);
    }
}

void Global::recalculateEntityChunks(float minX, float maxX, float minZ, float maxZ, float chunkSize)
{
    if (gameChunkedEntities.size() != 0)
    {
        std::fprintf(stderr, "Error: Trying to recalculate entity chunks when gameChunkedEntities is not 0.\n");
        return;
    }

    float xDiff = maxX - minX;
    float zDiff = maxZ - minZ;

    int newWidth  = (int)(xDiff/chunkSize) + 1;
    int newHeight = (int)(zDiff/chunkSize) + 1;

    int count = newWidth*newHeight;
    for (int i = 0; i < count; i++)
    {
        std::unordered_set<Entity*> set;
        gameChunkedEntities.push_back(set);
    }

    chunkedEntitiesMinX = minX;
    chunkedEntitiesMinZ = minZ;
    chunkedEntitiesChunkSize = chunkSize;
    chunkedEntitiesWidth = newWidth;
    chunkedEntitiesHeight = newHeight;
}

void Main_addChunkedEntity(Entity* entityToAdd)
{
    gameChunkedEntitiesToAdd.push_back(entityToAdd);
}

void Main_deleteChunkedEntity(Entity* entityToAdd)
{
    gameChunkedEntitiesToDelete.push_back(entityToAdd);
}

void Main_deleteAllChunkedEntities()
{
    //Make sure no entities get left behind in transition
    for (Entity* entityToAdd : gameChunkedEntitiesToAdd)
    {
        delete entityToAdd; INCR_DEL("Entity");
    }
    gameChunkedEntitiesToAdd.clear();

    gameChunkedEntitiesToDelete.clear();

    for (std::unordered_set<Entity*> set : gameChunkedEntities)
    {
        for (Entity* e : set)
        {
            delete e; INCR_DEL("Entity");
        }
        set.clear();
    }
    gameChunkedEntities.clear();
}

void Global::createTitleCard()
{
    Global::startStageTimer = 1.0f;

    ParticleMaster::deleteAllParticles();
    GuiManager::clearGuisToRender();

    Vector3f vel(0,0,0);
    ParticleMaster::createParticle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &vel, 0, 1.0f, 0.0f, 50.0f, 0, true, false, 1.0f, false);
    GuiManager::addGuiToRender(GuiTextureResources::textureBlueLine);

    Global::clearTitleCard();

    titleCardLevelName          = new GUIText(Global::levelNameDisplay, 0.09f, Global::fontVipnagorgialla, 0.5f, 0.6f, 4, true); INCR_NEW("GUIText");
    titleCardMission            = new GUIText("Mission "+std::to_string(Global::gameMissionNumber+1)+":", 0.075f, Global::fontVipnagorgialla, 0.5f, 0.7f, 4, true); INCR_NEW("GUIText");
    //titleCardMissionDescription = new GUIText(Global::gameMissionDescription, 0.06f, Global::fontVipnagorgialla, 0.5f, 0.8f, 4, true); INCR_NEW("GUIText");
    titleCardMissionDescription = new GUIText(Global::gameMissionDescription, 0.06f, Global::fontVipnagorgialla, 0.0f, 0.8f, 1.0f, true, false, true); INCR_NEW("GUIText");
    
    //GUIText::GUIText(std::string text, float fontSize, FontType* font, float x, float y, float maxLineLength,
    //bool centered, bool rightAligned, bool visible)
}

void Global::clearTitleCard()
{
    if (titleCardLevelName != nullptr)
    {
        titleCardLevelName->deleteMe();
        delete titleCardLevelName; INCR_DEL("GUIText");
        titleCardLevelName = nullptr;
    }
    if (titleCardMission != nullptr)
    {
        titleCardMission->deleteMe();
        delete titleCardMission; INCR_DEL("GUIText");
        titleCardMission = nullptr;
    }
    if (titleCardMissionDescription != nullptr)
    {
        titleCardMissionDescription->deleteMe();
        delete titleCardMissionDescription; INCR_DEL("GUIText");
        titleCardMissionDescription = nullptr;
    }
}

std::unordered_map<std::string, int> heapObjects;

void Global::debugNew(const char* name)
{
    Global::countNew++;

    #ifdef DEV_MODE
    if (heapObjects.find(name) == heapObjects.end())
    {
        heapObjects[name] = 1;
    }
    else
    {
        int num = heapObjects[name];
        heapObjects[name] = num+1;
    }
    #else
    name;
    #endif
}

void Global::debugDel(const char* name)
{
    Global::countDelete++;

    #ifdef DEV_MODE
    if (heapObjects.find(name) == heapObjects.end())
    {
        std::fprintf(stdout, "Warning: trying to delete '%s' when there are none.\n", name);
        heapObjects[name] = 0;
    }
    else
    {
        int num = heapObjects[name];
        heapObjects[name] = num-1;
    }
    #else
    name;
    #endif
}
