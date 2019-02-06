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
#include <list>

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
#include "../toolbox/pausescreen.h"
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
#include "../toolbox/mainmenu.h"
#include "../toolbox/level.h"
#include "../guis/guitexture.h"
#include "../particles/particle.h"
#include "../entities/skysphere.h"
#include "../fontMeshCreator/guinumber.h"
#include "../entities/car.h"
#include "../entities/checkpoint.h"
#include "../water/waterframebuffers.h"
#include "../water/watershader.h"
#include "../water/waterrenderer.h"
#include "../water/watertile.h"
#include "../toolbox/getline.h"

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif


std::unordered_map<Entity*, Entity*> gameEntities;
std::list<Entity*> gameEntitiesToAdd;
std::list<Entity*> gameEntitiesToDelete;

std::unordered_map<Entity*, Entity*> gameEntitiesPass2;
std::list<Entity*> gameEntitiesPass2ToAdd;
std::list<Entity*> gameEntitiesPass2ToDelete;

std::unordered_map<Entity*, Entity*> gameEntitiesPass3;
std::list<Entity*> gameEntitiesPass3ToAdd;
std::list<Entity*> gameEntitiesPass3ToDelete;

std::unordered_map<Entity*, Entity*> gameTransparentEntities;
std::list<Entity*> gameTransparentEntitiesToAdd;
std::list<Entity*> gameTransparentEntitiesToDelete;

float Global::waterHeight = 0.0f;
WaterRenderer* Global::gameWaterRenderer = nullptr;
WaterFrameBuffers* Global::gameWaterFBOs = nullptr;
std::list<WaterTile*>* Global::gameWaterTiles = nullptr;

float dt = 0;
double timeOld = 0;
double timeNew = 0;
Camera*    Global::gameCamera      = nullptr;
Car*       Global::gameMainVehicle = nullptr;
Stage*     Global::gameStage       = nullptr;
SkySphere* Global::gameSkySphere   = nullptr;
Light*     Global::gameLightSun    = nullptr;
Light*     Global::gameLightMoon   = nullptr;

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
float Global::raceStartTimer = -1;
bool Global::shouldLoadLevel = false;
bool Global::isNewLevel = false;
bool Global::isAutoCam = true;
std::string Global::levelName = "";
std::string Global::levelNameDisplay = "";
int Global::gameRingCount = 0;
int Global::gameScore = 0;
int Global::gameLives = 4;
int Global::gameClock = 0;
float Global::gameTotalPlaytime = 0;
float Global::gameArcadePlaytime = 0;
float Global::deathHeight = -100.0f;
int Global::gameMainVehicleSpeed = 0;

int Global::gameMissionNumber = 0;
std::string Global::gameMissionDescription = "";
bool Global::gameIsNormalMode = false;
bool Global::gameIsHardMode = false;
bool Global::gameIsChaoMode = false;
bool Global::gameIsRingMode = false;
int  Global::gameRingTarget = 100;
bool Global::gameIsArcadeMode = false;
std::vector<Level> Global::gameLevelData;
std::unordered_map<std::string, std::string> Global::gameSaveData;
bool Global::stageUsesWater = true;

std::list<Checkpoint*> Global::gameCheckpointList;
int Global::gameCheckpointLast;

bool   Global::spawnAtCheckpoint  = false;
float  Global::checkpointX        = 0;
float  Global::checkpointY        = 0;
float  Global::checkpointZ        = 0;
float  Global::checkpointRotY     = 0;
float  Global::checkpointCamYaw   = 0;
float  Global::checkpointCamPitch = 0;
int    Global::checkpointTimeCen  = 0;
int    Global::checkpointTimeSec  = 0;
int    Global::checkpointTimeMin  = 0;

bool Global::unlockedSonicDoll = true;
bool Global::unlockedMechaSonic = true;
bool Global::unlockedDage4 = true;
bool Global::unlockedManiaSonic = true;
bool Global::unlockedAmy = true;

std::vector<std::string> Global::npcList;

void increaseProcessPriority();

void doListenThread();

void listen();

