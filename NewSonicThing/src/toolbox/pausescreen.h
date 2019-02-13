#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H
#include "../fontMeshCreator/guitext.h"
#include"../fontMeshCreator/fonttype.h"
#include "menu.h"


class PauseScreen : public Menu
{
private:
	int menuSelection;
	int menuSelectionMAX;
	int menuDisplayID;
	const int ROOT = 0;

	int moveYPrevious;
	bool selectInputPrevious;
	bool backInputPrevious;

	GUIText* textCursor;
	GUIText* textResume;
	GUIText* textCamera;
	GUIText* textRestart;
	GUIText* textQuit;

	static bool shouldPause;

	bool pausedSounds[14];

	float aspectRatio;
	float size;
	void selectButton();

public:
	PauseScreen();

	~PauseScreen();

	FontType* font;

	int step();
};

#endif