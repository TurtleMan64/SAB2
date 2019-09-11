#include "timer.h"
#include "../renderEngine/renderEngine.h"
#include "../fontMeshCreator/fonttype.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/guitext.h"
#include "../engineTester/main.h"
#include <iostream>

/* Timer initialized to 0.
 * font: font for the timer
 * posX, posY: position of the timer
 * scale: scale of the timer
 * alignment: where to anchor the timer
 * visible: set timer visibility */

Timer::Timer(FontType* font, float posX, float posY, float scale, int alignment, bool visible)
{
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;

	float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;

	this->minutes = new GUINumber(0, posX, posY, scale, alignment, visible, 2); INCR_NEW("GUINumber");
	this->colon = new GUIText(":", scale, font, posX + (0.118f / aspectRatio), posY, alignment, true); INCR_NEW("GUIText");
	this->seconds = new GUINumber(0, posX + (0.15f / aspectRatio), posY, scale, alignment, visible, 2); INCR_NEW("GUINumber");
	this->dot = new GUIText(".", scale, font, posX + (0.268f / aspectRatio), posY, alignment, true); INCR_NEW("GUIText");
	this->centiseconds = new GUINumber(0, posX + (0.30f / aspectRatio), posY, scale, alignment, visible, 2); INCR_NEW("GUINumber");

	this->totalTime = 0;
	this->frozen = false;
}


Timer::~Timer()
{
	std::cout << "Deleting timer\n";
    if (Global::mainHudTimer == this)
    {
        Global::mainHudTimer = nullptr;
    }
	this->colon->deleteMe(); delete this->colon; INCR_DEL("GUIText");
	this->dot->deleteMe(); delete this->dot; INCR_DEL("GUIText");
	this->minutes->deleteMe(); delete this->minutes; INCR_DEL("GUINumber");
	this->seconds->deleteMe(); delete this->seconds; INCR_DEL("GUINumber");
	this->centiseconds->deleteMe(); delete this->centiseconds; INCR_DEL("GUINumber");
}

void Timer::increment()
{
	extern float dt;
	if (!this->frozen)
	{
		this->totalTime += dt;
	}
}

void Timer::refresh()
{
	// Convert float timer to ints of mm:ss:cc
	int toMinutesSeconds = (int)totalTime;
	int minuteDisplay = toMinutesSeconds / 60;
	int secondDisplay = toMinutesSeconds % 60;
	float toCentiseconds = totalTime * 100.0f;
	int centisecondDisplay = (int)toCentiseconds % 100;

	this->minutes->displayNumber = minuteDisplay;
	this->seconds->displayNumber = secondDisplay;
	this->centiseconds->displayNumber = centisecondDisplay;

	this->minutes->refresh();
	this->seconds->refresh();
	this->centiseconds->refresh();
}

float Timer::getTime()
{
	return this->totalTime;
}

void Timer::setTime(float newTime)
{
	this->totalTime = newTime;
}

void Timer::freeze(bool freezeStatus)
{
	this->frozen = freezeStatus;
}
