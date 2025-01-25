#ifndef MAIN_H
#define MAIN_H

class Entity;
class Camera;
class Stage;
class SkySphere;
class Light;
class Fbo;
class Checkpoint;
class WaterRenderer;
class WaterFrameBuffers;
class WaterTile;
class FontType;
class GUIText;
class Timer;
class ControllablePlayer;
class TC_Kart;
class MainMenu;
class CharacterSelect;
class MissionMenu;
class ConfigMenu;
class Extras;
class ResultsScreen;
class PlayerModel;

#include <string>
#include <random>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../toolbox/level.hpp"
#include "../menu/menumanager.hpp"
#include "../toolbox/vector.hpp"
#include "../entities/raceghost.hpp"

#define STATE_PAUSED 0
#define STATE_RUNNING 1
#define STATE_EXITING 2
#define STATE_CUTSCENE 3
#define STATE_TITLE 4
#define STATE_DEBUG 5

#define LVL_TUTORIAL         0
#define LVL_GREEN_FOREST     1
#define LVL_METAL_HARBOR     2
#define LVL_PYRAMID_CAVE     3
#define LVL_SKY_RAIL         4
#define LVL_RADICAL_HIGHWAY  5
#define LVL_GREEN_HILL_ZONE  6
#define LVL_CITY_ESCAPE      7
#define LVL_WINDY_VALLEY     8
#define LVL_SEASIDE_HILL     9
#define LVL_FROG_FOREST     10
#define LVL_TEST            11
#define LVL_SPEED_HIGHWAY   12
#define LVL_NOKI_BAY        13
#define LVL_DELFINO_PLAZA   14
#define LVL_SACRED_SKY      15
#define LVL_DRY_LAGOON      16
#define LVL_TWINKLE_CIRCUIT 17
#define LVL_CLOUD_TEMPLE    18
#define LVL_DRAGON_ROAD     19
#define LVL_EMERALD_COAST   20
#define LVL_FREEZEEZY_PEAK  21
#define LVL_SWEET_MOUNTAIN  22
#define LVL_ICE_CAP         23
#define LVL_CASTLE_TOWN     24
#define LVL_BOSS            25

//#define DEV_MODE
//#define PERFORMANCE_ANALYSIS

#ifdef DEV_MODE
    #include <thread>
    #define INCR_NEW(NAME) Global::debugNew(NAME);
    #define INCR_DEL(NAME) Global::debugDel(NAME);
#else
    #define INCR_NEW(NAME) ;
    #define INCR_DEL(NAME) ;
#endif

#ifdef PERFORMANCE_ANALYSIS
    #define ANALYSIS_START(NAME) Global::performanceAnalysisStart(NAME);
    #define ANALYSIS_DONE(NAME)  Global::performanceAnalysisDone(NAME);
    #define ANALYSIS_REPORT()    Global::performanceAnalysisReport();
#else
    #define ANALYSIS_START(NAME) ;
    #define ANALYSIS_DONE(NAME)  ;
    #define ANALYSIS_REPORT()    ;
#endif


class Global
{
public:
    static std::string pathToEXE;
    static MenuManager menuManager;
    static MainMenu* menuMain;
    static CharacterSelect* menuCharacterSelect;
    static MissionMenu* menuMission;
    static ConfigMenu* menuConfig;
    static Extras* menuExtras;
    static ResultsScreen* menuResults;
    static Timer* mainHudTimer;
    static Camera* gameCamera;
    static ControllablePlayer* gameMainPlayer;
    static TC_Kart* gameKart;
    static Entity* gameStageManager;
    static Stage* gameStage;
    static SkySphere* gameSkySphere;
    static Light* gameLightSun;
    static Light* gameLightMoon;
    static int countNew;
    static int countDelete;
    static int gameState;
    static int levelId;
    static std::string levelName;
    static std::string levelNameDisplay;
    static bool shouldLoadLevel;
    static bool isNewLevel;
    static bool isAutoCam;
    static int gameRingCount;
    static int gameDebugRingCount;
    static int gameScore;
    static int gameLives;
    static float gameClock;
    static WaterRenderer* gameWaterRenderer;
    static WaterFrameBuffers* gameWaterFBOs;
    static std::vector<WaterTile*> gameWaterTiles;
    static float gameMainVehicleSpeed;
    static float startStageTimer;
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
    static bool gameIsRaceMode;
    static float gameRaceTimeLimit;
    static std::string gameMissionDescription;
    static int gameRingTarget; //How many rings you have to collect to finish the stage in Ring mode
    static bool gameIsArcadeMode;
    static int gameTotalPlaytime; //in seconds
    static int gameAccumulatedRings;
    static float gameArcadePlaytime;
    static bool stageUsesWater;
    static Vector3f stageWaterColor;
    static float stageWaterBlendAmount;
    static float stageWaterMurkyAmount;
    static FontType* fontVipnagorgialla;
    static bool renderWithCulling; //if the stage uses backface culling or not
    static bool displayFPS;
    static bool displaySpeedometer;
    static int currentCalculatedFPS;
    static int renderCount;
    //static const int syncToDisplayEveryXFrames = 1; //if for some reason you want to render at double, triple, etc. the fps of the monitor
    //static float fpsTarget; //experiment to try to sleep to maintain a target fps
    static float fpsLimit; //experiment to try to busy wait to maintain a target fps
    static bool useFullscreen;
    static int displaySizeChanged; //This will be 1 for a single frame after the size of the window changes (set in callback)
    static PlayerModel* mainMenuTails;
    //static PlayerModel* mainMenuKnuckles
    static bool restartAudioOnRestart;