int main()
{
	#ifdef DEV_MODE
	std::thread listenThread(doListenThread);
	#endif

	increaseProcessPriority();

	Global::countNew = 0;
	Global::countDelete = 0;

	srand(0);

	createDisplay();

	Global::loadSaveData();

	#if !defined(DEV_MODE) && defined(_WIN32)
	FreeConsole();
	#endif

	Input::init();

	//This camera is never deleted.
	Camera cam;
	Global::gameCamera = &cam;

	Master_init();

	LevelLoader::loadLevelData();

	PauseScreen::init();

	AudioMaster::init();

	TextMaster::init();

	GuiManager::init();

	MainMenu::init();


	if (Global::renderParticles)
	{
		ParticleResources::loadParticles();
	}

	GuiTextureResources::loadGuiTextures();

	CollisionChecker::initChecker();
	//AnimationResources::createAnimations();

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
		Global::gameWaterFBOs     = new WaterFrameBuffers; INCR_NEW
		WaterShader* waterShader  = new WaterShader; INCR_NEW
		Global::gameWaterRenderer = new WaterRenderer(waterShader, Master_getProjectionMatrix(), Global::gameWaterFBOs, Master_getShadowRenderer()); INCR_NEW
		Global::gameWaterTiles    = new std::list<WaterTile*>; INCR_NEW
		for (int r = -6; r < 6; r++) //-9 , 9
		{
			for (int c = -8; c < 8; c++) //-12  12
			{
				Global::gameWaterTiles->push_back(new WaterTile(r*WaterTile::TILE_SIZE*2, c*WaterTile::TILE_SIZE*2)); INCR_NEW
			}
		}
	}

	if (Global::renderBloom)
	{
		Global::gameMultisampleFbo = new Fbo(SCR_WIDTH, SCR_HEIGHT); INCR_NEW
		Global::gameOutputFbo      = new Fbo(SCR_WIDTH, SCR_HEIGHT, Fbo::DEPTH_TEXTURE); INCR_NEW
		Global::gameOutputFbo2     = new Fbo(SCR_WIDTH, SCR_HEIGHT, Fbo::DEPTH_TEXTURE); INCR_NEW
		PostProcessing::init();
	}

	ParticleMaster::init(Master_getProjectionMatrix());


	glfwSetTime(0);

	int frameCount = 0;
	double previousTime = 0;

	//PauseScreen::pause();
	Global::gameState = STATE_TITLE;

	while (Global::gameState != STATE_EXITING && displayWantsToClose() == 0)
	{
		frameCount++;
		timeNew = glfwGetTime();
		dt = (float)(timeNew - timeOld);
		dt = std::fminf(dt, 0.04f); //Anything lower than 25fps will slow the gameplay down

		timeOld = timeNew;

		Global::gameTotalPlaytime+=dt;

		if (Global::gameIsArcadeMode)
		{
			Global::gameArcadePlaytime+=dt;
		}

		Input::pollInputs();

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
			gameEntities.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
		}
		gameEntitiesToAdd.clear();

		for (auto entityToDelete : gameEntitiesToDelete)
		{
			gameEntities.erase(entityToDelete);
			delete entityToDelete; INCR_DEL
		}
		gameEntitiesToDelete.clear();


		//entities pass2 managment
		for (auto entityToAdd : gameEntitiesPass2ToAdd)
		{
			gameEntitiesPass2.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
		}
		gameEntitiesPass2ToAdd.clear();

		for (auto entityToDelete : gameEntitiesPass2ToDelete)
		{
			gameEntitiesPass2.erase(entityToDelete);
			delete entityToDelete; INCR_DEL
		}
		gameEntitiesPass2ToDelete.clear();


		//entities pass3 managment
		for (auto entityToAdd : gameEntitiesPass3ToAdd)
		{
			gameEntitiesPass3.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
		}
		gameEntitiesPass3ToAdd.clear();

		for (auto entityToDelete : gameEntitiesPass3ToDelete)
		{
			gameEntitiesPass3.erase(entityToDelete);
			delete entityToDelete; INCR_DEL
		}
		gameEntitiesPass3ToDelete.clear();


		//transnaprent entities managment
		for (auto entityToAdd : gameTransparentEntitiesToAdd)
		{
			gameTransparentEntities.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
		}
		gameTransparentEntitiesToAdd.clear();

		for (auto entityToDelete : gameTransparentEntitiesToDelete)
		{
			gameTransparentEntities.erase(entityToDelete);
			delete entityToDelete; INCR_DEL
		}
		gameTransparentEntitiesToDelete.clear();


		MainMenu::step();
		PauseScreen::step();

		switch (Global::gameState)
		{
			case STATE_RUNNING:
			{
				//game logic
				GuiManager::increaseTimer(dt);

				if (Global::raceStartTimer >= 0)
				{
					Global::raceStartTimer -= dt;
					if (Global::raceStartTimer < 0)
					{
						//if (bgmHasLoop != 0)
						{
							//By default, first 2 buffers are the intro and loop, respectively
							AudioPlayer::playBGMWithIntro(0, 1);
						}
						//else
						{
							//AudioPlayer::playBGM(0);
						}
						GuiManager::startTimer();
						//Global::gameMainVehicle->setCanMoveTimer(0);
						//Global::gameMainVehicle->setPosition(22.3715019f, 0.01f, 20.5539f);
						//Global::gameMainVehicle->setVelocity(0, 0, -0.001f);
					}
					else
					{
						//Global::gameMainVehicle->setPosition(22.3715019f, 0.01f, 20.5539f);
						//Global::gameMainVehicle->setVelocity(0, 0, -0.001f);
						//Global::gameMainVehicle->setCanMoveTimer(1.0f);
					}
				}

				for (auto e : gameEntities)
				{
					e.first->step();
				}
				for (auto e : gameEntitiesPass2)
				{
					e.first->step();
				}
				for (auto e : gameEntitiesPass3)
				{
					e.first->step();
				}
				for (auto e : gameTransparentEntities)
				{
					e.first->step();
				}
				skySphere.step();
				Global::gameCamera->refresh();
				if (Global::renderParticles)
				{
					ParticleMaster::update(Global::gameCamera);
				}
				Global::gameClock++;

				if (Global::debugDisplay && Global::frozen)
				{
					Global::gameState = STATE_DEBUG;
				}

				if (Global::gameIsRingMode)
				{
					if (Global::gameRingCount >= Global::gameRingTarget && Global::finishStageTimer < -0.5f)
					{
						Global::finishStageTimer = 0;
						GuiManager::stopTimer();
					}
				}
				break;
			}

			case STATE_PAUSED:
			{
				break;
			}

			case STATE_CUTSCENE:
			{
				Global::gameCamera->refresh();
				break;
			}

			case STATE_TITLE:
			{
				Global::gameCamera->refresh();
				if (Global::renderParticles)
				{
					ParticleMaster::update(Global::gameCamera);
				}
				break;
			}

			case STATE_DEBUG:
			{
				if (Global::gameMainVehicle != nullptr)
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

		Stage::updateVisibleChunks();
		SkyManager::calculateValues();

		//prepare entities to render
		for (auto e : gameEntities)
		{
			Master_processEntity(e.first);
		}
		for (auto e : gameEntitiesPass2)
		{
			Master_processEntityPass2(e.first);
		}
		for (auto e : gameEntitiesPass3)
		{
			Master_processEntityPass3(e.first);
		}
		for (auto e : gameTransparentEntities)
		{
			Master_processTransparentEntity(e.first);
		}
		for (Checkpoint* check : Global::gameCheckpointList)
		{
			Master_processEntity(check);
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
			Global::gameWaterRenderer->render(Global::gameWaterTiles, &cam, &lightSun);
		}

		if (Global::renderParticles)
		{
			ParticleMaster::renderParticles(&cam, SkyManager::getOverallBrightness(), 0);
		}

		if (Global::renderBloom)
		{
			Global::gameMultisampleFbo->unbindFrameBuffer();
			Global::gameMultisampleFbo->resolveToFbo(GL_COLOR_ATTACHMENT0, Global::gameOutputFbo);
			Global::gameMultisampleFbo->resolveToFbo(GL_COLOR_ATTACHMENT1, Global::gameOutputFbo2);
			PostProcessing::doPostProcessing(Global::gameOutputFbo->getColourTexture(), Global::gameOutputFbo2->getColourTexture());
		}

		Master_clearEntities();
		Master_clearEntitiesPass2();
		Master_clearEntitiesPass3();
		Master_clearTransparentEntities();

		GuiManager::refresh();
		TextMaster::render();

		updateDisplay();

		AudioPlayer::refreshBGM();

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
				new Particle(ParticleResources::textureWhiteFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 0, 900, 0, true, false);
			}
			else if (finishTimerBefore < 1.0f && Global::finishStageTimer >= 1.0f)
			{
				AudioPlayer::stopBGM();
				//AudioPlayer::play(24, Global::gamePlayer->getPosition());
			}
			else if (finishTimerBefore < 8.166f && Global::finishStageTimer >= 8.166f)
			{
				Vector3f partVel(0, 0, 0);
				new Particle(ParticleResources::textureBlackFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 0, 900, 0, true, false);

				//AudioPlayer::play(25, Global::gamePlayer->getPosition());
			}

			if (Global::finishStageTimer >= 0 &&
				Global::finishStageTimer < 1)
			{
				AudioPlayer::setBGMVolume(1-Global::finishStageTimer);
			}

			if (finishTimerBefore < 9.166f && Global::finishStageTimer >= 9.166f)
			{
				if (Global::gameIsArcadeMode)
				{
					Global::levelID+=1;

					if (Global::levelID <= LVL_GREEN_HILL_ZONE)
					{
						Level* nextLevel = &Global::gameLevelData[Global::levelID];
						Global::shouldLoadLevel = true;
						Global::isNewLevel = true;
						Global::levelName = nextLevel->fileName;
						Global::levelNameDisplay = nextLevel->displayName;
						Global::gameMissionDescription = (nextLevel->missionData[Global::gameMissionNumber])[(nextLevel->missionData[Global::gameMissionNumber]).size()-1];

						MainMenu::createTitleCard();
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
						MainMenu::selectMenuArcadeClear();
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
				GuiManager::setTimer(0);
			}

			if (finishTimerBefore < 6.166f && Global::finishStageTimer >= 6.166f)
			{
				int rank = Global::calculateRankAndUpdate();
				switch (rank)
				{
					case 0: GuiTextureResources::textureRankDisplay->setTexture(MainMenu::textureRankE); break;
					case 1: GuiTextureResources::textureRankDisplay->setTexture(MainMenu::textureRankD); break;
					case 2: GuiTextureResources::textureRankDisplay->setTexture(MainMenu::textureRankC); break;
					case 3: GuiTextureResources::textureRankDisplay->setTexture(MainMenu::textureRankB); break;
					case 4: GuiTextureResources::textureRankDisplay->setTexture(MainMenu::textureRankA); break;
					default: break;
				}
				GuiManager::addGuiToRender(GuiTextureResources::textureRankDisplay);
				//AudioPlayer::play(44, Global::gamePlayer->getPosition());
			}
		}

		if (timeNew - previousTime >= 1.0)
		{
			std::fprintf(stdout, "fps: %f\n", frameCount / (timeNew - previousTime));
			//std::fprintf(stdout, "diff: %d\n", Global::countNew - Global::countDelete);
			//Loader::printInfo();
			//std::fprintf(stdout, "entity counts: %d %d %d\n", gameEntities.size(), gameEntitiesPass2.size(), gameTransparentEntities.size());
			frameCount = 0;
			previousTime = timeNew;
		}

		//std::fprintf(stdout, "dt: %f\n", dt);
		//std::this_thread::sleep_for(std::chrono::milliseconds(8));
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
	for (auto entityToAdd : gameEntitiesToAdd)
	{
		gameEntities.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
	}
	gameEntitiesToAdd.clear();

	for (auto entityToDelete : gameEntitiesToDelete)
	{
		gameEntities.erase(entityToDelete);
		delete entityToDelete;
		INCR_DEL
	}
	gameEntitiesToDelete.clear();


	//Delete all the rest
	for (auto entityToDelete : gameEntities)
	{
		delete entityToDelete.first;
		INCR_DEL
	}
	gameEntities.clear();
}

void Main_addEntityPass2(Entity* entityToAdd)
{
	gameEntitiesPass2ToAdd.push_back(entityToAdd);
}

void Main_deleteEntityPass2(Entity* entityToDelete)
{
	gameEntitiesPass2ToDelete.push_back(entityToDelete);
}

void Main_deleteAllEntitesPass2()
{
	//Make sure no entities get left behind in transition
	for (auto entityToAdd : gameEntitiesPass2ToAdd)
	{
		gameEntitiesPass2.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
	}
	gameEntitiesPass2ToAdd.clear();

	for (auto entityToDelete : gameEntitiesPass2ToDelete)
	{
		gameEntitiesPass2.erase(entityToDelete);
		delete entityToDelete;
		INCR_DEL
	}
	gameEntitiesPass2ToDelete.clear();

	for (auto entityToDelete : gameEntitiesPass2)
	{
		delete entityToDelete.first;
		INCR_DEL
	}
	gameEntitiesPass2.clear();
}

void Main_addEntityPass3(Entity* entityToAdd)
{
	gameEntitiesPass3ToAdd.push_back(entityToAdd);
}

void Main_deleteEntityPass3(Entity* entityToDelete)
{
	gameEntitiesPass3ToDelete.push_back(entityToDelete);
}

void Main_deleteAllEntitesPass3()
{
	//Make sure no entities get left behind in transition
	for (auto entityToAdd : gameEntitiesPass3ToAdd)
	{
		gameEntitiesPass3.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
	}
	gameEntitiesPass3ToAdd.clear();

	for (auto entityToDelete : gameEntitiesPass3ToDelete)
	{
		gameEntitiesPass3.erase(entityToDelete);
		delete entityToDelete;
		INCR_DEL
	}
	gameEntitiesPass3ToDelete.clear();

	for (auto entityToDelete : gameEntitiesPass3)
	{
		delete entityToDelete.first;
		INCR_DEL
	}
	gameEntitiesPass3.clear();
}

//Transparent entities shouldn't create new transparent entities from within their step function
void Main_addTransparentEntity(Entity* entityToAdd)
{
	gameTransparentEntities.insert(std::pair<Entity*, Entity*>(entityToAdd, entityToAdd));
}

void Main_deleteTransparentEntity(Entity* entityToDelete)
{
	gameTransparentEntities.erase(entityToDelete);
	delete entityToDelete;
	INCR_DEL
}

void Main_deleteAllTransparentEntites()
{
	for (auto entityToDelete : gameTransparentEntities)
	{
		delete entityToDelete.first;
		INCR_DEL
	}
	gameTransparentEntities.clear();
}

void increaseProcessPriority()
{
	#ifdef _WIN32
	//DWORD dwError;//, dwPriClass;

	/*
	if (!SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS))
	{
		dwError = GetLastError();
		_tprintf(TEXT("Failed to enter above normal mode (%d)\n"), (int)dwError);
	}

	if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL))
	{
		dwError = GetLastError();
		_tprintf(TEXT("Failed to enter above normal mode (%d)\n"), (int)dwError);
	}
	*/

	/*
	if (!SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS))
	{
		dwError = GetLastError();
		_tprintf(TEXT("Failed to enter above normal mode (%d)\n"), (int)dwError);
	}

	if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL))
	{
		dwError = GetLastError();
		_tprintf(TEXT("Failed to enter above normal mode (%d)\n"), (int)dwError);
	}
	*/

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

	std::ifstream file("res/SaveData/SaveData.sav");
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
		Global::gameTotalPlaytime = std::stof(Global::gameSaveData["PLAYTIME"]);
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

void Global::saveSaveData()
{
	#ifdef _WIN32
	_mkdir("res/SaveData");
	#else
	mkdir("res/SaveData", 0777);
	#endif

	std::ofstream file;
	file.open("res/SaveData/SaveData.sav", std::ios::out | std::ios::trunc);

	if (!file.is_open())
	{
		std::fprintf(stderr, "Error: Failed to create/access 'res/SaveData/SaveData.sav'\n");
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

		float newTime   = GuiManager::getTotalTimerInSeconds();
		float savedTime = 6000.0f;

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
		else if (missionType == "Ring" || missionType == "Chao")
		{
			int timeForRankA = std::stoi((currentLevel->missionData[Global::gameMissionNumber])[1]);
			int timeForRankB = (4*timeForRankA)/3;
			int timeForRankC = (3*timeForRankA)/2;
			int timeForRankD = (2*timeForRankA)/1;

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
			//	Global::gamePlayer->setGravity(0);
			//}
			//else if (input == "gon")
			//{
			//	Global::gamePlayer->setGravity(0.08f);
			//}
			//else if (input.c_str()[0] == 'x')
			//{
			//	const char* data = &input.c_str()[1];
			//	Global::gamePlayer->setX(std::stof(data));
			//	Global::gamePlayer->setGravity(0);
			//}
			//else if (input.c_str()[0] == 'y')
			//{
			//	const char* data = &input.c_str()[1];
			//	Global::gamePlayer->setY(std::stof(data));
			//	Global::gamePlayer->setGravity(0);
			//}
			//else if (input.c_str()[0] == 'z')
			//{
			//	const char* data = &input.c_str()[1];
			//	Global::gamePlayer->setZ(std::stof(data));
			//	Global::gamePlayer->setGravity(0);
			//}
			//else
			//{
			//	char lineBuf[256];
			//	memcpy(lineBuf, input.c_str(), input.size()+1);
			//
			//	int splitLength = 0;
			//	char** lineSplit = split(lineBuf, ' ', &splitLength);
			//
			//	if (splitLength == 3)
			//	{
			//		Global::gamePlayer->setX(std::stof(lineSplit[0]));
			//		Global::gamePlayer->setY(std::stof(lineSplit[1]));
			//		Global::gamePlayer->setZ(std::stof(lineSplit[2]));
			//		Global::gamePlayer->setGravity(0);
			//	}
			//	free(lineSplit);
			//}
		}
	}
}
