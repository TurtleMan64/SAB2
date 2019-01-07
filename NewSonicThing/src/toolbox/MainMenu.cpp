#include "mainmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <algorithm>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
#include "../fontMeshCreator/guitext.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particletexture.h"
#include "../entities/camera.h"
#include "../particles/particlemaster.h"
#include "../guis/guitextureresources.h"
#include "../guis/guimanager.h"
#include "../guis/guitexture.h"

int MainMenu::menuSelectionID = 0;

int MainMenu::moveXPrevious = 0;
int MainMenu::moveYPrevious = 0;
bool MainMenu::selectInputPrevious = false;
bool MainMenu::backInputPrevious = false;

GUIText* MainMenu::textMission1  = nullptr; //WINDY_VALLEY   
GUIText* MainMenu::textMission2  = nullptr; //GREEN_FOREST   
GUIText* MainMenu::textMission3  = nullptr; //METAL_HARBOR   
GUIText* MainMenu::textMission4  = nullptr; //PYRAMID_CAVE   
GUIText* MainMenu::textMission5  = nullptr; //SPEED_HIGHWAY  
GUIText* MainMenu::textMission6  = nullptr; //RADICAL_HIGHWAY
GUIText* MainMenu::textMission7  = nullptr; //GREEN_HILL_ZONE

GUIText* MainMenu::textTitleCardLevelName          = nullptr;
GUIText* MainMenu::textTitleCardMission            = nullptr;
GUIText* MainMenu::textTitleCardMissionDescription = nullptr;

GUIText* MainMenu::textItem1 = nullptr;
GUIText* MainMenu::textItem2 = nullptr;
GUIText* MainMenu::textItem3 = nullptr;
GUIText* MainMenu::textItem4 = nullptr;

//GUIText* MainMenu::textExtra1Title = nullptr; //Number of A ranks
//GUIText* MainMenu::textExtra1Data  = nullptr;
//GUIText* MainMenu::textExtra2Title = nullptr; //Number of NPC found
//GUIText* MainMenu::textExtra2Data  = nullptr;
GUIText* MainMenu::textExtra3Title = nullptr; //Total playtime
GUIText* MainMenu::textExtra3Data  = nullptr;
GUIText* MainMenu::textExtra4Title = nullptr; //Best Arcade Clear Time
GUIText* MainMenu::textExtra4Data  = nullptr;
GUIText* MainMenu::textExtra5Title = nullptr; //Character selection
GUIText* MainMenu::textExtra5Data  = nullptr;

GUIText* MainMenu::textArcadeResultTitle = nullptr; //Total time during arcade mode
GUIText* MainMenu::textArcadeResultData  = nullptr; //Total time during arcade mode

GLuint MainMenu::textureParallelogram              = GL_NONE;
GLuint MainMenu::textureParallelogramBackdrop      = GL_NONE;
GLuint MainMenu::textureParallelogramHalf          = GL_NONE;
GLuint MainMenu::textureParallelogramHalfBackdrop  = GL_NONE;
GLuint MainMenu::textureParallelogramHalf2         = GL_NONE;
GLuint MainMenu::textureParallelogramHalf2Backdrop = GL_NONE;
GLuint MainMenu::textureRankA					   = GL_NONE;
GLuint MainMenu::textureRankB					   = GL_NONE;
GLuint MainMenu::textureRankC					   = GL_NONE;
GLuint MainMenu::textureRankD					   = GL_NONE;
GLuint MainMenu::textureRankE					   = GL_NONE;
GLuint MainMenu::textureRankBlank				   = GL_NONE;
GLuint MainMenu::textureMissionSelect			   = GL_NONE;

GLuint MainMenu::textureNPCicon                    = GL_NONE;

GuiTexture* MainMenu::item1 = nullptr;
GuiTexture* MainMenu::item2 = nullptr;
GuiTexture* MainMenu::item3 = nullptr;
GuiTexture* MainMenu::item4 = nullptr;
GuiTexture* MainMenu::itemBackdrop1 = nullptr;
GuiTexture* MainMenu::itemBackdrop2 = nullptr;
GuiTexture* MainMenu::itemBackdrop3 = nullptr;
GuiTexture* MainMenu::itemBackdrop4 = nullptr;

GuiTexture* MainMenu::itemMission1  = nullptr;
GuiTexture* MainMenu::itemMission2  = nullptr;
GuiTexture* MainMenu::itemMission3  = nullptr;
GuiTexture* MainMenu::itemMission4  = nullptr;
GuiTexture* MainMenu::itemMission5  = nullptr;
GuiTexture* MainMenu::itemMission6  = nullptr;
GuiTexture* MainMenu::itemMission7  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop1  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop2  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop3  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop4  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop5  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop6  = nullptr;
GuiTexture* MainMenu::itemMissionBackdrop7  = nullptr;

GuiTexture* MainMenu::itemRankDisplay          = nullptr;
GuiTexture* MainMenu::itemRankDisplayBackdrop  = nullptr;
GuiTexture* MainMenu::itemTimeDisplay          = nullptr;
GuiTexture* MainMenu::itemTimeDisplayBackdrop  = nullptr;
GuiTexture* MainMenu::itemScoreDisplay         = nullptr;
GuiTexture* MainMenu::itemScoreDisplayBackdrop = nullptr;

GUIText* MainMenu::textBestScore = nullptr;
GUIText* MainMenu::textBestTime  = nullptr;

GuiTexture* MainMenu::rankM1 = nullptr;
GuiTexture* MainMenu::rankM2 = nullptr;
GuiTexture* MainMenu::rankM3 = nullptr;
GuiTexture* MainMenu::rankM4 = nullptr;

GuiTexture* MainMenu::missionSelect = nullptr;

GuiTexture* MainMenu::npcIcon = nullptr;

std::vector<std::string> MainMenu::unlockedCharacters;
int MainMenu::characterSelectIndex = 0;

int MainMenu::titleCardTextTimer = 0;

int MainMenu::holdUpTimer = 0;
int MainMenu::holdDownTimer = 0;

FontType* MainMenu::font = nullptr;

void MainMenu::init()
{
	font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt"); INCR_NEW

	MainMenu::titleCardTextTimer = 0;

	textureParallelogram              = Loader::loadTextureNoInterpolation("res/Images/MainMenu/Parallelogram.png");
	textureParallelogramBackdrop      = Loader::loadTextureNoInterpolation("res/Images/MainMenu/ParallelogramBackdrop.png");
	textureParallelogramHalf          = Loader::loadTextureNoInterpolation("res/Images/MainMenu/ParallelogramHalf.png");
	textureParallelogramHalfBackdrop  = Loader::loadTextureNoInterpolation("res/Images/MainMenu/ParallelogramHalfBackdrop.png");
	textureParallelogramHalf2         = Loader::loadTextureNoInterpolation("res/Images/MainMenu/ParallelogramHalf2.png");
	textureParallelogramHalf2Backdrop = Loader::loadTextureNoInterpolation("res/Images/MainMenu/ParallelogramHalf2Backdrop.png");
	textureRankA					  = Loader::loadTexture               ("res/Images/MainMenu/RankA.png");
	textureRankB					  = Loader::loadTexture               ("res/Images/MainMenu/RankB.png");
	textureRankC					  = Loader::loadTexture               ("res/Images/MainMenu/RankC.png");
	textureRankD					  = Loader::loadTexture               ("res/Images/MainMenu/RankD.png");
	textureRankE					  = Loader::loadTexture               ("res/Images/MainMenu/RankE.png");
	textureRankBlank				  = Loader::loadTextureNoInterpolation("res/Images/MainMenu/NoRank.png");
	textureMissionSelect			  = Loader::loadTextureNoInterpolation("res/Images/MainMenu/MissionSelect.png");
	textureNPCicon                    = Loader::loadTexture               ("res/Images/MainMenu/NPCicon.png");

	MainMenu::loadResources();

	MainMenu::selectMenuRoot(ROOT_STORY);
}

