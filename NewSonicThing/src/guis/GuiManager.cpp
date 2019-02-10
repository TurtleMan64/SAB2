#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "guimanager.h"
#include "../engineTester/main.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/fonttype.h"
#include "../toolbox/pausescreen.h"
#include "guirenderer.h"
#include "../toolbox/input.h"

#include <cmath>
#include <string>

//extern InputStruct Inputs;

//GUIText* GuiManager::textTimer = nullptr;

GUIText* GuiManager::timerColon  = nullptr;
GUIText* GuiManager::timerPeriod = nullptr;
GUIText* GuiManager::timerMin1[10];
GUIText* GuiManager::timerMin2[10];
GUIText* GuiManager::timerSec1[10];
GUIText* GuiManager::timerSec2[10];
GUIText* GuiManager::timerCen1[10];
GUIText* GuiManager::timerCen2[10];

bool   GuiManager::timerIsRunning = false;
float  GuiManager::timerValue     = 0;
double GuiManager::timestampStart = 0;
double GuiManager::timestampEnd   = 0;

GUIText* GuiManager::textLives = nullptr;

GUINumber*  GuiManager::numberSpeed    = nullptr;
GUIText*    GuiManager::textSpeedUnits = nullptr;

int GuiManager::previousLives = -1;

GUIText* GuiManager::textHorVel = nullptr;
GUIText* GuiManager::textVerVel = nullptr;
GUIText* GuiManager::textTotalVel = nullptr;

GUIText* GuiManager::textA = nullptr;
GUIText* GuiManager::textB = nullptr;
GUIText* GuiManager::textX = nullptr;
GUIText* GuiManager::textY = nullptr;

FontType* GuiManager::fontVip = nullptr;

float GuiManager::horVel = 0;
float GuiManager::verVel = 0;
float GuiManager::totalVel = 0;

std::list<GuiTexture*> GuiManager::guisToRender;

