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
#include <utility>

#include <chrono>
#include <ctime>
#include <random>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "main.hpp"
#include "../renderEngine/masterrenderer.hpp"
#include "../renderEngine/display.hpp"
#include "../loading/loadergl.hpp"
#include "../toolbox/input.hpp"
#include "../models/models.hpp"
#include "../shaders/entityshader.hpp"
#include "../textures/modeltexture.hpp"
#include "../entities/entity.hpp"
#include "../toolbox/vector.hpp"
#include "../toolbox/matrix.hpp"
#include "../entities/camera.hpp"
#include "../loading/modelloader.hpp"
#include "../entities/light.hpp"
#include "../entities/stage.hpp"
#include "../loading/levelloader.hpp"
#include "../collision/collisionchecker.hpp"
#include "../entities/skysphere.hpp"
#include "../renderEngine/skymanager.hpp"
#include "../animation/animationresources.hpp"
#include "../fontRendering/textmaster.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../guis/guimanager.hpp"
#include "../audio/audiomaster.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particlemaster.hpp"
#include "../particles/particleresources.hpp"
#include "../toolbox/split.hpp"
#include "../postProcessing/postprocessing.hpp"
#include "../postProcessing/fbo.hpp"
#include "../guis/guirenderer.hpp"
#include "../guis/guiimageresources.hpp"
#include "../toolbox/level.hpp"
#include "../guis/guiimage.hpp"
#include "../particles/particle.hpp"
#include "../entities/skysphere.hpp"
#include "../fontMeshCreator/guinumber.hpp"
#include "../entities/controllableplayer.hpp"
#include "../entities/checkpoint.hpp"
#include "../water/waterframebuffers.hpp"
#include "../water/watershader.hpp"
#include "../water/waterrenderer.hpp"
#include "../water/watertile.hpp"
#include "../toolbox/getline.hpp"
#include "../menu/menumanager.hpp"
#include "../menu/mainmenu.hpp"
#include "../menu/timer.hpp"
#include "../menu/hud.hpp"
#include "../toolbox/format.hpp"
#include "../entities/TwinkleCircuit/tckart.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/eggpawn.hpp"
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include "../toolbox/usleep.hpp"
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
Camera*             Global::gameCamera       = nullptr;
ControllablePlayer* Global::gameMainPlayer   = nullptr;
TC_Kart*            Global::gameKart         = nullptr;
Entity*             Global::gameStageManager = nullptr;
Stage*              Global::gameStage        = nullptr;
SkySphere*          Global::gameSkySphere    = nullptr;
Light*              Global::gameLightSun     = nullptr;
Light*              Global::gameLightMoon    = nullptr;

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

//extern bool INPUT_JUMP;
//extern bool INPUT_ACTION;
//extern bool INPUT_ACTION2;
//
//extern bool INPUT_PREVIOUS_JUMP;
//extern bool INPUT_PREVIOUS_ACTION;
//extern bool INPUT_PREVIOUS_ACTION2;

int Global::countNew = 0;
int Global::countDelete = 0;
int Global::gameState = 0;
int Global::levelId = 0;
float Global::startStageTimer = -1;
bool Global::shouldLoadLevel = false;
bool Global::isNewLevel = false;
bool Global::isAutoCam = true;
std::string Global::levelName = "";
std::string Global::levelNameDisplay = "";
int Global::gameRingCount = 0;
int Global::gameDebugRingCount = 0;
int Global::gameScore = 0;
int Global::gameLives = 4;
float Global::gameClock = 0.0f;
int Global::gameTotalPlaytime = 0;
int Global::gameAccumulatedRings = 0;
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
Vector3f Global::stageWaterColor(0,0,0);
float Global::stageWaterBlendAmount = 0.0f;
float Global::stageWaterMurkyAmount = 0.0f;
FontType* Global::fontVipnagorgialla = nullptr;
bool Global::renderWithCulling = true;
bool Global::displayFPS = true;
bool Global::displaySpeedometer = true;
//float Global::fpsTarget = 120.0f;
float Global::fpsLimit = 60.0f;
int Global::currentCalculatedFPS = 0;
int Global::renderCount = 0;
int Global::displaySizeChanged = 0;

Global::PlayableCharacter Global::currentCharacterType = Global::PlayableCharacter::Sonic;
std::unordered_map<Global::PlayableCharacter, std::string> Global::characterNames;

int Global::gameArcadeIndex = 0;
std::vector<std::pair<int, Global::PlayableCharacter>> Global::gameArcadeLevelIds; //level ids and the character id

std::vector<int> Global::gameLevelIdsSonic;
std::vector<int> Global::gameLevelIdsTails;
std::vector<int> Global::gameLevelIdsKnuckles;

//std::list<std::string> Global::raceLog;
bool Global::shouldLogRace = false;

int Global::raceLogSize = 0;
GhostFrame Global::raceLog[Global::raceLogSizeMax]; //enough for 10 minutes at 720fps