void MainMenu::loadResources()
{
	if (textItem1 != nullptr)
	{
		std::fprintf(stdout, "Main Menu trying to load resources when they are already loaded!\n");
		return;
	}

	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;
	float px = 1.0f/(SCR_WIDTH);  //1 pixel in x dimension
	float py = 1.0f/(SCR_HEIGHT); //1 pixel in y dimension

	float fontScale = 2.85f;

	if (SCR_HEIGHT > 900)
	{
		fontScale = 2.55f;
	}

	textItem1 = new GUIText("GRAND PRIX",fontScale, font, 0.0f,   0.333f-0.04f, 0.666f, true, false, true); INCR_NEW
	textItem2 = new GUIText("MISSIONS",  fontScale, font, 0.333f, 0.333f-0.04f, 0.666f, true, false, true); INCR_NEW
	textItem3 = new GUIText("EXTRAS",    fontScale, font, 0.0f,   0.666f-0.04f, 0.666f, true, false, true); INCR_NEW
	textItem4 = new GUIText("EXIT",      fontScale, font, 0.333f, 0.666f-0.04f, 0.666f, true, false, true); INCR_NEW

	item1 = new GuiTexture(textureParallelogram, 0.333f, 0.333f, 512*px, 64*py, 0); INCR_NEW
	item2 = new GuiTexture(textureParallelogram, 0.666f, 0.333f, 512*px, 64*py, 0); INCR_NEW
	item3 = new GuiTexture(textureParallelogram, 0.333f, 0.666f, 512*px, 64*py, 0); INCR_NEW
	item4 = new GuiTexture(textureParallelogram, 0.666f, 0.666f, 512*px, 64*py, 0); INCR_NEW
	itemBackdrop1 = new GuiTexture(textureParallelogramBackdrop, 0.333f+8*px, 0.333f+6*py, 512*px, 64*py, 0); INCR_NEW
	itemBackdrop2 = new GuiTexture(textureParallelogramBackdrop, 0.666f+8*px, 0.333f+6*py, 512*px, 64*py, 0); INCR_NEW
	itemBackdrop3 = new GuiTexture(textureParallelogramBackdrop, 0.333f+8*px, 0.666f+6*py, 512*px, 64*py, 0); INCR_NEW
	itemBackdrop4 = new GuiTexture(textureParallelogramBackdrop, 0.666f+8*px, 0.666f+6*py, 512*px, 64*py, 0); INCR_NEW

	itemMission1  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMission2  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMission3  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMission4  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMission5  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMission6  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMission7  = new GuiTexture(textureParallelogramHalf, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop1  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop2  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop3  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop4  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop5  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop6  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW
	itemMissionBackdrop7  = new GuiTexture(textureParallelogramHalfBackdrop, 0.0f, 0.0f, 1024*px, 64*py, 0); INCR_NEW

	itemRankDisplay          = new GuiTexture(textureParallelogram,              0.75f,             0.5f,        512*px,  64*py, 0); INCR_NEW
	itemRankDisplayBackdrop  = new GuiTexture(textureParallelogramBackdrop,      0.75f+8*px,        0.5f+6*py,   512*px,  64*py, 0); INCR_NEW
	itemTimeDisplay          = new GuiTexture(textureParallelogramHalf2,         0.75f+256*px,      0.5f+128*py, 1024*px, 64*py, 0); INCR_NEW
	itemTimeDisplayBackdrop  = new GuiTexture(textureParallelogramHalf2Backdrop, 0.75f+256*px+8*px, 0.5f+134*py, 1024*px, 64*py, 0); INCR_NEW
	itemScoreDisplay         = new GuiTexture(textureParallelogramHalf2,         0.75f+256*px,      0.5f+256*py, 1024*px, 64*py, 0); INCR_NEW
	itemScoreDisplayBackdrop = new GuiTexture(textureParallelogramHalf2Backdrop, 0.75f+256*px+8*px, 0.5f+262*py, 1024*px, 64*py, 0); INCR_NEW

	textBestScore = new GUIText("SCORE:",      2.0f, font, 0.75f-192*px, 0.5f+128*py-0.025f, 1-(0.75f-192*px), false, false, true); INCR_NEW
	textBestTime  = new GUIText("TIME:",       2.0f, font, 0.75f-192*px, 0.5f+256*py-0.025f, 1-(0.75f-192*px), false, false, true); INCR_NEW

	rankM1 = new GuiTexture(textureRankA, 0.75f-83*px, 0.5f, 64*px, 64*py, 0); INCR_NEW
	rankM2 = new GuiTexture(textureRankB, 0.75f-28*px, 0.5f, 64*px, 64*py, 0); INCR_NEW
	rankM3 = new GuiTexture(textureRankC, 0.75f+27*px, 0.5f, 64*px, 64*py, 0); INCR_NEW
	rankM4 = new GuiTexture(textureRankD, 0.75f+82*px, 0.5f, 64*px, 64*py, 0); INCR_NEW

	missionSelect = new GuiTexture(textureMissionSelect, 0.75f-83*px, 0.5f, 96*px, 96*py, 0); INCR_NEW

	npcIcon = new GuiTexture(textureNPCicon, 0.75f + 320*px, 0.5f, 128*px, 128*py, 0); INCR_NEW
	//TODO: make these text load from the Global::gameLevelData
	textMission1  = new GUIText("WINDY VALLEY",    2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW
	textMission2  = new GUIText("GREEN FOREST",    2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW
	textMission3  = new GUIText("METAL HARBOR",    2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW
	textMission4  = new GUIText("PYRAMID CAVE",    2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW
	textMission5  = new GUIText("SPEED HIGHWAY",   2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW
	textMission6  = new GUIText("RADICAL HIGHWAY", 2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW
	textMission7  = new GUIText("GREEN HILL ZONE", 2.0f, font, 0.0f, 0.0f, 0.5f-128*px, false, true, true); INCR_NEW

	int totalRanks = 0;
	int rankAs = 0;
	for (unsigned int i = 0; i < Global::gameLevelData.size(); i++)
	{
		Level* level = &Global::gameLevelData[i];
		int missionCount = level->numMissions;
		totalRanks += missionCount;

		std::string name = level->displayName;
		auto end = Global::gameSaveData.end();

		switch (missionCount)
		{
			case 4: if (Global::gameSaveData.find(name+"_M4_RANK") != end) if (Global::gameSaveData[name+"_M4_RANK"] == "A") rankAs++;
			case 3: if (Global::gameSaveData.find(name+"_M3_RANK") != end) if (Global::gameSaveData[name+"_M3_RANK"] == "A") rankAs++;
			case 2: if (Global::gameSaveData.find(name+"_M2_RANK") != end) if (Global::gameSaveData[name+"_M2_RANK"] == "A") rankAs++;
			case 1: if (Global::gameSaveData.find(name+"_M1_RANK") != end) if (Global::gameSaveData[name+"_M1_RANK"] == "A") rankAs++;
			default: break;
		}
	}

	unsigned int totalNPC = (unsigned int)Global::npcList.size();
	unsigned int foundNPC = 0;
	for (unsigned int i = 0; i < totalNPC; i++)
	{
		std::string npc = Global::npcList[i];
		auto end = Global::gameSaveData.end();

		if (Global::gameSaveData.find(npc) != end)
		{
			if (Global::gameSaveData[npc] == "true")
			{
				foundNPC++;
			}
		}
	}

	//if (Global::gameSaveData.find("BestArcadeClearTime") != Global::gameSaveData.end())
	//{
	//	if (Global::gameSaveData.find("UnlockedAmy") == Global::gameSaveData.end())
	//	{
	//		Global::gameSaveData["UnlockedAmy"] = "true";
	//		Global::saveSaveData();
	//	}
	//}
	//if (foundNPC >= totalNPC/2)
	//{
	//	if (Global::gameSaveData.find("UnlockedPacman") == Global::gameSaveData.end())
	//	{
	//		Global::gameSaveData["UnlockedPacman"] = "true";
	//		Global::saveSaveData();
	//	}
	//}
	//if (rankAs == totalRanks)
	//{
	//	if (Global::gameSaveData.find("UnlockedMetalSonic") == Global::gameSaveData.end())
	//	{
	//		Global::gameSaveData["UnlockedMetalSonic"] = "true";
	//		Global::saveSaveData();
	//	}
	//}
	//if (foundNPC == totalNPC)
	//{
	//	if (Global::gameSaveData.find("UnlockedNPC") == Global::gameSaveData.end())
	//	{
	//		Global::gameSaveData["UnlockedNPC"] = "true";
	//		Global::saveSaveData();
	//	}
	//}



	std::string playtime = "";
	int hrs = (int)(Global::gameTotalPlaytime/3600);
	int min = (int)((fmodf(Global::gameTotalPlaytime, 3600))/60);
	float sec =     (fmodf(Global::gameTotalPlaytime, 60));

	playtime += std::to_string(hrs)+" HOURS, ";
	playtime += std::to_string(min)+" MINUTES, ";
	playtime += std::to_string(sec)+" SECONDS";

	const float yoff = -0.06f; //what you add to top text to make center of both textst be the center

	unlockedCharacters.clear();
	unlockedCharacters.push_back("BLUE FALCON");
	unlockedCharacters.push_back("TWIN NORITA");
	unlockedCharacters.push_back("SONIC PHANTOM");
	unlockedCharacters.push_back("BLACK BULL");
	//unlockedCharacters.push_back("ARWING");

	MainMenu::characterSelectIndex = std::max(0, MainMenu::characterSelectIndex);
	MainMenu::characterSelectIndex = std::min(MainMenu::characterSelectIndex, (int)unlockedCharacters.size()-1);

	//textExtra1Title = new GUIText("A RANK:",         2, font, 0, (1/divisor)+yoff, 1, true, false, false); INCR_NEW
	//textExtra2Title = new GUIText("HIDDEN NPC:",     2, font, 0, (2/divisor)+yoff, 1, true, false, false); INCR_NEW
	textExtra3Title = new GUIText("TOTAL PLAYTIME:", 2, font, 0, (3/4.0f)+yoff , 1, true, false, false); INCR_NEW

	//textExtra1Data = new GUIText(std::to_string(rankAs)  +"/"+std::to_string(totalRanks), 2, font, 0, (1/divisor)-yoff, 1, true, false, false); INCR_NEW //Number of A ranks
	//textExtra2Data = new GUIText(std::to_string(foundNPC)+"/"+std::to_string(totalNPC),   2, font, 0, (2/divisor)-yoff, 1, true, false, false); INCR_NEW //Number of NPC found
	textExtra3Data = new GUIText(playtime, 2, font, 0, (3/4.0f), 1, true, false, false); INCR_NEW //Total playtime

	if (Global::gameSaveData.find("BestArcadeClearTime") == Global::gameSaveData.end())
	{
		textExtra4Title = new GUIText("BEST GRAND PRIX CLEAR TIME:", 2, font, 0, (2/4.0f)+yoff, 1, true, false, false); INCR_NEW
		textExtra4Data  = new GUIText("NONE", 2, font, 0, (2/4.0f), 1, true, false, false); INCR_NEW
	}
	else
	{
		textExtra4Title = new GUIText("BEST GRAND PRIX CLEAR TIME:", 2, font, 0, (2/4.0f)+yoff, 1, true, false, false); INCR_NEW
		textExtra4Data  = new GUIText(MainMenu::convertSecondsToTimeString(stof(Global::gameSaveData["BestArcadeClearTime"])), 2, font, 0, (2/4.0f), 1, true, false, false); INCR_NEW
	}

	textExtra5Title = new GUIText("PLAY AS:", 2, font, 0, (1/4.0f)+yoff, 1, true, false, false); INCR_NEW
	textExtra5Data  = new GUIText(unlockedCharacters[Global::gameMissionNumber], 2, font, 0, (1/4.0f), 1, true, false, false); INCR_NEW

	textArcadeResultTitle = new GUIText("GRAND PRIX CLEAR TIME:", 3, font, 0, 0.5f-0.09f, 1, true, false, false); INCR_NEW
	textArcadeResultData  = new GUIText(MainMenu::convertSecondsToTimeString(Global::gameArcadePlaytime), 3, font, 0, 0.5f, 1, true, false, false); INCR_NEW

	if (menuSelectionID <= MISSION_MAX)
	{
		MainMenu::selectMenuMission(menuSelectionID);
		missionSelect->setX(0.75f-83*px+(Global::gameMissionNumber*55)*px);
		MainMenu::updateBestDisplay(menuSelectionID);
	}
	else if (menuSelectionID <= ROOT_EXIT)
	{
		MainMenu::selectMenuRoot(menuSelectionID);
	}
	else if (menuSelectionID == EXTRAS)
	{
		MainMenu::selectMenuExtra();
	}
	else if (menuSelectionID == ARCADE_CLEAR)
	{
		MainMenu::selectMenuArcadeClear();
	}
}

void MainMenu::unloadResources()
{
	if (textItem1 == nullptr)
	{
		std::fprintf(stdout, "Main Menu trying to unload resources when they are already unloaded!\n");
		return;
	}

	textItem1->deleteMe(); delete textItem1; INCR_DEL textItem1 = nullptr;
	textItem2->deleteMe(); delete textItem2; INCR_DEL textItem2 = nullptr;
	textItem3->deleteMe(); delete textItem3; INCR_DEL textItem3 = nullptr;
	textItem4->deleteMe(); delete textItem4; INCR_DEL textItem4 = nullptr;

	textMission1 ->deleteMe(); delete textMission1;  INCR_DEL textMission1  = nullptr;
	textMission2 ->deleteMe(); delete textMission2;  INCR_DEL textMission2  = nullptr;
	textMission3 ->deleteMe(); delete textMission3;  INCR_DEL textMission3  = nullptr;
	textMission4 ->deleteMe(); delete textMission4;  INCR_DEL textMission4  = nullptr;
	textMission5 ->deleteMe(); delete textMission5;  INCR_DEL textMission5  = nullptr;
	textMission6 ->deleteMe(); delete textMission6;  INCR_DEL textMission6  = nullptr;
	textMission7 ->deleteMe(); delete textMission7;  INCR_DEL textMission7  = nullptr;

	//textExtra1Title->deleteMe(); delete textExtra1Title; INCR_DEL textExtra1Title = nullptr;
	//textExtra1Data ->deleteMe(); delete textExtra1Data;  INCR_DEL textExtra1Data  = nullptr;
	//textExtra2Title->deleteMe(); delete textExtra2Title; INCR_DEL textExtra2Title = nullptr;
	//textExtra2Data ->deleteMe(); delete textExtra2Data;  INCR_DEL textExtra2Data  = nullptr;
	textExtra3Title->deleteMe(); delete textExtra3Title; INCR_DEL textExtra3Title = nullptr;
	textExtra3Data ->deleteMe(); delete textExtra3Data;  INCR_DEL textExtra3Data  = nullptr;
	textExtra4Title->deleteMe(); delete textExtra4Title; INCR_DEL textExtra4Title = nullptr;
	textExtra4Data ->deleteMe(); delete textExtra4Data;  INCR_DEL textExtra4Data  = nullptr;
	textExtra5Title->deleteMe(); delete textExtra5Title; INCR_DEL textExtra5Title = nullptr;
	textExtra5Data ->deleteMe(); delete textExtra5Data;  INCR_DEL textExtra5Data  = nullptr;

	textArcadeResultTitle->deleteMe(); delete textArcadeResultTitle; INCR_DEL textArcadeResultTitle  = nullptr;
	textArcadeResultData ->deleteMe(); delete textArcadeResultData;  INCR_DEL textArcadeResultData   = nullptr;

	GuiManager::clearGuisToRender();

	delete item1; INCR_DEL item1 = nullptr;
	delete item2; INCR_DEL item2 = nullptr;
	delete item3; INCR_DEL item3 = nullptr;
	delete item4; INCR_DEL item4 = nullptr;
	delete itemBackdrop1; INCR_DEL itemBackdrop1 = nullptr;
	delete itemBackdrop2; INCR_DEL itemBackdrop2 = nullptr;
	delete itemBackdrop3; INCR_DEL itemBackdrop3 = nullptr;
	delete itemBackdrop4; INCR_DEL itemBackdrop4 = nullptr;

	delete itemMission1;  INCR_DEL itemMission1  = nullptr;
	delete itemMission2;  INCR_DEL itemMission2  = nullptr;
	delete itemMission3;  INCR_DEL itemMission3  = nullptr;
	delete itemMission4;  INCR_DEL itemMission4  = nullptr;
	delete itemMission5;  INCR_DEL itemMission5  = nullptr;
	delete itemMission6;  INCR_DEL itemMission6  = nullptr;
	delete itemMission7;  INCR_DEL itemMission7  = nullptr;
	delete itemMissionBackdrop1;  INCR_DEL itemMissionBackdrop1  = nullptr; 
	delete itemMissionBackdrop2;  INCR_DEL itemMissionBackdrop2  = nullptr; 
	delete itemMissionBackdrop3;  INCR_DEL itemMissionBackdrop3  = nullptr; 
	delete itemMissionBackdrop4;  INCR_DEL itemMissionBackdrop4  = nullptr; 
	delete itemMissionBackdrop5;  INCR_DEL itemMissionBackdrop5  = nullptr; 
	delete itemMissionBackdrop6;  INCR_DEL itemMissionBackdrop6  = nullptr; 
	delete itemMissionBackdrop7;  INCR_DEL itemMissionBackdrop7  = nullptr; 

	delete itemRankDisplay;          INCR_DEL itemRankDisplay          = nullptr;
	delete itemRankDisplayBackdrop;  INCR_DEL itemRankDisplayBackdrop  = nullptr;
	delete itemTimeDisplay;          INCR_DEL itemTimeDisplay          = nullptr;
	delete itemTimeDisplayBackdrop;  INCR_DEL itemTimeDisplayBackdrop  = nullptr;
	delete itemScoreDisplay;         INCR_DEL itemScoreDisplay         = nullptr;
	delete itemScoreDisplayBackdrop; INCR_DEL itemScoreDisplayBackdrop = nullptr;

	textBestScore->deleteMe(); delete textBestScore; INCR_DEL textBestScore = nullptr;
	textBestTime ->deleteMe(); delete textBestTime;  INCR_DEL textBestTime  = nullptr;

	delete rankM1; INCR_DEL rankM1 = nullptr;
	delete rankM2; INCR_DEL rankM2 = nullptr;
	delete rankM3; INCR_DEL rankM3 = nullptr;
	delete rankM4; INCR_DEL rankM4 = nullptr;

	delete missionSelect; INCR_DEL missionSelect = nullptr;

	delete npcIcon; INCR_DEL npcIcon = nullptr;
}

void MainMenu::selectMenuArcadeClear()
{
	GuiManager::clearGuisToRender();

	menuSelectionID = ARCADE_CLEAR;

	textItem1->setVisibility(false);
	textItem2->setVisibility(false);
	textItem3->setVisibility(false);
	textItem4->setVisibility(false);

	textMission1 ->setVisibility(false);
	textMission2 ->setVisibility(false);
	textMission3 ->setVisibility(false);
	textMission4 ->setVisibility(false);
	textMission5 ->setVisibility(false);
	textMission6 ->setVisibility(false);
	textMission7 ->setVisibility(false);

	textBestScore->setVisibility(false);
	textBestTime ->setVisibility(false);

	//textExtra1Title->setVisibility(false);
	//textExtra1Data ->setVisibility(false);
	//textExtra2Title->setVisibility(false);
	//textExtra2Data ->setVisibility(false);
	textExtra3Title->setVisibility(false);
	textExtra3Data ->setVisibility(false);
	textExtra4Title->setVisibility(false);
	textExtra4Data ->setVisibility(false);
	textExtra5Title->setVisibility(false);
	textExtra5Data ->setVisibility(false);

	textArcadeResultTitle->setVisibility(true);
	textArcadeResultData ->setVisibility(true);
}

void MainMenu::selectMenuMission(int newSelection)
{
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;
	float px = 1.0f/(SCR_WIDTH);  //1 pixel in x dimension
	float py = 1.0f/(SCR_HEIGHT); //1 pixel in y dimension

	GuiManager::clearGuisToRender();

	float offsetY = (MISSION_1 - newSelection)*128*py;

	float yPos = 0.5f;
	itemMission1 ->setY(yPos+offsetY); yPos+=128*py;
	itemMission2 ->setY(yPos+offsetY); yPos+=128*py;
	itemMission3 ->setY(yPos+offsetY); yPos+=128*py;
	itemMission4 ->setY(yPos+offsetY); yPos+=128*py;
	itemMission5 ->setY(yPos+offsetY); yPos+=128*py;
	itemMission6 ->setY(yPos+offsetY); yPos+=128*py;
	itemMission7 ->setY(yPos+offsetY);
	yPos = 0.5f+6.0f*py;
	itemMissionBackdrop1 ->setY(yPos+offsetY); yPos+=128*py;
	itemMissionBackdrop2 ->setY(yPos+offsetY); yPos+=128*py;
	itemMissionBackdrop3 ->setY(yPos+offsetY); yPos+=128*py;
	itemMissionBackdrop4 ->setY(yPos+offsetY); yPos+=128*py;
	itemMissionBackdrop5 ->setY(yPos+offsetY); yPos+=128*py;
	itemMissionBackdrop6 ->setY(yPos+offsetY); yPos+=128*py;
	itemMissionBackdrop7 ->setY(yPos+offsetY);

	itemMission1 ->setX(0.5f-512*px-64*px);
	itemMission2 ->setX(0.5f-512*px-64*px);
	itemMission3 ->setX(0.5f-512*px-64*px);
	itemMission4 ->setX(0.5f-512*px-64*px);
	itemMission5 ->setX(0.5f-512*px-64*px);
	itemMission6 ->setX(0.5f-512*px-64*px);
	itemMission7 ->setX(0.5f-512*px-64*px);
	itemMissionBackdrop1 ->setX(0.5f-512*px-64*px+8*px);
	itemMissionBackdrop2 ->setX(0.5f-512*px-64*px+8*px);
	itemMissionBackdrop3 ->setX(0.5f-512*px-64*px+8*px);
	itemMissionBackdrop4 ->setX(0.5f-512*px-64*px+8*px);
	itemMissionBackdrop5 ->setX(0.5f-512*px-64*px+8*px);
	itemMissionBackdrop6 ->setX(0.5f-512*px-64*px+8*px);
	itemMissionBackdrop7 ->setX(0.5f-512*px-64*px+8*px);

	yPos = 0.5f-0.025f;
	textMission1 ->getPosition()->y = yPos+offsetY; yPos+=128*py;
	textMission2 ->getPosition()->y = yPos+offsetY; yPos+=128*py;
	textMission3 ->getPosition()->y = yPos+offsetY; yPos+=128*py;
	textMission4 ->getPosition()->y = yPos+offsetY; yPos+=128*py;
	textMission5 ->getPosition()->y = yPos+offsetY; yPos+=128*py;
	textMission6 ->getPosition()->y = yPos+offsetY; yPos+=128*py;
	textMission7 ->getPosition()->y = yPos+offsetY;
	
	textMission1 ->getPosition()->x = 0.0f;
	textMission2 ->getPosition()->x = 0.0f;
	textMission3 ->getPosition()->x = 0.0f;
	textMission4 ->getPosition()->x = 0.0f;
	textMission5 ->getPosition()->x = 0.0f;
	textMission6 ->getPosition()->x = 0.0f;
	textMission7 ->getPosition()->x = 0.0f;

	GuiManager::addGuiToRender(itemMission1);
	GuiManager::addGuiToRender(itemMission2);
	GuiManager::addGuiToRender(itemMission3);
	GuiManager::addGuiToRender(itemMission4);
	GuiManager::addGuiToRender(itemMission5);
	GuiManager::addGuiToRender(itemMission6);
	GuiManager::addGuiToRender(itemMission7);

	GuiManager::addGuiToRender(itemRankDisplay);
	GuiManager::addGuiToRender(itemRankDisplayBackdrop);
	GuiManager::addGuiToRender(itemTimeDisplay);
	GuiManager::addGuiToRender(itemTimeDisplayBackdrop);
	GuiManager::addGuiToRender(itemScoreDisplay);
	GuiManager::addGuiToRender(itemScoreDisplayBackdrop);

	int numMissions = Global::gameLevelData[newSelection].numMissions;
	switch (numMissions)
	{
		case 4: GuiManager::addGuiToRender(rankM4);
		case 3: GuiManager::addGuiToRender(rankM3);
		case 2: GuiManager::addGuiToRender(rankM2);
		case 1: GuiManager::addGuiToRender(rankM1);
		default: break;
	}

	if (numMissions >= 4)
	{
		std::string rank = "";
		rankM4->setTexture(textureRankBlank);
		if (Global::gameSaveData.find(Global::gameLevelData[newSelection].displayName+"_M4_RANK") != Global::gameSaveData.end())
		{
			rank = Global::gameSaveData[Global::gameLevelData[newSelection].displayName+"_M4_RANK"];
			if (rank == "A") rankM4->setTexture(textureRankA);
			if (rank == "B") rankM4->setTexture(textureRankB);
			if (rank == "C") rankM4->setTexture(textureRankC);
			if (rank == "D") rankM4->setTexture(textureRankD);
			if (rank == "E") rankM4->setTexture(textureRankE);
		}
	}
	if (numMissions >= 3)
	{
		std::string rank = "";
		rankM3->setTexture(textureRankBlank);
		if (Global::gameSaveData.find(Global::gameLevelData[newSelection].displayName+"_M3_RANK") != Global::gameSaveData.end())
		{
			rank = Global::gameSaveData[Global::gameLevelData[newSelection].displayName+"_M3_RANK"];
			if (rank == "A") rankM3->setTexture(textureRankA);
			if (rank == "B") rankM3->setTexture(textureRankB);
			if (rank == "C") rankM3->setTexture(textureRankC);
			if (rank == "D") rankM3->setTexture(textureRankD);
			if (rank == "E") rankM3->setTexture(textureRankE);
		}
	}
	if (numMissions >= 2)
	{
		std::string rank = "";
		rankM2->setTexture(textureRankBlank);
		if (Global::gameSaveData.find(Global::gameLevelData[newSelection].displayName+"_M2_RANK") != Global::gameSaveData.end())
		{
			rank = Global::gameSaveData[Global::gameLevelData[newSelection].displayName+"_M2_RANK"];
			if (rank == "A") rankM2->setTexture(textureRankA);
			if (rank == "B") rankM2->setTexture(textureRankB);
			if (rank == "C") rankM2->setTexture(textureRankC);
			if (rank == "D") rankM2->setTexture(textureRankD);
			if (rank == "E") rankM2->setTexture(textureRankE);
		}
	}
	if (numMissions >= 1)
	{
		std::string rank = "";
		rankM1->setTexture(textureRankBlank);
		if (Global::gameSaveData.find(Global::gameLevelData[newSelection].displayName+"_M1_RANK") != Global::gameSaveData.end())
		{
			rank = Global::gameSaveData[Global::gameLevelData[newSelection].displayName+"_M1_RANK"];
			if (rank == "A") rankM1->setTexture(textureRankA);
			if (rank == "B") rankM1->setTexture(textureRankB);
			if (rank == "C") rankM1->setTexture(textureRankC);
			if (rank == "D") rankM1->setTexture(textureRankD);
			if (rank == "E") rankM1->setTexture(textureRankE);
		}
	}

	GuiManager::addGuiToRender(missionSelect);

	switch (newSelection)
	{
		case MISSION_1 : textMission1 ->getPosition()->x += 64*px; itemMission1 ->setX(0.5f-512*px); itemMissionBackdrop1 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop1);  break;
		case MISSION_2 : textMission2 ->getPosition()->x += 64*px; itemMission2 ->setX(0.5f-512*px); itemMissionBackdrop2 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop2);  break;
		case MISSION_3 : textMission3 ->getPosition()->x += 64*px; itemMission3 ->setX(0.5f-512*px); itemMissionBackdrop3 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop3);  break;
		case MISSION_4 : textMission4 ->getPosition()->x += 64*px; itemMission4 ->setX(0.5f-512*px); itemMissionBackdrop4 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop4);  break;
		case MISSION_5 : textMission5 ->getPosition()->x += 64*px; itemMission5 ->setX(0.5f-512*px); itemMissionBackdrop5 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop5);  break;
		case MISSION_6 : textMission6 ->getPosition()->x += 64*px; itemMission6 ->setX(0.5f-512*px); itemMissionBackdrop6 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop6);  break;
		case MISSION_7 : textMission7 ->getPosition()->x += 64*px; itemMission7 ->setX(0.5f-512*px); itemMissionBackdrop7 ->setX(0.5f-512*px+8*px); GuiManager::addGuiToRender(itemMissionBackdrop7);  break;
		default: break;
	}

	menuSelectionID = newSelection;

	unsigned int totalNPC = (unsigned int)Global::npcList.size();
	auto end = Global::gameSaveData.end();
	std::string base = Global::gameLevelData[newSelection].displayName+"_NPC_";
	int npcInThisStageCount = 0;

	for (unsigned int i = 0; i < totalNPC; i++)
	{
		std::string baseCheck = Global::npcList[i];
		baseCheck.erase(baseCheck.length()-1, std::string::npos);
		if (baseCheck == base)
		{
			npcInThisStageCount++;
		}
	}

	int totalNPCWeFound = 0;

	for (int i = 1; i <= npcInThisStageCount; i++)
	{
		std::string check = base + std::to_string(i);

		if (Global::gameSaveData.find(check) != end)
		{
			if (Global::gameSaveData[check] == "true")
			{
				totalNPCWeFound++;
			}
		}
	}

	if (totalNPCWeFound != npcInThisStageCount)
	{
		GuiManager::addGuiToRender(npcIcon);
	}

	missionSelect->setX(0.75f-83*px+(Global::gameMissionNumber*55)*px);
	MainMenu::updateBestDisplay(newSelection);

	textItem1->setVisibility(false);
	textItem2->setVisibility(false);
	textItem3->setVisibility(false);
	textItem4->setVisibility(false);

	textMission1 ->setVisibility(true);
	textMission2 ->setVisibility(true);
	textMission3 ->setVisibility(true);
	textMission4 ->setVisibility(true);
	textMission5 ->setVisibility(true);
	textMission6 ->setVisibility(true);
	textMission7 ->setVisibility(true);

	textBestScore->setVisibility(true);
	textBestTime ->setVisibility(true);

	//textExtra1Title->setVisibility(false);
	//textExtra1Data ->setVisibility(false);
	//textExtra2Title->setVisibility(false);
	//textExtra2Data ->setVisibility(false);
	textExtra3Title->setVisibility(false);
	textExtra3Data ->setVisibility(false);
	textExtra4Title->setVisibility(false);
	textExtra4Data ->setVisibility(false);
	textExtra5Title->setVisibility(false);
	textExtra5Data ->setVisibility(false);

	textArcadeResultTitle->setVisibility(false);
	textArcadeResultData ->setVisibility(false);

	AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
}

void MainMenu::selectMenuRoot(int newSelection)
{
	GuiManager::clearGuisToRender();
	GuiManager::addGuiToRender(item1); textItem1->getPosition()->y = 0.333f-0.04f;
	GuiManager::addGuiToRender(item2); textItem2->getPosition()->y = 0.333f-0.04f;
	GuiManager::addGuiToRender(item3); textItem3->getPosition()->y = 0.666f-0.04f;
	GuiManager::addGuiToRender(item4); textItem4->getPosition()->y = 0.666f-0.04f;

	switch (newSelection)
	{
		case ROOT_STORY:   GuiManager::addGuiToRender(itemBackdrop1); textItem1->getPosition()->y = 0.333f-0.06f; break;
		case ROOT_MISSION: GuiManager::addGuiToRender(itemBackdrop2); textItem2->getPosition()->y = 0.333f-0.06f; break;
		case ROOT_EXTRAS:  GuiManager::addGuiToRender(itemBackdrop3); textItem3->getPosition()->y = 0.666f-0.06f; break;
		case ROOT_EXIT:    GuiManager::addGuiToRender(itemBackdrop4); textItem4->getPosition()->y = 0.666f-0.06f; break;
		default: break;
	}

	menuSelectionID = newSelection;

	textItem1->setVisibility(true);
	textItem2->setVisibility(true);
	textItem3->setVisibility(true);
	textItem4->setVisibility(true);

	textMission1 ->setVisibility(false);
	textMission2 ->setVisibility(false);
	textMission3 ->setVisibility(false);
	textMission4 ->setVisibility(false);
	textMission5 ->setVisibility(false);
	textMission6 ->setVisibility(false);
	textMission7 ->setVisibility(false);

	textBestScore->setVisibility(false);
	textBestTime ->setVisibility(false);

	//textExtra1Title->setVisibility(false);
	//textExtra1Data ->setVisibility(false);
	//textExtra2Title->setVisibility(false);
	//textExtra2Data ->setVisibility(false);
	textExtra3Title->setVisibility(false);
	textExtra3Data ->setVisibility(false);
	textExtra4Title->setVisibility(false);
	textExtra4Data ->setVisibility(false);
	textExtra5Title->setVisibility(false);
	textExtra5Data ->setVisibility(false);

	textArcadeResultTitle->setVisibility(false);
	textArcadeResultData ->setVisibility(false);

	//Global::gameMissionNumber = 0;

	AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
}

void MainMenu::selectMenuExtra()
{
	GuiManager::clearGuisToRender();

	menuSelectionID = EXTRAS;

	textExtra5Title->deleteMe(); delete textExtra5Title; INCR_DEL textExtra5Title = nullptr;
	textExtra5Data ->deleteMe(); delete textExtra5Data;  INCR_DEL textExtra5Data  = nullptr;
	const float yoff = -0.06f;
	textExtra5Title = new GUIText("PLAY AS:", 2, font, 0, (1/4.0f)+yoff, 1, true, false, true); INCR_NEW
	textExtra5Data  = new GUIText(unlockedCharacters[Global::gameMissionNumber], 2, font, 0, (1/4.0f), 1, true, false, true); INCR_NEW

	textItem1->setVisibility(false);
	textItem2->setVisibility(false);
	textItem3->setVisibility(false);
	textItem4->setVisibility(false);

	textMission1 ->setVisibility(false);
	textMission2 ->setVisibility(false);
	textMission3 ->setVisibility(false);
	textMission4 ->setVisibility(false);
	textMission5 ->setVisibility(false);
	textMission6 ->setVisibility(false);
	textMission7 ->setVisibility(false);

	textBestScore->setVisibility(false);
	textBestTime ->setVisibility(false);

	//textExtra1Title->setVisibility(true);
	//textExtra1Data ->setVisibility(true);
	//textExtra2Title->setVisibility(true);
	//textExtra2Data ->setVisibility(true);
	textExtra3Title->setVisibility(true);
	textExtra3Data ->setVisibility(true);
	textExtra4Title->setVisibility(true);
	textExtra4Data ->setVisibility(true);
	textExtra5Title->setVisibility(true);
	textExtra5Data ->setVisibility(true);

	textArcadeResultTitle->setVisibility(false);
	textArcadeResultData ->setVisibility(false);

	AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
}

void MainMenu::step()
{
	bool shouldGoUp    = false;
	bool shouldGoDown  = false;
	bool shouldGoLeft  = false;
	bool shouldGoRight = false;
	bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
	bool pressedBack   = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);

	int moveX = (int)round(Input::inputs.INPUT_X);
	int moveY = (int)round(Input::inputs.INPUT_Y);

	MainMenu::characterSelectIndex = Global::gameMissionNumber;

	if (moveX != moveXPrevious)
	{
		if (moveX == 1)
		{
			shouldGoRight = true;
		}
		else if (moveX == -1)
		{
			shouldGoLeft = true;
		}
	}

	if (moveY != moveYPrevious)
	{
		if (moveY == -1)
		{
			shouldGoUp = true;
		}
		else if (moveY == 1)
		{
			shouldGoDown = true;
		}
	}

	moveXPrevious = moveX;
	moveYPrevious = moveY;

	if (moveY == -1)
	{
		holdUpTimer++;
	}
	else
	{
		holdUpTimer = 0;
	}

	if (moveY == 1)
	{
		holdDownTimer++;
	}
	else
	{
		holdDownTimer = 0;
	}

	if (holdUpTimer >= 20)
	{
		if (holdUpTimer % 5 == 0)
		{
			shouldGoUp = true;
		}
	}

	if (holdDownTimer >= 20)
	{
		if (holdDownTimer % 5 == 0)
		{
			shouldGoDown = true;
		}
	}

	if (MainMenu::titleCardTextTimer != 0)
	{
		if (MainMenu::titleCardTextTimer == 1)
		{
			if (textTitleCardLevelName != nullptr)
			{
				textTitleCardLevelName->deleteMe();
				delete textTitleCardLevelName; INCR_DEL
				textTitleCardLevelName = nullptr;
			}
			if (textTitleCardMission != nullptr)
			{
				textTitleCardMission->deleteMe();
				delete textTitleCardMission; INCR_DEL
				textTitleCardMission = nullptr;
			}
			if (textTitleCardMissionDescription != nullptr)
			{
				textTitleCardMissionDescription->deleteMe();
				delete textTitleCardMissionDescription; INCR_DEL
				textTitleCardMissionDescription = nullptr;
			}

			GuiManager::removeGui(GuiTextureResources::textureBlueLine);
		}

		MainMenu::titleCardTextTimer--;
	}


	if (Global::gameState == STATE_TITLE)
	{
		extern unsigned int SCR_WIDTH;
		float px = 1.0f/(SCR_WIDTH);  //1 pixel in x dimension

		if (menuSelectionID >= ROOT_STORY && menuSelectionID <= ROOT_EXIT)
		{
			if (shouldGoUp)
			{
				switch (menuSelectionID)
				{
					case ROOT_EXTRAS: MainMenu::selectMenuRoot(ROOT_STORY);   break;
					case ROOT_EXIT:   MainMenu::selectMenuRoot(ROOT_MISSION); break;
					default: break;
				}
			}
			else if (shouldGoDown)
			{
				switch (menuSelectionID)
				{
					case ROOT_STORY:   MainMenu::selectMenuRoot(ROOT_EXTRAS); break;
					case ROOT_MISSION: MainMenu::selectMenuRoot(ROOT_EXIT);   break;
					default: break;
				}
			}
			else if (shouldGoLeft)
			{
				switch (menuSelectionID)
				{
					case ROOT_MISSION: MainMenu::selectMenuRoot(ROOT_STORY);  break;
					case ROOT_EXIT:    MainMenu::selectMenuRoot(ROOT_EXTRAS); break;
					default: break;
				}
			}
			else if (shouldGoRight)
			{
				switch (menuSelectionID)
				{
					case ROOT_STORY:  MainMenu::selectMenuRoot(ROOT_MISSION); break;
					case ROOT_EXTRAS: MainMenu::selectMenuRoot(ROOT_EXIT);    break;
					default: break;
				}
			}
			else if (pressedSelect)
			{
				switch (menuSelectionID)
				{
					case ROOT_STORY:
					{
						Global::levelID = LVL_WINDY_VALLEY;
						//Global::gameMissionNumber = 0;
						Global::gameIsArcadeMode = true;
						Global::gameArcadePlaytime = 0;

						AudioPlayer::play(38, Global::gameCamera->getFadePosition1());

						Level* currentLevel = &Global::gameLevelData[Global::levelID];
						Global::levelName = currentLevel->fileName;
						Global::levelNameDisplay = currentLevel->displayName;
						Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size()-1];

						Global::isNewLevel = true;
						Global::shouldLoadLevel = true;
						MainMenu::unloadResources();
						MainMenu::createTitleCard();
						Global::gameState = STATE_RUNNING;
						break;
					}

					case ROOT_MISSION:
					{
						MainMenu::selectMenuMission(MISSION_1);
						Global::gameIsArcadeMode = false;
						break;
					}

					case ROOT_EXTRAS:
					{
						MainMenu::selectMenuExtra();
						break;
					}

					case ROOT_EXIT: 
					{
						Global::gameState = STATE_EXITING;
						break;
					}

					default: break;
				}
			}
		}
		else if (menuSelectionID >= MISSION_1 && menuSelectionID <= MISSION_MAX)
		{
			if (shouldGoUp)
			{
				if (menuSelectionID > MISSION_1)
				{
					MainMenu::selectMenuMission(menuSelectionID-1);
					//Global::gameMissionNumber = 0;
					//missionSelect->setX(0.75f-83*px);
				}
			}
			else if (shouldGoDown)
			{
				if (menuSelectionID < MISSION_MAX)
				{
					MainMenu::selectMenuMission(menuSelectionID+1);
					//Global::gameMissionNumber = 0;
					//missionSelect->setX(0.75f-83*px);
				}
			}
			else if (shouldGoLeft)
			{
				if (Global::gameMissionNumber > 0)
				{
					Global::gameMissionNumber--;
					missionSelect->setX(0.75f-83*px+(Global::gameMissionNumber*55)*px);
					MainMenu::updateBestDisplay(menuSelectionID);
					AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
				}
			}
			else if (shouldGoRight)
			{
				int maxMissionCount = Global::gameLevelData[menuSelectionID].numMissions;
				if (Global::gameMissionNumber < maxMissionCount-1)
				{
					Global::gameMissionNumber++;
					missionSelect->setX(0.75f-83*px+(Global::gameMissionNumber*55)*px);
					MainMenu::updateBestDisplay(menuSelectionID);
					AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
				}
			}
			else if (pressedSelect)
			{
				switch (menuSelectionID)
				{
					case MISSION_1: Global::levelID = LVL_WINDY_VALLEY   ; break;
					case MISSION_2: Global::levelID = LVL_GREEN_FOREST   ; break;
					case MISSION_3: Global::levelID = LVL_METAL_HARBOR   ; break;
					case MISSION_4: Global::levelID = LVL_PYRAMID_CAVE   ; break;
					case MISSION_5: Global::levelID = LVL_SPEED_HIGHWAY  ; break;
					case MISSION_6: Global::levelID = LVL_RADICAL_HIGHWAY; break;
					case MISSION_7: Global::levelID = LVL_GREEN_HILL_ZONE; break;
					default:                                               break;
				}

				MainMenu::characterSelectIndex = Global::gameMissionNumber;

				AudioPlayer::play(38, Global::gameCamera->getFadePosition1());

				Level* currentLevel = &Global::gameLevelData[Global::levelID];
				Global::levelName = currentLevel->fileName;
				Global::levelNameDisplay = currentLevel->displayName;
				Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size()-1];

				Global::isNewLevel = true;
				Global::shouldLoadLevel = true;
				MainMenu::unloadResources();
				MainMenu::createTitleCard();
				Global::gameState = STATE_RUNNING;
			}
			else if (pressedBack)
			{
				MainMenu::selectMenuRoot(ROOT_MISSION);
			}
		}
		if (menuSelectionID == EXTRAS)
		{
			if (unlockedCharacters.size() > 1)
			{
				if (shouldGoLeft || shouldGoRight)
				{
					if (shouldGoLeft)
					{
						MainMenu::characterSelectIndex--;
						if (MainMenu::characterSelectIndex < 0)
						{
							MainMenu::characterSelectIndex = ((int)unlockedCharacters.size())-1;
						}
					}
					if (shouldGoRight)
					{
						MainMenu::characterSelectIndex++;
						if (MainMenu::characterSelectIndex > ((int)unlockedCharacters.size())-1)
						{
							MainMenu::characterSelectIndex = 0;
						}
					}

					MainMenu::characterSelectIndex = std::max(0, MainMenu::characterSelectIndex);
					MainMenu::characterSelectIndex = std::min(MainMenu::characterSelectIndex, (int)unlockedCharacters.size()-1);

					Global::gameMissionNumber = MainMenu::characterSelectIndex;

					textExtra5Title->deleteMe(); delete textExtra5Title; INCR_DEL textExtra5Title = nullptr;
					textExtra5Data ->deleteMe(); delete textExtra5Data;  INCR_DEL textExtra5Data  = nullptr;
					const float yoff = -0.06f;
					textExtra5Title = new GUIText("PLAY AS:", 2, font, 0, (1/4.0f)+yoff, 1, true, false, true); INCR_NEW
					textExtra5Data  = new GUIText(unlockedCharacters[Global::gameMissionNumber], 2, font, 0, (1/4.0f), 1, true, false, true); INCR_NEW
				}
			}
			if (pressedBack)
			{
				MainMenu::selectMenuRoot(ROOT_EXTRAS);
			}
		}
		else if (menuSelectionID == ARCADE_CLEAR)
		{
			if (pressedBack)
			{
				MainMenu::selectMenuRoot(ROOT_STORY);
			}
		}
	}
}

