#ifndef MAIN_H
#define MAIN_H

class Entity;
class Camera;
class Stage;
class SkySphere;
class Light;
class ShadowMapMasterRenderer;
class Fbo;
class Checkpoint;
class WaterRenderer;
class WaterFrameBuffers;
class WaterTile;
class FontType;
class GUIText;
class Timer;
class ControllablePlayer;

#include <string>
#include <random>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../toolbox/level.h"
#include "../menu/menumanager.h"

void Main_addEntity(Entity* entityToAdd);
void Main_deleteEntity(Entity* entityToDelete);
void Main_deleteAllEntites();

void Main_addEntityPass2(Entity* entityToAdd);
void Main_deleteEntityPass2(Entity* entityToDelete);
void Main_deleteAllEntitesPass2();

void Main_addEntityPass3(Entity* entityToAdd);
void Main_deleteEntityPass3(Entity* entityToDelete);
void Main_deleteAllEntitesPass3();

void Main_addTransparentEntity(Entity* entityToAdd);
void Main_deleteTransparentEntity(Entity* entityToDelete);
void Main_deleteAllTransparentEntites();

void Main_addChunkedEntity(Entity* entityToAdd);
void Main_deleteChunkedEntity(Entity* entityToAdd);
void Main_deleteAllChunkedEntities();

#define STATE_PAUSED 0
#define STATE_RUNNING 1
#define STATE_EXITING 2
#define STATE_CUTSCENE 3
#define STATE_TITLE 4
#define STATE_DEBUG 5

#define LVL_WINDY_VALLEY     0
#define LVL_GREEN_FOREST     1
#define LVL_METAL_HARBOR     2
#define LVL_PYRAMID_CAVE     3
#define LVL_SPEED_HIGHWAY    4
#define LVL_RADICAL_HIGHWAY  5
#define LVL_GREEN_HILL_ZONE  6

//#define ENTITY_RENDER_DIST 2000.0f
//#define ENTITY_RENDER_DIST_HIGH 5000.0f

#define DEV_MODE

#ifdef DEV_MODE
	#include <thread>
	#define INCR_NEW(NAME) Global::debugNew(NAME);
	#define INCR_DEL(NAME) Global::debugDel(NAME);
#else
	#define INCR_NEW(NAME) ;
	#define INCR_DEL(NAME) ;
#endif

class Global
{
public:
    static std::string pathToEXE;
	static MenuManager menuManager;
    static Timer* mainHudTimer;
	static Camera* gameCamera;
	static ControllablePlayer* gameMainPlayer;
	static Stage* gameStage;
	static SkySphere* gameSkySphere;
	static Light* gameLightSun;
	static Light* gameLightMoon;
	static int countNew;
	static int countDelete;
	static int gameState;
	static int levelID;
	static std::string levelName;
	static std::string levelNameDisplay;
	static float raceStartTimer;
	static bool shouldLoadLevel;
	static bool isNewLevel;
	static bool isAutoCam;
	static int gameRingCount;
	static int gameScore;
	static int gameLives;
	static float gameClock;
	static WaterRenderer* gameWaterRenderer;
	static WaterFrameBuffers* gameWaterFBOs;
	static std::vector<WaterTile*> gameWaterTiles;
	static int gameMainVehicleSpeed;
	static float finishStageTimer;
	static Fbo* gameMultisampleFbo;
	static Fbo* gameOutputFbo;
	static Fbo* gameOutputFbo2;
	static float deathHeight;
	static float waterHeight;
	static int gameMissionNumber;
	static bool gameIsNormalMode;
	static bool gameIsHardMode;
	static bool gameIsChaoMode;
	static bool gameIsRingMode;
	static std::string gameMissionDescription;
	static int gameRingTarget; //How many rings you have to collect to finish the stage in Ring mode
	static bool gameIsArcadeMode;
	static float gameTotalPlaytime;
	static float gameArcadePlaytime;
	static bool stageUsesWater;
	static FontType* fontVipnagorgialla;
    static bool renderWithCulling; //if the stage uses backface culling or not
    static bool displayFPS;
    static int currentCalculatedFPS;
    static int renderCount;
    //static const int syncToDisplayEveryXFrames = 1; //if for some reason you want to render at double, triple, etc. the fps of the monitor

    static std::list<std::string> raceLog;
    static bool shouldLogRace;

	static bool  spawnAtCheckpoint;
	static float checkpointX;
	static float checkpointY;
	static float checkpointZ;
	static float checkpointRotY;
	static float checkpointCamYaw;
	static float checkpointCamPitch;
	static int   checkpointTimeCen;
	static int   checkpointTimeSec;
	static int   checkpointTimeMin;

	//texts for the title card
	static GUIText* titleCardLevelName;
	static GUIText* titleCardMission;
	static GUIText* titleCardMissionDescription;

	static std::list<Checkpoint*> gameCheckpointList;
	static int gameCheckpointLast;

	static std::vector<Level> gameLevelData;
	static std::unordered_map<std::string, std::string> gameSaveData;

	//Debug display
	static bool debugDisplay;
	static bool frozen;
	static bool step;

	//Graphics settings
	static bool useHighQualityWater;
	static unsigned HQWaterReflectionWidth;
	static unsigned HQWaterReflectionHeight;
	static unsigned HQWaterRefractionWidth;
	static unsigned HQWaterRefractionHeight;

	static bool renderParticles;

	static bool renderBloom;

    static bool framerateUnlock;

	static bool renderShadowsFar;
	static bool renderShadowsClose;
	static int shadowsFarQuality;

	static bool unlockedSonicDoll;
	static bool unlockedMechaSonic;
	static bool unlockedDage4;
	static bool unlockedManiaSonic;
	static bool unlockedAmy;

	static std::vector<std::string> npcList;

	static void checkErrorAL(const char* description);

	static void loadSaveData();

	static void saveSaveData();

	static int calculateRankAndUpdate();

	static void increaseRingCount(int rings);

	//Return a list of nearby entity sets. renderDistance is number of layers to go outwards.
	//0 = only the exact chunk.
	//1 = 4 chunks
	//2 = 9 chunks
	static void getNearbyEntities(float x, float z, int renderDistance, std::list<std::unordered_set<Entity*>*>* list);

	//Returns the index of 'gameChunkedEntities' for the (x, z) location
	static int getChunkIndex(float x, float z);

	static void recalculateEntityChunks(float minX, float maxX, float minZ, float maxZ, float chunkSize);

	static void createTitleCard();

	static void clearTitleCard();

    static void debugNew(const char* name);

    static void debugDel(const char* name);
};
#endif
