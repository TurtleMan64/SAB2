#include "hud.h"

#include "../toolbox/input.h"
#include "pausescreen.h"
#include "../engineTester/main.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "timer.h"


HUD::HUD()
{
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;

	this->aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	this->safeAreaY = 0.03f;
	this->safeAreaX = safeAreaY / this->aspectRatio;

	this->s = 0.05f;
	this->w = 0.8f * s / this->aspectRatio; //width of a single text character

	this->numberLives = new GUINumber(Global::gameLives, safeAreaX, 1.0f - safeAreaY, s, 6, true, 2); INCR_NEW("GUINumber");

    this->numberFPS = new GUINumber(Global::currentCalculatedFPS, 1.0f, 0.0f, s, 2, Global::displayFPS, 0); INCR_NEW("GUINumber");

	this->timer = new Timer(Global::fontVipnagorgialla, safeAreaX, safeAreaY, s, 0, true); INCR_NEW("Timer");
    Global::mainHudTimer = this->timer;

	this->speedometerScale = 1.5f;

	this->numberSpeed = new GUINumber(Global::gameMainVehicleSpeed, 1.0f - safeAreaX - (4 * w), 1.0f - safeAreaY, speedometerScale * s, 8, true, 3); INCR_NEW("GUINumber");
	this->textSpeedUnits = new GUIText("km/h", s, Global::fontVipnagorgialla, 1 - safeAreaX, 1.0f - safeAreaY, 8, true); INCR_NEW("GUIText");
}

HUD::~HUD()
{
	delete this->timer; INCR_DEL("Timer");
    this->numberFPS->deleteMe(); delete this->numberFPS; INCR_DEL("GUINumber");
	this->numberLives->deleteMe(); delete this->numberLives; INCR_DEL("GUINumber");
	this->numberSpeed->deleteMe(); delete this->numberSpeed; INCR_DEL("GUINumber");
	this->textSpeedUnits->deleteMe(); delete this->textSpeedUnits; INCR_DEL("GUIText");
}

Timer* HUD::getTimer()
{
	return this->timer;
}

void HUD::draw()
{
    if (Global::displayFPS)
    {
        this->numberFPS->visible = true;
        this->numberFPS->displayNumber = Global::currentCalculatedFPS;
	    this->numberFPS->refresh();
    }
    else
    {
        this->numberFPS->visible = false;
    }
	this->numberLives->displayNumber = Global::gameLives;
	this->numberLives->refresh();
	this->numberSpeed->displayNumber = Global::gameMainVehicleSpeed;
	this->numberSpeed->refresh();

	this->timer->refresh();
}

Menu* HUD::step()
{
	Menu* retVal = nullptr;

	if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START &&
        Global::finishStageTimer < 0.0f)
	{
		retVal = new PauseScreen(this); INCR_NEW("Menu");
	}
    else if (Global::finishStageTimer >= 9.05f)
    {
        if (!Global::gameIsArcadeMode)
		{
            retVal = ClearStack::get();
        }
    }

    if (Global::raceStartTimer <= 0.0f)
    {
        this->timer->increment();
    }

	this->draw();
	return retVal;
}