void GuiManager::init()
{
	fontVip = PauseScreen::font;
	textLives = new GUIText("0", 1, fontVip, 0.01f, 0.98f, 1, false, false, false); INCR_NEW

	//Player debug text
	textHorVel              = new GUIText("Hor Vel:"     + std::to_string(horVel),              1, fontVip, 0.01f, 0.70f, 1, false, false, Global::debugDisplay); INCR_NEW
	textVerVel              = new GUIText("Ver Vel:"     + std::to_string(verVel),              1, fontVip, 0.01f, 0.75f, 1, false, false, Global::debugDisplay); INCR_NEW
	textTotalVel            = new GUIText("Total Vel:"   + std::to_string(horVel),              1, fontVip, 0.01f, 0.80f, 1, false, false, Global::debugDisplay); INCR_NEW

	//Input text
	textA = new GUIText("A", 1, fontVip, 0.80f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
	textB = new GUIText("B", 1, fontVip, 0.85f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
	textX = new GUIText("X", 1, fontVip, 0.90f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
	textY = new GUIText("Y", 1, fontVip, 0.95f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW

	const float w = 0.02f; //width of a single text character
	const float s = 1.5f; //size of timer text

	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;

	float px = 1.0f/(SCR_WIDTH);  //1 pixel in x dimension
	float py = 1.0f/(SCR_HEIGHT); //1 pixel in y dimension

	GuiManager::timerColon   = new GUIText(":", s, fontVip, 2*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerPeriod  = new GUIText(".", s, fontVip, 5*w+16*px, 16*py, w, true, false, true); INCR_NEW

	GuiManager::timerMin1[0] = new GUIText("0", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[1] = new GUIText("1", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[2] = new GUIText("2", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[3] = new GUIText("3", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[4] = new GUIText("4", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[5] = new GUIText("5", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[6] = new GUIText("6", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[7] = new GUIText("7", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[8] = new GUIText("8", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin1[9] = new GUIText("9", s, fontVip, 0*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[0] = new GUIText("0", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[1] = new GUIText("1", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[2] = new GUIText("2", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[3] = new GUIText("3", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[4] = new GUIText("4", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[5] = new GUIText("5", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[6] = new GUIText("6", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[7] = new GUIText("7", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[8] = new GUIText("8", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerMin2[9] = new GUIText("9", s, fontVip, 1*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[0] = new GUIText("0", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[1] = new GUIText("1", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[2] = new GUIText("2", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[3] = new GUIText("3", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[4] = new GUIText("4", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[5] = new GUIText("5", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[6] = new GUIText("6", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[7] = new GUIText("7", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[8] = new GUIText("8", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec1[9] = new GUIText("9", s, fontVip, 3*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[0] = new GUIText("0", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[1] = new GUIText("1", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[2] = new GUIText("2", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[3] = new GUIText("3", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[4] = new GUIText("4", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[5] = new GUIText("5", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[6] = new GUIText("6", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[7] = new GUIText("7", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[8] = new GUIText("8", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerSec2[9] = new GUIText("9", s, fontVip, 4*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[0] = new GUIText("0", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[1] = new GUIText("1", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[2] = new GUIText("2", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[3] = new GUIText("3", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[4] = new GUIText("4", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[5] = new GUIText("5", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[6] = new GUIText("6", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[7] = new GUIText("7", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[8] = new GUIText("8", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen1[9] = new GUIText("9", s, fontVip, 6*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[0] = new GUIText("0", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[1] = new GUIText("1", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[2] = new GUIText("2", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[3] = new GUIText("3", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[4] = new GUIText("4", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[5] = new GUIText("5", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[6] = new GUIText("6", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[7] = new GUIText("7", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[8] = new GUIText("8", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW
	GuiManager::timerCen2[9] = new GUIText("9", s, fontVip, 7*w+16*px, 16*py, w, true, false, true); INCR_NEW

	//GuiManager::numberSpeed    = new GUINumber(0, 0.87f, 0.9f, 0, true); INCR_NEW
	GuiManager::numberSpeed    = new GUINumber(0, 0.87f, 0.9f, 0.06f, 2, true, 4); INCR_NEW
	//GuiManager::textSpeedUnits = new GUIText("km/h", 1.5f, fontVip, 0.88f, 0.918f, 1.0f, false, false, true); INCR_NEW
	GuiManager::textSpeedUnits = new GUIText("km/h", 0.045f, fontVip, 0.88f, 0.918f, 0, true); INCR_NEW

	GuiManager::setTimerInvisible();

	GuiRenderer::init();
}

void GuiManager::refresh()
{
	//double start = glfwGetTime();

	//GuiManager::timestampEnd = glfwGetTime();

	//std::fprintf(stdout, "timer chunks = %f   timer stamp = %f\n", timerValue, (float)(GuiManager::timestampEnd-GuiManager::timestampStart));

	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;

	float px = 1.0f/(SCR_WIDTH);  //1 pixel in x dimension
	float py = 1.0f/(SCR_HEIGHT); //1 pixel in y dimension

	//const float w = 0.02f;   //width of a single text character
	const float o = 0.0008f; //horizontal offset to adjust for centered vs non centered
	//const float s = 1.5f;    //size of timer text

	if (Global::gameLives != GuiManager::previousLives)
	{
		textLives->deleteMe();
		delete textLives; INCR_DEL
		textLives = new GUIText(std::to_string(Global::gameLives), 1.5f, fontVip, o+16*px, 1.0f-80*py, 1, false, false, true); INCR_NEW
		GuiManager::previousLives = Global::gameLives;
	}

	if (Global::debugDisplay)
	{
		//Check if player exists before trying to get values.
		if (Global::gameMainVehicle != nullptr)
		{
			//float xVel = Global::gamePlayer->getxVel() + Global::gamePlayer->getXVelAir();
			//float zVel = Global::gamePlayer->getzVel() + Global::gamePlayer->getZVelAir();

			//horVel = sqrtf(xVel * xVel + zVel * zVel);

			//verVel = Global::gamePlayer->getyVel();

			//totalVel = sqrtf(xVel * xVel + verVel * verVel + zVel * zVel);

			//hoverCount = Global::gamePlayer->getHoverCount();

			//storedSpindashSpeed = Global::gamePlayer->getSpindashSpeed();
		}

		//Display debug text is debugDisplay is true.
		textHorVel->deleteMe(); delete textHorVel; INCR_DEL textHorVel = nullptr;
		textHorVel = new GUIText("Hor Vel:" + std::to_string(horVel), 1.0f, fontVip, 0.01f, 0.70f, 1, false, false, Global::debugDisplay); INCR_NEW

		textVerVel->deleteMe(); delete textVerVel; INCR_DEL textVerVel = nullptr;
		textVerVel = new GUIText("Ver Vel:" + std::to_string(verVel), 1.0f, fontVip, 0.01f, 0.75f, 1, false, false, Global::debugDisplay); INCR_NEW

		textTotalVel->deleteMe(); delete textTotalVel; INCR_DEL textTotalVel = nullptr;
		textTotalVel = new GUIText("Total Vel:" + std::to_string(totalVel), 1.0f, fontVip, 0.01f, 0.80f, 1, false, false, Global::debugDisplay); INCR_NEW

		//Input display

		textA->deleteMe(); delete textA; INCR_DEL textA = nullptr;
		textA = new GUIText("A", 1, fontVip, 0.80f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
		if (!Input::inputs.INPUT_ACTION1)
		{
			textA->setColour(0.2f, 0.2f, 0.2f);
		}

		textB->deleteMe(); delete textB; INCR_DEL textB = nullptr;
		textB = new GUIText("B", 1, fontVip, 0.85f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
		if (!Input::inputs.INPUT_ACTION2)
		{
			textB->setColour(0.2f, 0.2f, 0.2f);
		}

		textX->deleteMe(); delete textX; INCR_DEL textX = nullptr;
		textX = new GUIText("X", 1, fontVip, 0.90f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
		if (!Input::inputs.INPUT_ACTION3)
		{
			textX->setColour(0.2f, 0.2f, 0.2f);
		}

		textY->deleteMe(); delete textY; INCR_DEL textY = nullptr;
		textY = new GUIText("Y", 1, fontVip, 0.95f, 0.95f, 1, false, false, Global::debugDisplay); INCR_NEW
		if (!Input::inputs.INPUT_ACTION4)
		{
			textY->setColour(0.2f, 0.2f, 0.2f);
		}
	}
	else
	{
		textHorVel->setVisibility(false);
		textVerVel->setVisibility(false);
		textTotalVel->setVisibility(false);

		textA->setVisibility(false);
		textB->setVisibility(false);
		textX->setVisibility(false);
		textY->setVisibility(false);
	}

	if (Global::gameState != STATE_TITLE)
	{
		textLives->setVisibility(false);

		if (Global::gameIsArcadeMode)
		{
			textLives->setVisibility(true);
		}

		GuiManager::setTimerInvisible();

		int centi = (int)(100*fmodf(timerValue, 1.0f));
		int secon = (int)(fmodf(timerValue, 60.0f));
		int minut = (int)(timerValue/60.0f);

		timerColon ->setVisibility(true);
		timerPeriod->setVisibility(true);
		timerMin1[minut/10]->setVisibility(true);
		timerMin2[minut%10]->setVisibility(true);
		timerSec1[secon/10]->setVisibility(true);
		timerSec2[secon%10]->setVisibility(true);
		timerCen1[centi/10]->setVisibility(true);
		timerCen2[centi%10]->setVisibility(true);

		GuiManager::numberSpeed->displayNumber = Global::gameMainVehicleSpeed;
		GuiManager::numberSpeed->refresh();
		GuiManager::numberSpeed->visible = true;
		GuiManager::textSpeedUnits->setVisibility(true);
	}
	else
	{
		textLives->setVisibility(false);

		GuiManager::setTimerInvisible();

		GuiManager::numberSpeed->visible = false;
		GuiManager::textSpeedUnits->setVisibility(false);
	}

	//double end = glfwGetTime();
	//std::fprintf(stdout, "time diff = %.12f\n", end-start);

	//Render images
	GuiRenderer::render(&GuiManager::guisToRender);
}

void GuiManager::setTimerInvisible()
{
	GuiManager::timerColon  ->setVisibility(false);
	GuiManager::timerPeriod ->setVisibility(false);
	GuiManager::timerMin1[0]->setVisibility(false);
	GuiManager::timerMin1[1]->setVisibility(false);
	GuiManager::timerMin1[2]->setVisibility(false);
	GuiManager::timerMin1[3]->setVisibility(false);
	GuiManager::timerMin1[4]->setVisibility(false);
	GuiManager::timerMin1[5]->setVisibility(false);
	GuiManager::timerMin1[6]->setVisibility(false);
	GuiManager::timerMin1[7]->setVisibility(false);
	GuiManager::timerMin1[8]->setVisibility(false);
	GuiManager::timerMin1[9]->setVisibility(false);
	GuiManager::timerMin2[0]->setVisibility(false);
	GuiManager::timerMin2[1]->setVisibility(false);
	GuiManager::timerMin2[2]->setVisibility(false);
	GuiManager::timerMin2[3]->setVisibility(false);
	GuiManager::timerMin2[4]->setVisibility(false);
	GuiManager::timerMin2[5]->setVisibility(false);
	GuiManager::timerMin2[6]->setVisibility(false);
	GuiManager::timerMin2[7]->setVisibility(false);
	GuiManager::timerMin2[8]->setVisibility(false);
	GuiManager::timerMin2[9]->setVisibility(false);
	GuiManager::timerSec1[0]->setVisibility(false);
	GuiManager::timerSec1[1]->setVisibility(false);
	GuiManager::timerSec1[2]->setVisibility(false);
	GuiManager::timerSec1[3]->setVisibility(false);
	GuiManager::timerSec1[4]->setVisibility(false);
	GuiManager::timerSec1[5]->setVisibility(false);
	GuiManager::timerSec1[6]->setVisibility(false);
	GuiManager::timerSec1[7]->setVisibility(false);
	GuiManager::timerSec1[8]->setVisibility(false);
	GuiManager::timerSec1[9]->setVisibility(false);
	GuiManager::timerSec2[0]->setVisibility(false);
	GuiManager::timerSec2[1]->setVisibility(false);
	GuiManager::timerSec2[2]->setVisibility(false);
	GuiManager::timerSec2[3]->setVisibility(false);
	GuiManager::timerSec2[4]->setVisibility(false);
	GuiManager::timerSec2[5]->setVisibility(false);
	GuiManager::timerSec2[6]->setVisibility(false);
	GuiManager::timerSec2[7]->setVisibility(false);
	GuiManager::timerSec2[8]->setVisibility(false);
	GuiManager::timerSec2[9]->setVisibility(false);
	GuiManager::timerCen1[0]->setVisibility(false);
	GuiManager::timerCen1[1]->setVisibility(false);
	GuiManager::timerCen1[2]->setVisibility(false);
	GuiManager::timerCen1[3]->setVisibility(false);
	GuiManager::timerCen1[4]->setVisibility(false);
	GuiManager::timerCen1[5]->setVisibility(false);
	GuiManager::timerCen1[6]->setVisibility(false);
	GuiManager::timerCen1[7]->setVisibility(false);
	GuiManager::timerCen1[8]->setVisibility(false);
	GuiManager::timerCen1[9]->setVisibility(false);
	GuiManager::timerCen2[0]->setVisibility(false);
	GuiManager::timerCen2[1]->setVisibility(false);
	GuiManager::timerCen2[2]->setVisibility(false);
	GuiManager::timerCen2[3]->setVisibility(false);
	GuiManager::timerCen2[4]->setVisibility(false);
	GuiManager::timerCen2[5]->setVisibility(false);
	GuiManager::timerCen2[6]->setVisibility(false);
	GuiManager::timerCen2[7]->setVisibility(false);
	GuiManager::timerCen2[8]->setVisibility(false);
	GuiManager::timerCen2[9]->setVisibility(false);
}

void GuiManager::increaseTimer(float deltaTime)
{
	if (timerIsRunning)
	{
		timerValue += deltaTime;
	}
}

void GuiManager::setTimer(float newVal)
{
	timerValue = newVal;
}

void GuiManager::startTimer()
{
	timerIsRunning = true;
	//GuiManager::timestampStart = glfwGetTime();
}

void GuiManager::stopTimer()
{
	timerIsRunning = false;
}

float GuiManager::getTotalTimerInSeconds()
{
	return timerValue;
}

void GuiManager::addGuiToRender(GuiTexture* newImage)
{
	GuiManager::guisToRender.push_back(newImage);
}

void GuiManager::removeGui(GuiTexture* imageToRemove)
{
	GuiManager::guisToRender.remove(imageToRemove);
}

void GuiManager::clearGuisToRender()
{
	GuiManager::guisToRender.clear();
}
