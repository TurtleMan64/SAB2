#ifndef MAINMENU_H
#define MAINMENU_H

#include <glad/glad.h>
#include <string>
#include <vector>

class FontType;
class GUIText;
class GuiTexture;

class MainMenu
{
private:
	static int menuSelectionID;
	static const int MISSION_1    =  0;
	static const int MISSION_2    =  1;
	static const int MISSION_3    =  2;
	static const int MISSION_4    =  3;
	static const int MISSION_5    =  4;
	static const int MISSION_6    =  5;
	static const int MISSION_7    =  6;
	static const int ROOT_STORY   = 100000;
	static const int ROOT_MISSION = 100001;
	static const int ROOT_EXTRAS  = 100002;
	static const int ROOT_EXIT    = 100003;
	static const int EXTRAS       = 100004;
	static const int ARCADE_CLEAR = 100005;

	static const int MISSION_MAX = MISSION_7;

	static int moveXPrevious;
	static int moveYPrevious;
	static bool selectInputPrevious;
	static bool backInputPrevious;

	static GUIText* textMission1; //WINDY_VALLEY   
	static GUIText* textMission2; //GREEN_FOREST   
	static GUIText* textMission3; //METAL_HARBOR   
	static GUIText* textMission4; //PYRAMID_CAVE   
	static GUIText* textMission5; //SPEED_HIGHWAY  
	static GUIText* textMission6; //RADICAL_HIGHWAY
	static GUIText* textMission7; //GREEN_HILL_ZONE

	static GUIText* textTitleCardLevelName;
	static GUIText* textTitleCardMission;
	static GUIText* textTitleCardMissionDescription;

	static GUIText* textItem1; //GRAND PRIX
	static GUIText* textItem2; //MISSIONS
	static GUIText* textItem3; //EXTRAS
	static GUIText* textItem4; //QUIT

	//static GUIText* textExtra1Title; //Number of A ranks
	//static GUIText* textExtra1Data;
	//static GUIText* textExtra2Title; //Number of NPC found
	//static GUIText* textExtra2Data;
	static GUIText* textExtra3Title; //Total playtime
	static GUIText* textExtra3Data;
	static GUIText* textExtra4Title; //Best Arcade clear time
	static GUIText* textExtra4Data;
	static GUIText* textExtra5Title; //Unlocked Character select
	static GUIText* textExtra5Data;

	static GUIText* textArcadeResultTitle; //Total time during arcade mode
	static GUIText* textArcadeResultData;  //Total time during arcade mode

	static GLuint textureParallelogram;
	static GLuint textureParallelogramBackdrop;
	static GLuint textureParallelogramHalf;
	static GLuint textureParallelogramHalfBackdrop;
	static GLuint textureParallelogramHalf2;
	static GLuint textureParallelogramHalf2Backdrop;
	static GLuint textureRankBlank;
	static GLuint textureMissionSelect;

	static GLuint textureNPCicon;

	static float aspectRatio;

public:
	static GLuint textureRankA;
	static GLuint textureRankB;
	static GLuint textureRankC;
	static GLuint textureRankD;
	static GLuint textureRankE;

private:
	//Parallelograms for ROOT
	static GuiTexture* item1;
	static GuiTexture* item2;
	static GuiTexture* item3;
	static GuiTexture* item4;
	static GuiTexture* itemBackdrop1;
	static GuiTexture* itemBackdrop2;
	static GuiTexture* itemBackdrop3;
	static GuiTexture* itemBackdrop4;

	//Parallelograms for MISSION
	static GuiTexture* itemMission1;
	static GuiTexture* itemMission2;
	static GuiTexture* itemMission3;
	static GuiTexture* itemMission4;
	static GuiTexture* itemMission5;
	static GuiTexture* itemMission6;
	static GuiTexture* itemMission7;
	static GuiTexture* itemMissionBackdrop1;
	static GuiTexture* itemMissionBackdrop2;
	static GuiTexture* itemMissionBackdrop3;
	static GuiTexture* itemMissionBackdrop4;
	static GuiTexture* itemMissionBackdrop5;
	static GuiTexture* itemMissionBackdrop6;
	static GuiTexture* itemMissionBackdrop7;

	static GuiTexture* itemRankDisplay;
	static GuiTexture* itemRankDisplayBackdrop;
	static GuiTexture* itemTimeDisplay;
	static GuiTexture* itemTimeDisplayBackdrop;
	static GuiTexture* itemScoreDisplay;
	static GuiTexture* itemScoreDisplayBackdrop;

	static GUIText* textBestScore;
	static GUIText* textBestTime;

	static GuiTexture* rankM1;
	static GuiTexture* rankM2;
	static GuiTexture* rankM3;
	static GuiTexture* rankM4;

	static GuiTexture* missionSelect;

	static GuiTexture* npcIcon;

public:
	static std::vector<std::string> unlockedCharacters;
	static int characterSelectIndex;

private:
	static int titleCardTextTimer;

	static int holdUpTimer;
	static int holdDownTimer;

public:
	static void createTitleCard();

	static void selectMenuArcadeClear();

private:
	static void selectMenuRoot(int newSelection);

	static void selectMenuMission(int newSelection);

	static void selectMenuExtra();

	static void unloadResources();

	static std::string convertSecondsToTimeString(float timerSeconds);

	static void updateBestDisplay(int levelID);

public:
	static FontType* font;

	static void init();

	static void step();

	static void loadResources();
};

#endif