bool Global::spawnAtCheckpoint = false;
Vector3f Global::checkpointPlayerPos;
Vector3f Global::checkpointPlayerDir;
Vector3f Global::checkpointCamDir;
float Global::checkpointTime = 0;

GUIText* Global::titleCardLevelName          = nullptr;
GUIText* Global::titleCardMission            = nullptr;
GUIText* Global::titleCardMissionDescription = nullptr;

std::unordered_map<int, int> Global::stageNpcCounts;

void increaseProcessPriority();

void doListenThread();

void listen();

MenuManager      Global::menuManager;
MainMenu*        Global::menuMain            = nullptr;
CharacterSelect* Global::menuCharacterSelect = nullptr;
MissionMenu*     Global::menuMission         = nullptr;
ConfigMenu*      Global::menuConfig          = nullptr;
Extras*          Global::menuExtras          = nullptr;
ResultsScreen*   Global::menuResults         = nullptr;
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

    Display::createDisplay();

    Global::characterNames[Global::PlayableCharacter::Sonic] = "Sonic";
    Global::characterNames[Global::PlayableCharacter::Tails] = "Tails";
    Global::characterNames[Global::PlayableCharacter::Knuckles] = "Knuckles";

    Global::loadSaveData();

    //The levels you play in arcade mode, in order
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_TUTORIAL,        Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_DRY_LAGOON,      Global::PlayableCharacter::Knuckles));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_CLOUD_TEMPLE,    Global::PlayableCharacter::Tails));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_GREEN_FOREST,    Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_METAL_HARBOR,    Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_NOKI_BAY,        Global::PlayableCharacter::Knuckles));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_FROG_FOREST,     Global::PlayableCharacter::Tails));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_DRAGON_ROAD,     Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_GREEN_HILL_ZONE, Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_TWINKLE_CIRCUIT, Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_DELFINO_PLAZA,   Global::PlayableCharacter::Knuckles));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_SKY_RAIL,        Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_RADICAL_HIGHWAY, Global::PlayableCharacter::Sonic));
    Global::gameArcadeLevelIds.push_back(std::make_pair(LVL_SACRED_SKY,      Global::PlayableCharacter::Sonic));

    Global::gameLevelIdsSonic.push_back(LVL_TUTORIAL);
    Global::gameLevelIdsSonic.push_back(LVL_DRAGON_ROAD);
    Global::gameLevelIdsSonic.push_back(LVL_GREEN_FOREST);
    Global::gameLevelIdsSonic.push_back(LVL_METAL_HARBOR);
    Global::gameLevelIdsSonic.push_back(LVL_SKY_RAIL);
    Global::gameLevelIdsSonic.push_back(LVL_PYRAMID_CAVE);
    Global::gameLevelIdsSonic.push_back(LVL_RADICAL_HIGHWAY);
    Global::gameLevelIdsSonic.push_back(LVL_GREEN_HILL_ZONE);
    Global::gameLevelIdsSonic.push_back(LVL_CITY_ESCAPE);
    Global::gameLevelIdsSonic.push_back(LVL_WINDY_VALLEY);
    Global::gameLevelIdsSonic.push_back(LVL_SEASIDE_HILL);
    Global::gameLevelIdsSonic.push_back(LVL_FROG_FOREST);
    Global::gameLevelIdsSonic.push_back(LVL_TEST);
    Global::gameLevelIdsSonic.push_back(LVL_SPEED_HIGHWAY);
    Global::gameLevelIdsSonic.push_back(LVL_SACRED_SKY);
    Global::gameLevelIdsSonic.push_back(LVL_TWINKLE_CIRCUIT);
    Global::gameLevelIdsSonic.push_back(LVL_EMERALD_COAST);

    Global::gameLevelIdsTails.push_back(LVL_TUTORIAL);
    Global::gameLevelIdsTails.push_back(LVL_GREEN_FOREST);
    Global::gameLevelIdsTails.push_back(LVL_METAL_HARBOR);
    Global::gameLevelIdsTails.push_back(LVL_PYRAMID_CAVE);
    Global::gameLevelIdsTails.push_back(LVL_RADICAL_HIGHWAY);
    Global::gameLevelIdsTails.push_back(LVL_GREEN_HILL_ZONE);
    Global::gameLevelIdsTails.push_back(LVL_CITY_ESCAPE);
    Global::gameLevelIdsTails.push_back(LVL_WINDY_VALLEY);
    Global::gameLevelIdsTails.push_back(LVL_SEASIDE_HILL);
    Global::gameLevelIdsTails.push_back(LVL_FROG_FOREST);
    Global::gameLevelIdsTails.push_back(LVL_TEST);
    Global::gameLevelIdsTails.push_back(LVL_SPEED_HIGHWAY);
    Global::gameLevelIdsTails.push_back(LVL_SACRED_SKY);
    Global::gameLevelIdsTails.push_back(LVL_CLOUD_TEMPLE);
    Global::gameLevelIdsTails.push_back(LVL_EMERALD_COAST);

    Global::gameLevelIdsKnuckles.push_back(LVL_DRY_LAGOON);
    Global::gameLevelIdsKnuckles.push_back(LVL_DELFINO_PLAZA);
    Global::gameLevelIdsKnuckles.push_back(LVL_NOKI_BAY);
    Global::gameLevelIdsKnuckles.push_back(LVL_FREEZEEZY_PEAK);
    Global::gameLevelIdsKnuckles.push_back(LVL_SWEET_MOUNTAIN);

    //create NPC list
    Global::stageNpcCounts[LVL_SKY_RAIL]        = 3;
    Global::stageNpcCounts[LVL_DRY_LAGOON]      = 1;
    Global::stageNpcCounts[LVL_METAL_HARBOR]    = 4;
    Global::stageNpcCounts[LVL_RADICAL_HIGHWAY] = 5;
    Global::stageNpcCounts[LVL_DRAGON_ROAD]     = 7;
    Global::stageNpcCounts[LVL_GREEN_HILL_ZONE] = 1;
    Global::stageNpcCounts[LVL_DELFINO_PLAZA]   = 3;
    Global::stageNpcCounts[LVL_EMERALD_COAST]   = 5;

    #if !defined(DEV_MODE) && defined(_WIN32)
    //FreeConsole(); //dont do this anymore, just change the subsystem to Windows instead of Console
    #endif

    LoaderGL::init();

    Input::init();

    //This camera is never deleted.
    Camera cam;
    Global::gameCamera = &cam;

    MasterRenderer::init();

    LevelLoader::loadLevelData();

    AudioMaster::init();

    Global::fontVipnagorgialla = new FontType(LoaderGL::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt"); INCR_NEW("FontType");

    TextMaster::init();

    GuiManager::init();

    Global::menuMain = new MainMenu; INCR_NEW("Menu");
    Global::menuManager.push(Global::menuMain);

    //if (Global::renderParticles)
    {
        ParticleResources::loadParticles();
    }

    GuiImageResources::loadGuiImages();

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

    //lightSun.getPosition()->x = 0;
    //lightSun.getPosition()->y = 0;
    //lightSun.getPosition()->z = 0;
    //lightMoon.getPosition()->y = -100000;

    if (Global::useHighQualityWater)
    {
        Global::gameWaterFBOs     = new WaterFrameBuffers; INCR_NEW("WaterFrameBuffers");
        WaterShader* waterShader  = new WaterShader; INCR_NEW("WaterShader");
        Global::gameWaterRenderer = new WaterRenderer(waterShader, MasterRenderer::projectionMatrix, Global::gameWaterFBOs); INCR_NEW("WaterRenderer");
    }

    if (Global::renderBloom)
    {
        Global::gameMultisampleFbo = new Fbo(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT); INCR_NEW("Fbo");
        Global::gameOutputFbo      = new Fbo(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT, Fbo::DEPTH_TEXTURE); INCR_NEW("Fbo");
        Global::gameOutputFbo2     = new Fbo(Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT, Fbo::DEPTH_TEXTURE); INCR_NEW("Fbo");
        PostProcessing::init();
    }

    ParticleMaster::init(MasterRenderer::projectionMatrix);

    long long secSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    glfwSetTime(0);

    int frameCount = 0;
    double previousTime = 0;

    Global::gameState = STATE_TITLE;

    GuiImage* rankDisplay = nullptr;

    LevelLoader::loadTitle();

    timeNew = 0.0;
    timeOld = 0.0;
    dt = 0.016f;

    //extern GLuint transparentDepthTexture;
    //GuiImage* debugDepth = new GuiImage(transparentDepthTexture, 0.2f, 0.8f, 0.3f, 0.3f, 0); INCR_NEW("GuiImage");

    std::list<std::unordered_set<Entity*>*> entityChunkedList;

    //GUIText* debugNumber = new GUIText("a", 0.04f, Global::fontVipnagorgialla, 0, 0, 0, true);

    while (Global::gameState != STATE_EXITING && Display::displayWantsToClose() == 0)
    {
        ANALYSIS_START("Frame Time");

        timeNew = glfwGetTime();

        ANALYSIS_START("Frame DT Spin");

        #ifndef _WIN32
        //spin lock to meet the target fps, and gives extremely consistent dt's.
        // also of course uses a ton of cpu.
        if (Global::gameState == STATE_RUNNING && Global::framerateUnlock)
        {
            if (Global::fpsLimit > 0.0f)
            {
                double dtFrameNeedsToTake = 1.0/((double)Global::fpsLimit);
                while ((timeNew - timeOld) < dtFrameNeedsToTake)
                {
                    timeNew = glfwGetTime();
                }
            }
        }
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
            if (Global::fpsLimit > 0.0f)
            {
                double dtFrameNeedsToTake = 1.0/((double)Global::fpsLimit);
                timeNew = glfwGetTime();

                bool slept = false;
        
                const double sleepBuffer = 0.002; //sleep will hopefully never take longer than this to return (2 milliseconds)
                double sleepTime = (dtFrameNeedsToTake - (timeNew - timeOld)) - sleepBuffer;
                int msToSleep = (int)(sleepTime*1000);
                if (msToSleep >= 1)
                {
                    usleep(msToSleep*1000);
                    slept = true;
                }

                bool spinned = false;
        
                timeNew = glfwGetTime();
                while ((timeNew - timeOld) < dtFrameNeedsToTake)
                {
                    timeNew = glfwGetTime();
                    spinned = true;
                }

                if (slept && !spinned)
                {
                    printf("slept for too long! (%f seconds too much)\n", (timeNew - timeOld) - dtFrameNeedsToTake);
                }
                else if (!slept && !spinned)
                {
                    //printf("frame has already taken too long!\n");
                }
            }
        }
        #endif

        dt = (float)(timeNew - timeOld);
        dt = std::fminf(dt, 0.04f); //Anything lower than 25fps will slow the gameplay down
        //dt *= 0.25f;
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

        ANALYSIS_DONE("Frame DT Spin");

        #ifdef DEV_MODE
        ANALYSIS_START("ERROR CHECK GL");
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("########  GL ERROR  ########\n");
            printf("%d\n", err);
        }
        ANALYSIS_DONE("ERROR CHECK GL");

        ANALYSIS_START("ERROR CHECK AL");
        ALenum erral = alGetError();
        if (erral != AL_NO_ERROR)
        {
            printf("########  AL ERROR  ########\n");
            printf("%d\n", erral);
        }
        ANALYSIS_DONE("ERROR CHECK AL");
        #endif

        //long double thisTime = std::time(0);
        //printf("time: %f time\n", thisTime);

        ANALYSIS_START("Entity Management");
        //entities managment
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

        //chunked entities mamanegement
        for (Entity* entityToAdd : gameChunkedEntitiesToAdd)
        {
            int realIndex = Global::getChunkIndex(entityToAdd->getX(), entityToAdd->getZ());
            gameChunkedEntities[realIndex].insert(entityToAdd);
        }
        gameChunkedEntitiesToAdd.clear();

        for (Entity* entityToDelete : gameChunkedEntitiesToDelete)
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
                    printf("Error: Tried to delete a chunked entity that wasn't in the lists.\n");
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
        ANALYSIS_DONE("Entity Management");

        ANALYSIS_START("Object Logic");

        cam.inWater = false;
        EggPawn::pawns.clear();

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

                //printf("\nbefore player %f\n", Global::gameCamera->eye.x);
                if (Global::gameMainPlayer != nullptr)
                {
                    Global::gameMainPlayer->step();
                }
                //printf("after player %f\n", Global::gameCamera->eye.x);
                for (Entity* e : gameEntities)
                {
                    ///printf("    new entity %f  %s\n", Global::gameCamera->eye.x, typeid(*e).name());
                    e->step();
                }
                //printf("after kart %f\n", Global::gameCamera->eye.x);
                if (gameChunkedEntities.size() > 0)
                {
                    //int chunkCt = 0;
                    Global::getNearbyEntities(cam.eye.x, cam.eye.z, 2, &entityChunkedList);
                    for (std::unordered_set<Entity*>* entitySet : entityChunkedList)
                    {
                        for (Entity* e : (*entitySet))
                        {
                            e->step();
                            //chunkCt++;
                        }
                    }
                    //printf("%d\n", chunkCt);
                }
                if (Global::gameStageManager != nullptr)
                {
                    Global::gameStageManager->step();
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

                skySphere.step();
                ModelTexture::updateAnimations(dt);
                Global::gameCamera->refresh();
                ParticleMaster::update(Global::gameCamera);
                Global::gameClock+=dt;

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

        ANALYSIS_DONE("Object Logic");

        ANALYSIS_START("Object Render");

        //prepare entities to render
        for (Entity* e : gameEntities)
        {
            MasterRenderer::processEntity(e);
        }
        if (gameChunkedEntities.size() > 0)
        {
            for (std::unordered_set<Entity*>* entitySet : entityChunkedList)
            {
                for (Entity* e : (*entitySet))
                {
                    MasterRenderer::processEntity(e);
                }
            }
        }

        if (Global::gameStageManager != nullptr)
        {
            MasterRenderer::processEntity(Global::gameStageManager);
        }
        MasterRenderer::processEntity(&stage);
        //MasterRenderer::processEntity(&skySphere);

        float waterBlendAmount = 0.0f;
        if (cam.inWater || (cam.eye.y < Global::waterHeight && Global::stageUsesWater))
        {
            waterBlendAmount = Global::stageWaterBlendAmount;
        }

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
                MasterRenderer::render(&cam, 0, 1, 0, offsetWater - Global::waterHeight, waterBlendAmount);
                if (Global::renderParticles)
                {
                    ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), 1);
                }
            }
            else
            {
                MasterRenderer::render(&cam, 0, -1, 0, offsetWater + Global::waterHeight, waterBlendAmount);
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
                MasterRenderer::render(&cam, 0, -1, 0, offsetWater + Global::waterHeight, waterBlendAmount);
                if (Global::renderParticles)
                {
                    ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), -1);
                }
            }
            else
            {
                MasterRenderer::render(&cam, 0, 1, 0, offsetWater - Global::waterHeight, waterBlendAmount);
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
        AudioPlayer::setListenerIsUnderwater(cam.inWater || (cam.eye.y < Global::waterHeight && Global::stageUsesWater));

        if (Global::renderBloom)
        {
            Global::gameMultisampleFbo->bindFrameBuffer();
        }
        MasterRenderer::render(&cam, 0, 0, 0, 0, waterBlendAmount);
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
            PostProcessing::doPostProcessing(Global::gameOutputFbo->getColorTexture(), Global::gameOutputFbo2->getColorTexture());
        }

        MasterRenderer::clearAllEntities();

        ANALYSIS_DONE("Object Render");

        if (rankDisplay != nullptr)
        {
            GuiManager::addImageToRender(rankDisplay);
        }

        //debugNumber->deleteMe();
        //debugNumber = new GUIText(std::to_string(Global::renderCount), 0.04f, Global::fontVipnagorgialla, 0, 0, 0, true);

        //GuiManager::addGuiToRender(debugDepth);

        ANALYSIS_START("Screen Refresh");

        GuiManager::refresh();
        GuiManager::clearGuisToRender();
        TextMaster::render();

        //int sleepFor = (int)(8000*Maths::random());
        //usleep(sleepFor);

        //timeNew = glfwGetTime();
        ////spin lock to meet the target fps, and gives extremely consistent dt's.
        //// also of course uses a ton of cpu.
        //if (Global::gameState == STATE_RUNNING && Global::framerateUnlock)
        //{
        //    if (Global::fpsLimit > 0.0f)
        //    {
        //        double dtFrameNeedsToTake = 1.0/((double)Global::fpsLimit);
        //        while ((timeNew - timeOld) < dtFrameNeedsToTake)
        //        {
        //            timeNew = glfwGetTime();
        //        }
        //    }
        //}

        Display::updateDisplay();

        ANALYSIS_DONE("Screen Refresh");

        //dt = (float)(timeNew - timeOld);
        //dt = std::fminf(dt, 0.04f); //Anything lower than 25fps will slow the gameplay down
        ////dt *= 0.5f;
        //timeOld = timeNew;

        ANALYSIS_START("Audio Refresh");

        AudioPlayer::refreshBGM();

        ANALYSIS_DONE("Audio Refresh");

        ANALYSIS_START("Misc End Stuff");

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
                ParticleMaster::createParticle(ParticleResources::textureWhiteFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 900, 0, true, false, 1.0f, false);
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
                ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 900, 0, true, false, 1.0f, false);

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
                        Global::levelId = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).first;
                        Global::currentCharacterType = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).second;
                        Level* currentLevel = &Global::gameLevelData[Global::levelId];
                        Global::shouldLoadLevel = true;
                        Global::isNewLevel = true;
                        Global::levelName = currentLevel->fileName;
                        Global::levelNameDisplay = currentLevel->displayName;
                        Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];
                    
                        Global::createTitleCard();
                    }
                    else
                    {
                        if (Global::gameSaveData.find("BestArcadeClearTime_"+Global::characterNames[Global::currentCharacterType]) == Global::gameSaveData.end())
                        {
                            Global::gameSaveData["BestArcadeClearTime_"+Global::characterNames[Global::currentCharacterType]] = std::to_string(Global::gameArcadePlaytime);
                            Global::saveSaveData();
                        }
                        else
                        {
                            float currentPB = std::stof(Global::gameSaveData["BestArcadeClearTime_"+Global::characterNames[Global::currentCharacterType]]);
                            if (Global::gameArcadePlaytime < currentPB)
                            {
                                Global::gameSaveData["BestArcadeClearTime_"+Global::characterNames[Global::currentCharacterType]] = std::to_string(Global::gameArcadePlaytime);
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
                    printf("setting MenuManager::arcadeModeIsDone to true\n");
                    MenuManager::arcadeModeIsDone = true;
                }
                int rank = Global::calculateRankAndUpdate();
                rankDisplay = nullptr;
                switch (rank)
                {
                    case 0: rankDisplay = GuiImageResources::textureRankE; break;
                    case 1: rankDisplay = GuiImageResources::textureRankD; break;
                    case 2: rankDisplay = GuiImageResources::textureRankC; break;
                    case 3: rankDisplay = GuiImageResources::textureRankB; break;
                    case 4: rankDisplay = GuiImageResources::textureRankA; break;
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
            //printf("fps: %f\n", frameCount / (timeNew - previousTime));
            //printf("diff: %d\n", Global::countNew - Global::countDelete);
            //Loader::printInfo();
            //printf("entity counts: %d %d %d\n", gameEntities.size(), gameEntitiesPass2.size(), gameTransparentEntities.size());
            frameCount = 0;
            previousTime = timeNew;
        }

        Global::displaySizeChanged = std::max(0, Global::displaySizeChanged - 1);
        if (Global::displaySizeChanged == 1)
        {
            //recreate all fbos and other things to the new size of the window
            //if (Global::renderBloom)
            {
                //Global::gameMultisampleFbo->resize(SCR_WIdTH, SCR_HEIGHT); //memory leaks
                //Global::gameOutputFbo->resize(SCR_WIdTH, SCR_HEIGHT);
                //Global::gameOutputFbo2->resize(SCR_WIdTH, SCR_HEIGHT);
            }
        }
        //printf("dt: %f\n", dt);

        ANALYSIS_DONE("Misc End Stuff");

        ANALYSIS_DONE("Frame Time");
        ANALYSIS_REPORT();
    }

    Global::saveSaveData();

    #ifdef DEV_MODE
    listenThread.detach();
    #endif

    MasterRenderer::cleanUp();
    LoaderGL::cleanUp();
    TextMaster::cleanUp();
    AudioMaster::cleanUp();
    GuiRenderer::cleanUp();
    Input::cleanUp();
    Display::closeDisplay();

    return 0;
}

