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
	this->safeAreaY = 0.05f;
	this->safeAreaX = safeAreaY / this->aspectRatio;

	this->s = 0.05f;
	this->w = 0.8f * s / this->aspectRatio; //width of a single text character

	//this->fontVip = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt"); INCR_NEW
	this->numberLives = new GUINumber(Global::gameLives, safeAreaX, 1.0f - safeAreaY, s, 6, true, 2); INCR_NEW

	this->timer = new Timer(Global::fontVipnagorgialla, safeAreaX, safeAreaY, s, 0, true); INCR_NEW
    Global::mainHudTimer = this->timer;

	this->speedometerScale = 1.5f;

	this->numberSpeed = new GUINumber(Global::gameMainVehicleSpeed, 1.0f - safeAreaX - (4 * w), 1.0f - safeAreaY, speedometerScale * s, 8, true, 3); INCR_NEW
	this->textSpeedUnits = new GUIText("km/h", s, Global::fontVipnagorgialla, 1 - safeAreaX, 1.0f - safeAreaY, 8, true); INCR_NEW
}

HUD::~HUD()
{
	//delete this->fontVip; INCR_DEL
	delete this->timer; INCR_DEL
	this->numberLives->deleteMe(); delete this->numberLives; INCR_DEL
	this->numberSpeed->deleteMe(); delete this->numberSpeed; INCR_DEL
	this->textSpeedUnits->deleteMe(); delete this->textSpeedUnits; INCR_DEL
}

Timer* HUD::getTimer()
{
	return this->timer;
}

void HUD::draw()
{
	this->numberLives->displayNumber = Global::gameLives;
	this->numberLives->refresh();
	this->numberSpeed->displayNumber = Global::gameMainVehicleSpeed;
	this->numberSpeed->refresh();

	this->timer->increment();
	this->timer->refresh();
}

Menu* HUD::step()
{
	Menu* retVal = nullptr;

	if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START &&
        Global::finishStageTimer < 0.0f)
	{
		retVal = new PauseScreen(this); INCR_NEW
	}
    else if (Global::finishStageTimer >= 9.05f)
    {
        if (!Global::gameIsArcadeMode)
		{
            std::fprintf(stdout, "asdfafsf\n");
            retVal = ClearStack::get();
        }
    }

	this->draw();
	return retVal;
}
