#ifndef GUIMANAGER_H
#define GUIMANAGER_H

class FontType;
class GUIText;
class GUINumber;
class GuiTexture;

#include <list>

class GuiManager
{
private:
	static GUIText* timerColon;
	static GUIText* timerPeriod;
	static GUIText* timerMin1[10];
	static GUIText* timerMin2[10];
	static GUIText* timerSec1[10];
	static GUIText* timerSec2[10];
	static GUIText* timerCen1[10];
	static GUIText* timerCen2[10];

	static bool timerIsRunning;
	static float timerValue;

	static double timestampStart;
	static double timestampEnd;

	static GUIText* textLives;

	static GUINumber* numberSpeed;
	static GUIText* textSpeedUnits;

	static int previousLives;

	static std::list<GuiTexture*> guisToRender;

	static GUIText* textHorVel;
	static GUIText* textVerVel;
	static GUIText* textTotalVel;

	static GUIText* textA;
	static GUIText* textB;
	static GUIText* textX;
	static GUIText* textY;

	static float horVel;
	static float verVel;
	static float totalVel;

	static void setTimerInvisible();

	static float aspectRatio;
	static float safeAreaY;
	static float safeAreaX;
	static float w;
	static float s;

public:
	static FontType* fontVip;

	static void init();

	static void refresh();

	static void increaseTimer(float deltaTime);

	static void setTimer(float newVal);

	static void startTimer();

	static void stopTimer();

	//Total time on timer in seconds
	static float getTotalTimerInSeconds();

	static void addGuiToRender(GuiTexture* newImage);

	static void removeGui(GuiTexture* newImage);

	static void clearGuisToRender();
};

#endif