void MainMenu::createTitleCard()
{
	ParticleMaster::deleteAllParticles();

	Vector3f vel(0,0,0);
	new Particle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &vel, 0, 1.0f, 0.0f,  50.0f, 0, true, false);
	GuiManager::addGuiToRender(GuiTextureResources::textureBlueLine);


	if (textTitleCardLevelName != nullptr)
	{
		textTitleCardLevelName->deleteMe();
		delete textTitleCardLevelName;
		INCR_DEL
		textTitleCardLevelName = nullptr;
	}
	if (textTitleCardMission != nullptr)
	{
		textTitleCardMission->deleteMe();
		delete textTitleCardMission;
		INCR_DEL
		textTitleCardMission = nullptr;
	}
	if (textTitleCardMissionDescription != nullptr)
	{
		textTitleCardMissionDescription->deleteMe();
		delete textTitleCardMissionDescription;
		INCR_DEL
		textTitleCardMissionDescription = nullptr;
	}

	textTitleCardLevelName          = new GUIText(Global::levelNameDisplay, 3.0f, font, 0.0f, 0.6f, 1.0f, true, false, true); INCR_NEW
	textTitleCardMission            = new GUIText("Mission "+std::to_string(Global::gameMissionNumber+1)+":", 2.5f, font, 0.0f, 0.7f, 1.0f, true, false, true); INCR_NEW
	textTitleCardMissionDescription = new GUIText(Global::gameMissionDescription, 2.0f, font, 0.0f, 0.8f, 1.0f, true, false, true); INCR_NEW

	MainMenu::titleCardTextTimer = 3;
}

