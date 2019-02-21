#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H
#include "../fontMeshCreator/guitext.h"
#include"../fontMeshCreator/fonttype.h"
#include "menu.h"
#include "hud.h"


class PauseScreen : public Menu
{
private:
	int menuSelection;
	int menuSelectionMAX;
	int menuDisplayID;

	int moveYPrevious;
	bool selectInputPrevious;
	bool backInputPrevious;

	GUIText* textCursor;
	GUIText* textResume;
	GUIText* textCamera;
	GUIText* textRestart;
	GUIText* textQuit;

	bool pausedSounds[14];

	float size;
	void selectButton();
	void setVisible(bool);

	HUD* gameHud;

public:
	PauseScreen(HUD*);

	~PauseScreen();

	FontType* font;

	Menu* step();
};

#endif