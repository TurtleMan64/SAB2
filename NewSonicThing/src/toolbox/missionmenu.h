#ifndef MISSIONMENU_H
#define MISSIONMENU_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include "../guis/guitexture.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "button.h"
#include "menu.h"

class MissionMenu : public Menu
{
private:
	static int index;
	GLuint textureParallelogram;
	GLuint textureParallelogramBackdrop;
	GLuint textureParallelogramHalf;
	GLuint textureParallelogramHalfBackdrop;
	GLuint textureParallelogramHalf2;
	GLuint textureParallelogramHalf2Backdrop;
	GLuint textureRankBlank;
	GLuint textureMissionSelect;
	GLuint textureNPCicon;

	int moveXPrevious;
	int moveYPrevious;
	bool selectInputPrevious;
	bool backInputPrevious;

	int holdUpTimer;
	int holdDownTimer;

	Button** levelButton;
	int counter;

public:
	GLuint textureRankA;
	GLuint textureRankB;
	GLuint textureRankC;
	GLuint textureRankD;
	GLuint textureRankE;

private:
	GuiTexture* itemRankDisplay;
	GuiTexture* itemRankDisplayBackdrop;
	GuiTexture* itemTimeDisplay;
	GuiTexture* itemTimeDisplayBackdrop;
	GuiTexture* itemScoreDisplay;
	GuiTexture* itemScoreDisplayBackdrop;

	GUIText* textBestScore;
	GUIText* textBestTime;

	GuiTexture* rankM1;
	GuiTexture* rankM2;
	GuiTexture* rankM3;
	GuiTexture* rankM4;

	GuiTexture* missionSelect;

	GuiTexture* npcIcon;
	FontType* font;

	void selectButton();

public:
	MissionMenu();
	~MissionMenu();
	void init();
	void loadResources();
	void unloadResources();
	int step();
	
	float fontSize;
};

#endif