std::string MainMenu::convertSecondsToTimeString(float timerSeconds)
{
	int cen = (int)(100*fmodf(timerSeconds, 1.0f));
	int sec = (int)(fmodf(timerSeconds, 60.0f));
	int min = (int)(timerSeconds/60.0f);

	std::string partMin = std::to_string(min);
	if (min < 10)
	{
		partMin = "0" + std::to_string(min);
	}
	std::string partSec = std::to_string(sec);
	if (sec < 10)
	{
		partSec = "0" + std::to_string(sec);
	}
	std::string partCen = std::to_string(cen);
	if (cen < 10)
	{
		partCen = "0" + std::to_string(cen);
	}
	std::string timer = partMin + ":" + partSec + "." + partCen;

	return timer;
}

void MainMenu::updateBestDisplay(int levelID)
{
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;
	float px = 1.0f/(SCR_WIDTH);  //1 pixel in x dimension
	float py = 1.0f/(SCR_HEIGHT); //1 pixel in y dimension

	std::string missionTimeString  = "ERROR";
	std::string missionScoreString = "ERROR";
	switch (Global::gameMissionNumber)
	{
		case 0: missionTimeString = "_M1_TIME"; missionScoreString = "_M1_SCORE"; break;
		case 1: missionTimeString = "_M2_TIME"; missionScoreString = "_M2_SCORE"; break;
		case 2: missionTimeString = "_M3_TIME"; missionScoreString = "_M3_SCORE"; break;
		case 3: missionTimeString = "_M4_TIME"; missionScoreString = "_M4_SCORE"; break;
		default: break;
	}

	if (Global::gameSaveData.find(Global::gameLevelData[levelID].displayName+missionTimeString) != Global::gameSaveData.end())
	{
		std::string time = Global::gameSaveData[Global::gameLevelData[levelID].displayName+missionTimeString];
		time = MainMenu::convertSecondsToTimeString(std::stof(time));
		textBestTime->deleteMe(); delete textBestTime; INCR_DEL
		textBestTime  = new GUIText("TIME: "+time, 2.0f, font, 0.75f-192*px, 0.5f+256*py-0.025f, 1-(0.75f-192*px), false, false, true); INCR_NEW
	}
	else
	{
		textBestTime->deleteMe(); delete textBestTime; INCR_DEL
		textBestTime  = new GUIText("TIME: ", 2.0f, font, 0.75f-192*px, 0.5f+256*py-0.025f, 1-(0.75f-192*px), false, false, true); INCR_NEW
	}

	if (Global::gameSaveData.find(Global::gameLevelData[levelID].displayName+missionScoreString) != Global::gameSaveData.end())
	{
		std::string score = Global::gameSaveData[Global::gameLevelData[levelID].displayName+missionScoreString];
		textBestScore->deleteMe(); delete textBestScore; INCR_DEL
		textBestScore = new GUIText("SCORE: "+score, 2.0f, font, 0.75f-192*px, 0.5f+128*py-0.025f, 1-(0.75f-192*px), false, false, true); INCR_NEW
	}
	else
	{
		textBestScore->deleteMe(); delete textBestScore; INCR_DEL
		textBestScore = new GUIText("SCORE: ", 2.0f, font, 0.75f-192*px, 0.5f+128*py-0.025f, 1-(0.75f-192*px), false, false, true); INCR_NEW
	}
}