void Global::addEntity(Entity* entityToAdd)
{
    gameEntitiesToAdd.push_back(entityToAdd);
}

void Global::deleteEntity(Entity* entityToDelete)
{
    gameEntitiesToDelete.push_back(entityToDelete);
}

void Global::deleteAllEntites()
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

    if (Global::gameStageManager != nullptr)
    {
        delete Global::gameStageManager; INCR_DEL("Entity");
        Global::gameStageManager = nullptr;
    }
}

void increaseProcessPriority()
{
    #ifdef _WIN32
    DWORD dwError;

    
    if (!SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS))
    {
        dwError = GetLastError();
        printf("Failed to enter above normal mode (%d)\n", (int)dwError);
    }

    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL))
    {
        dwError = GetLastError();
        printf("Failed to enter above normal mode (%d)\n", (int)dwError);
    }
    

    
    //if (!SetPriorityClass(GetCurrentProcess(), IdLE_PRIORITY_CLASS))
    //{
    //    dwError = GetLastError();
    //    _tprintf(TEXT("Failed to enter below normal mode (%d)\n"), (int)dwError);
    //}
    //
    //if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IdLE))
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
        printf("########  AL ERROR  ########\n");
        printf("%s     %d\n", description, erral);
    }
}

void Global::loadSaveData()
{
    Global::gameSaveData.clear();

    std::ifstream file(Global::pathToEXE+"res/SaveData/SaveData.sav");
    if (!file.is_open())
    {
        printf("No save data found. Creating new save file...\n");
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

    if (Global::gameSaveData.find("RINGS") != Global::gameSaveData.end())
    {
        Global::gameAccumulatedRings = std::stoi(Global::gameSaveData["RINGS"]);
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

    std::string ghostFilename = Global::pathToEXE + 
        "res/SaveData/" + 
        Global::characterNames[Global::currentCharacterType] + "_" +
        std::to_string(Global::levelId) + "_" + 
        std::to_string(Global::gameMissionNumber) + 
        ".ghost";

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
                printf("Error: Failed to create/access '%s'\n", (ghostFilename).c_str());
            }
            else
            {
                for (int i = 0; i < Global::raceLogSize; i++)
                {
                    //Global::raceLog[i].pos = Global::raceLog[i].pos + Global::raceLog[i].up.scaleCopy(5.5f);
                    std::string line = Global::raceLog[i].toString();
                    //raceLogFile.write()
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
        printf("Error: Failed to create/access '%s'\n", (Global::pathToEXE + "res/SaveData/SaveData.sav").c_str());
        file.close();
    }
    else
    {
        Global::gameSaveData["PLAYTIME"] = std::to_string(Global::gameTotalPlaytime);

        Global::gameSaveData["RINGS"] = std::to_string(Global::gameAccumulatedRings);

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

void Global::saveConfigData()
{
    std::ofstream file;
    file.open((Global::pathToEXE + "Settings/GraphicsSettings.ini").c_str(), std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        printf("Error: Failed to create/access '%s'\n", (Global::pathToEXE + "Settings/GraphicsSettings.ini").c_str());
        file.close();
    }
    else
    {
        file << "#Reflective water.\n";
        file << "#Should be 'on' or 'off'.\n";
        if (Global::useHighQualityWater) { file << "Reflective_Water on\n\n"; }
        else                             { file << "Reflective_Water off\n\n"; }

        //file << "#Resolution of the high quality water\n";
        //file << "HQ_Water_Reflection_Width "  << Global::HQWaterReflectionWidth  << "\n";
        //file << "HQ_Water_Reflection_Height " << Global::HQWaterReflectionHeight << "\n\n";
        //file << "HQ_Water_Refraction_Width "  << Global::HQWaterRefractionWidth  << "\n";
        //file << "HQ_Water_Refraction_Height " << Global::HQWaterRefractionHeight << "\n\n";

        file << "#Bloom post processing.\n";
        if (Global::renderBloom) { file << "Bloom on\n\n"; }
        else                     { file << "Bloom off\n\n"; }

        file << "#Show particles.\n";
        if (Global::renderParticles) { file << "Show_Particles on\n\n"; }
        else                         { file << "Show_Particles off\n\n"; }

        file << "#Shows FPS in the corner of the screen.\n";
        if (Global::displayFPS)  { file << "Show_FPS on\n\n"; }
        else                     { file << "Show_FPS off\n\n"; }

        file << "#Shows speedometer in the corner of the screen.\n";
        if (Global::displaySpeedometer)  { file << "Show_Speedometer on\n\n"; }
        else                             { file << "Show_Speedometer off\n\n"; }

        file << "#Number of multisamples to use for anti-aliasing.\n";
        file << "Anti-Aliasing_Samples " << Display::AA_SAMPLES << "\n\n";

        file << "#Vertical Field of View.\n";
        file << "FOV " << MasterRenderer::VFOV_BASE << "\n\n";

        file << "#V-Sync.\n";
        if (!Global::framerateUnlock) { file << "V-Sync on\n\n"; }
        else                          { file << "V-Sync off\n\n"; }

        file << "#If v-sync is off, fps will not exceed this limit. -1 for no limit.\n";
        file << "FPS_Limit " << (int)Global::fpsLimit << "\n";

        file.close();
    }

    file.open((Global::pathToEXE + "Settings/AudioSettings.ini").c_str(), std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        printf("Error: Failed to create/access '%s'\n", (Global::pathToEXE + "Settings/AudioSettings.ini").c_str());
        file.close();
    }
    else
    {
        file << "#Volume of sound effects.\n";
        file << "#Should be 0 for no sfx, 100 for full volume sfx.\n";
        file << "SFX_Volume " << (int)std::round(AudioPlayer::soundLevelSFX*100) << "\n\n";

        file << "#Volume of music.\n";
        file << "#Should be 0 for no music, 100 for full volume music.\n";
        file << "Music_Volume " << (int)std::round(AudioPlayer::soundLevelBGM*100) << "\n";

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

    Level* currentLevel = &Global::gameLevelData[Global::levelId];

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

        if (Global::gameSaveData.find(currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionScoreString) != Global::gameSaveData.end())
        {
            std::string savedScoreString = Global::gameSaveData[currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionScoreString];
            savedScore = std::stoi(savedScoreString);
        }

        if (newScore > savedScore)
        {
            std::string newScoreString = std::to_string(newScore);
            Global::gameSaveData[currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionScoreString] = newScoreString;
        }

        float newTime = 60000.0f;
        if (Global::mainHudTimer != nullptr)
        {
            newTime = Global::mainHudTimer->totalTime;
        }
        float savedTime = 600000.0f;

        if (Global::gameSaveData.find(currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionTimeString) != Global::gameSaveData.end())
        {
            std::string savedTimeString = Global::gameSaveData[currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionTimeString];
            savedTime = std::stof(savedTimeString);
        }

        if (newTime < savedTime)
        {
            std::string newTimeString = std::to_string(newTime);
            Global::gameSaveData[currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionTimeString] = newTimeString;
        }

        int savedRings = 0;
        if (Global::gameSaveData.find(currentLevel->displayName+"_RINGS") != Global::gameSaveData.end())
        {
            std::string savedRingsString = Global::gameSaveData[currentLevel->displayName+"_RINGS"];
            savedRings = std::stoi(savedRingsString);
        }

        if (Global::gameRingCount >= savedRings)
        {
            Global::gameSaveData[currentLevel->displayName+"_RINGS"] = std::to_string(Global::gameRingCount);
        }

        Global::gameAccumulatedRings += Global::gameRingCount;

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

                Global::gameSaveData[currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionRankString]  = newRankString;
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

                Global::gameSaveData[currentLevel->displayName+"_"+Global::characterNames[Global::currentCharacterType]+missionRankString] = newRankString;
            }
        }

        Global::saveSaveData();
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
            printf("input = '%s'\n", input.c_str());
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

    //const float DIVIdE_OFFSET = 1.0f;

    //ensure the coords arent out of bounds
    //relativeX = std::fmaxf(0.0f, relativeX);
    //relativeX = std::fminf(relativeX, chunkedEntitiesWidth*chunkedEntitiesChunkSize - DIVIdE_OFFSET);

    //relativeZ = std::fmaxf(0.0f, relativeZ);
    //relativeZ = std::fminf(relativeZ, chunkedEntitiesHeight*chunkedEntitiesChunkSize - DIVIdE_OFFSET);

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
        printf("Error: Index out of bounds on gameNearbyEntities. THIS IS VERY BAD.\n");
        printf("    x = %f       z = %f\n", x, z);
        printf("    relativeX = %f      relativeZ = %f\n", relativeX, relativeZ);
        printf("    iX = %d        iZ = %d\n", iX, iZ);
        printf("    chunkedEntitiesWidth = %d             chunkedEntitiesHeight = %d\n", chunkedEntitiesWidth, chunkedEntitiesHeight);
        printf("    chunkedEntitiesChunkSize = %f\n", chunkedEntitiesChunkSize);
        printf("    realIndex = %d          gameChunkedEntities.size() = %d\n", realIndex, (int)gameChunkedEntities.size());
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
            printf("Error: Render distance not out of range.\n");
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
        printf("Error: Trying to recalculate entity chunks when gameChunkedEntities is not 0.\n");
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

void Global::addChunkedEntity(Entity* entityToAdd)
{
    gameChunkedEntitiesToAdd.push_back(entityToAdd);
}

void Global::deleteChunkedEntity(Entity* entityToAdd)
{
    gameChunkedEntitiesToDelete.push_back(entityToAdd);
}

void Global::deleteAllChunkedEntities()
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
    ParticleMaster::createParticle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &vel, 0, 1.0f, 50.0f, 0, true, false, false);
    
    switch (Global::currentCharacterType)
    {
        case Global::PlayableCharacter::Sonic:
            GuiManager::addImageToRender(GuiImageResources::textureLineBlue);
            break;

        case Global::PlayableCharacter::Tails:
            GuiManager::addImageToRender(GuiImageResources::textureLineYellow);
            break;

        case Global::PlayableCharacter::Knuckles:
            GuiManager::addImageToRender(GuiImageResources::textureLineRed);
            break;

        default: break;
    }

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

#ifdef DEV_MODE

std::unordered_map<std::string, int> heapObjects;

void Global::debugNew(const char* name)
{
    Global::countNew++;

    if (heapObjects.find(name) == heapObjects.end())
    {
        heapObjects[name] = 1;
    }
    else
    {
        int num = heapObjects[name];
        heapObjects[name] = num+1;
    }
}

void Global::debugDel(const char* name)
{
    Global::countDelete++;

    if (heapObjects.find(name) == heapObjects.end())
    {
        printf("Warning: trying to delete '%s' when there are none.\n", name);
        heapObjects[name] = 0;
    }
    else
    {
        int num = heapObjects[name];
        heapObjects[name] = num-1;
    }
}
#endif

#ifdef PERFORMANCE_ANALYSIS

std::unordered_map<std::string, float> operationTotalTimes;
std::unordered_map<std::string, float> operationStartTimes;

void Global::performanceAnalysisStart(const char* name)
{
    auto startTime = operationStartTimes.find(name);
    if (startTime == operationStartTimes.end())
    {
        operationStartTimes[name] = (float)glfwGetTime();
    }
    else
    {
        printf("Starting operation %s but that operation is already happening...\n", name);
    }
}

void Global::performanceAnalysisDone(const char* name)
{
    auto startTime = operationStartTimes.find(name);
    if (startTime != operationStartTimes.end())
    {
        float timeTaken = ((float)glfwGetTime()) - startTime->second;
        operationStartTimes.erase(startTime);

        auto totalTime = operationTotalTimes.find(name);
        if (totalTime == operationTotalTimes.end())
        {
            operationTotalTimes[name] = timeTaken;
        }
        else
        {
            operationTotalTimes[name] = totalTime->second + timeTaken;
        }
    }
    else
    {
        printf("Operation %s hasn't been started yet. Or a recursive operation, which is a no no.\n", name);
    }
}

float clockLast = 0.0f;

void Global::performanceAnalysisReport()
{
    //float frameTime = operationTotalTimes["Frame Time"];
    //frameTime = 0.0f;
    if (Global::gameClock - clockLast > 3.0f) //only show report every 3 seconds since showing it causes a lag spike
    {
        clockLast = Global::gameClock;
        printf("Performance Report\n");

        auto it = operationTotalTimes.begin();
        while (it != operationTotalTimes.end())
        {
            printf("%s    %f\n", it->first.c_str(), 1000*it->second);
            it++;
        }

        printf("\n");

        if (!operationStartTimes.empty())
        {
            printf("operationTotalTimes was not empty. Some operations were not finished when doing the report.\n");
        }
    }

    operationStartTimes.clear();
    operationTotalTimes.clear();
}

#endif
