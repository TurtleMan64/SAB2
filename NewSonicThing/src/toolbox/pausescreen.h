#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

class FontType;
class GUIText;


class PauseScreen
{
private:
	static int menuSelection;
	static int menuSelectionMAX;
	static int menuDisplayID;
	static const int ROOT = 0;

	static int moveYPrevious;
	static bool selectInputPrevious;
	static bool backInputPrevious;

	static GUIText* textCursor;
	static GUIText* textResume;
	static GUIText* textCamera;
	static GUIText* textRestart;
	static GUIText* textQuit;

	static bool isPaused;
	static bool shouldPause;

	static bool pausedSounds[14];

public:
	static FontType* font;

	static void init();

	static void step();

	static void unpause(bool shouldResumeSFX);

	static void pause();
};

#endif