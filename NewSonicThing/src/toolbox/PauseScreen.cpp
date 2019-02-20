#include "pausescreen.h"
#include "../engineTester/main.h"

#include <cmath>
#include <algorithm>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
#include "../fontMeshCreator/guitext.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particletexture.h"
#include "../entities/camera.h"
#include "../particles/particlemaster.h"
#include "../guis/guitextureresources.h"
#include "../guis/guimanager.h"
#include "hud.h"
#include "timer.h"

PauseScreen::PauseScreen(HUD* gameHud)
{
	this->gameHud = gameHud;

	this->size = 0.075f;

	printf("Pause Screen initializing...\n");
	this->font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"), "res/Fonts/vipnagorgialla.fnt");

	Global::gameState = STATE_PAUSED;
	this->menuSelection = 0;
	this->menuSelectionMAX = 3;

	this->textResume = new GUIText("Resume", size, font, 0.5f, 0.35f, 4, true);
	this->textRestart = new GUIText("Restart", size, font, 0.5f, 0.45f, 4, true);

	if (Global::isAutoCam)
	{
		this->textCamera = new GUIText("Auto Cam", size, font, 0.5f, 0.55f, 4, true);
	}
	else
	{
		this->textCamera = new GUIText("Free Cam", size, font, 0.5f, 0.55f, 4, true);
	}

	this->textQuit = new GUIText("Quit", size, font, 0.5f, 0.65f, 4, true);

	printf("Pause Screen initialized!\n");
}

PauseScreen::~PauseScreen()
{
	textResume->deleteMe(); delete textResume;

	textRestart->deleteMe(); delete textRestart;

	textCamera->deleteMe(); delete textCamera;

	textQuit->deleteMe(); delete textQuit;

	Global::gameState = STATE_RUNNING;

	AudioPlayer::stopAllSFX();
}

void PauseScreen::setVisible(bool visible)
{
	this->textResume->setVisibility(visible);
	this->textRestart->setVisibility(visible);
	this->textCamera->setVisibility(visible);
	this->textQuit->setVisibility(visible);
}

void PauseScreen::selectButton()
{
	GuiManager::clearGuisToRender();

	this->textResume->setColour(1.0f, 1.0f, 1.0f);
	this->textRestart->setColour(1.0f, 1.0f, 1.0f);
	this->textCamera->setColour(1.0f, 1.0f, 1.0f);
	this->textQuit->setColour(1.0f, 1.0f, 1.0f);

	switch (menuSelection)
	{
	case 0:
		this->textResume->setColour(0.7f, 0.7f, 0.7f);
		break;
	case 1:
		this->textRestart->setColour(0.7f, 0.7f, 0.7f);
		break;
	case 2:
		this->textCamera->setColour(0.7f, 0.7f, 0.7f);
		break;
	case 3:
		this->textQuit->setColour(0.7f, 0.7f, 0.7f);
		break;
	default:
		break;
	}
}

Menu* PauseScreen::step()
{
	Menu* retVal = nullptr;

	int moveY = Input::inputs.MENU_Y;
	bool pressedSelect = false;

	if (moveYPrevious != moveY)
	{
		menuSelection += moveY;
		menuSelection = std::max(0, std::min(menuSelectionMAX, menuSelection));
	}

	if (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1)
	{
		pressedSelect = true;
	}
		
	this->selectButton();

	if (pressedSelect)
	{
		switch (menuSelection)
		{
		case 0:
		{
			// Pop menu. Unpause.
			retVal = PopMenu::get();
			break;
		}

		case 1:
		{
			if (Global::gameLives > 0)
			{
				Global::shouldLoadLevel = true;
				this->gameHud->getTimer()->setTime(0.0f);
				Vector3f vel(0, 0, 0);
				new Particle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &vel, 0, 1.0f, 0.0f, 50.0f, 0, 1.0f, 0, true, false);
				Global::gameState = STATE_CUTSCENE;
				retVal = PopMenu::get();
			}
			break;
		}

		case 2:
		{
			//switch cam, reload text
			if (textCamera != nullptr)
			{
				textCamera->deleteMe(); delete textCamera; textCamera = nullptr;
				if (Global::isAutoCam)
				{
					textCamera = new GUIText("Free Cam", size, font, 0.5f, 0.55f, 4, true);
						Global::isAutoCam = false;
				}
				else
				{
					textCamera = new GUIText("Auto Cam", size, font, 0.5f, 0.55f, 4, true);
						Global::isAutoCam = true;
				}
			}
			break;
		}

		case 3:
		{
			Vector3f vel(0, 0, 0);
			new Particle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &vel, 0, 1.0f, 0.0f, 50.0f, 0, 1.0f, 0, true, false);
			LevelLoader::loadTitle();
			retVal = ClearStack::get();
			break;
		}

		default:
			break;
		}
	}

	moveYPrevious = moveY;
	return retVal;
}