    //the different types of character movements
    enum PlayableCharacter
    {
        Sonic    = 0,
        Tails    = 1,
        Knuckles = 2
    };
    static PlayableCharacter currentCharacterType;
    static std::unordered_map<PlayableCharacter, std::string> characterNames;

    //static std::list<std::string> raceLog;
    static bool shouldLogRace;

    static int raceLogSize;
    static GhostFrame raceLog[];
    static const int raceLogSizeMax = 432000; //enough for 10 minutes at 720fps

    static bool spawnAtCheckpoint;
    static Vector3f checkpointPlayerPos;
    static Vector3f checkpointPlayerDir;
    static Vector3f checkpointCamDir;
    static float checkpointTime;

    //texts for the title card
    static GUIText* titleCardLevelName;
    static GUIText* titleCardMission;
    static GUIText* titleCardMissionDescription;

    static std::vector<Level> gameLevelData;
    static std::unordered_map<std::string, std::string> gameSaveData;

    static int gameArcadeIndex;
    static std::vector<std::pair<int, Global::PlayableCharacter>> gameArcadeLevelIds;

    static std::vector<int> gameLevelIdsSonic;    //levels for sonic
    static std::vector<int> gameLevelIdsTails;    //levels for tails
    static std::vector<int> gameLevelIdsKnuckles; //levels for knuckles

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

    //how many npcs are in each level
    static std::unordered_map<int, int> stageNpcCounts;

    // Id for chaos emerald per level
    static std::unordered_map<int, int> stageChaosEmeraldId;

    static std::vector<bool> arcadeModeEmeraldFound;

    static void checkErrorAL(const char* description);

    static void loadSaveData();

    static void saveSaveData();

    static void saveGhostData();

    static void saveConfigData();

    static int calculateRankAndUpdate();

    static void increaseRingCount(int rings);

    //Entities passed here should be made on the heap, as they will be explicitly deleted later
    static void addEntity(Entity* entityToAdd);
    static void deleteEntity(Entity* entityToDelete);
    static void deleteAllEntites();

    static void addChunkedEntity(Entity* entityToAdd);
    static void deleteChunkedEntity(Entity* entityToAdd);
    static void deleteAllChunkedEntities();

    //Return a list of nearby entity sets. renderDistance is number of layers to go outwards.
    //0 = only the exact chunk.
    //1 = 4 chunks
    //2 = 9 chunks
    static void getNearbyEntities(float x, float z, int renderDistance, std::vector<std::unordered_set<Entity*>*>* list);

    //Return a list of nearby entity sets. Returns either 1 chunk, 2 chunks, or 4 chunks,
    // depending on the minDistance value.
    static void getNearbyEntities(float x, float z, std::vector<std::unordered_set<Entity*>*>* list, float minDistance);

    //Returns the index of 'gameChunkedEntities' for the (x, z) location
    static int getChunkIndex(float x, float z);

    static void recalculateEntityChunks(float minX, float maxX, float minZ, float maxZ, float chunkSize);

    static void createTitleCard();

    static void clearTitleCard();

    #ifdef DEV_MODE
    static void debugNew(const char* name);

    static void debugDel(const char* name);
    #endif

    #ifdef PERFORMANCE_ANALYSIS
    static void performanceAnalysisStart(const char* name);

    static void performanceAnalysisDone(const char* name);

    static void performanceAnalysisReport();
    #endif
};
#endif
