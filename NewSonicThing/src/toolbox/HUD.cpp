#include "hud.h"

#include "input.h"
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

	this->fontVip = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt");
	this->numberLives = new GUINumber(Global::gameLives, safeAreaX, 1.0f - safeAreaY, s, 6, true, 2);

	this->timer = new Timer(fontVip, safeAreaX, safeAreaY, s, 0, true);

	this->speedometerScale = 1.5f;

	this->numberSpeed = new GUINumber(Global::gameMainVehicleSpeed, 1.0f - safeAreaX - (4 * w), 1.0f - safeAreaY, speedometerScale * s, 8, true, 3);
	this->textSpeedUnits = new GUIText("km/h", s, fontVip, 1 - safeAreaX, 1.0f - safeAreaY, 8, true);
}

HUD::~HUD()
{
	delete this->fontVip;
	delete this->timer;
	this->numberLives->deleteMe(); delete this->numberLives;
	this->numberSpeed->deleteMe(); delete this->numberSpeed;
	this->textSpeedUnits->deleteMe(); delete this->textSpeedUnits;
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

	if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START)
	{
		retVal = new PauseScreen(this);
	}

	this->draw();
	return retVal;
}