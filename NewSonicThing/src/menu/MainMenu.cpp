#include "mainmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <algorithm>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
#include "button.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particletexture.h"
#include "../entities/camera.h"
#include "../particles/particlemaster.h"
#include "../guis/guitextureresources.h"
#include "../guis/guimanager.h"
#include "../guis/guitexture.h"
#include "missionmenu.h"
#include <iostream>

MainMenu::MainMenu()
{
	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;

	this->menuButtons = nullptr;
	this->aspectRatio = (float) SCR_WIDTH / SCR_HEIGHT;
	this->buttonCount = 0;
	this->menuSelectionID = 0;
	this->textureParallelogram = GL_NONE;
	this->textureParallelogramBackdrop = GL_NONE;
	this->textureLogo = GL_NONE;
	MainMenu::init();
	this->setVisible(true);
}

MainMenu::~MainMenu()
{
	MainMenu::unloadResources();
}

void MainMenu::init()
{
	printf("Main Menu initializing...\n");

	//font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"),
	//	"res/Fonts/vipnagorgialla.fnt"); INCR_NEW

	this->textureParallelogram = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/Parallelogram.png");
	this->textureParallelogramBackdrop = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramBackdrop.png");
	this->textureLogo = Loader::loadTexture("res/Images/MainMenu/Logo.png");
	this->logo = GuiTexture(textureLogo, 0.5f, 0.4f, 0.6f * (767.0f / 784.0f) / this->aspectRatio, 0.6f, 0.0f);

	MainMenu::loadResources();

	printf("Main Menu initialized!\n");
}

void MainMenu::loadResources()
{
	printf("Loading resources\n");
	if (MainMenu::menuButtons != nullptr)
	{
		std::fprintf(stdout, "Main Menu trying to load resources when they are already loaded!\n");
		return;
	}

	float fontScale = 0.08f;

	// Adding a string here *should* add a button.
	std::vector<std::string> buttonNames(
		{
		"MISSION",
		"CONFIG",
		"EXIT"
		}
	);

	int numButtonsToMake = (int) buttonNames.size();

	this->menuButtons = new Button*[numButtonsToMake]; INCR_NEW

	for (int i = 0; i < numButtonsToMake; i++)
	{
		this->menuButtons[i] = new Button(buttonNames[i], Global::fontVipnagorgialla, this->textureParallelogram, this->textureParallelogramBackdrop,
			0.5f + (0.5f * (i - this->menuSelectionID) / this->aspectRatio), 0.8f, (fontScale * 8.0f / this->aspectRatio), fontScale, true); INCR_NEW
	}

	this->buttonCount = numButtonsToMake;

	printf("Resources loaded\n");
}

void MainMenu::draw()
{
	extern float dt;

	if (oldSelection != this->menuSelectionID)
	{
		animationTime = 0.25f;
		animating = true;
		if (oldSelection < this->menuSelectionID)
		{
			animationDirection = 1;
		}
		else
		{
			animationDirection = -1;
		}
	}

	if (animating)
	{
		animationTime -= dt;
	}

	if (animationTime < 0.0f)
	{
		animationTime = 0.00f;
		animating = false;
	}

	GuiManager::clearGuisToRender();

	GuiManager::addGuiToRender(&this->logo);

	if (this->visible)
	{
		float animationOffset = animationDirection * 12 * (animationTime) * (animationTime);

		for (int i = 0; i < this->buttonCount; i++)
		{
			this->menuButtons[i]->setPos(0.5f + (0.75f * (i - this->menuSelectionID) / this->aspectRatio) + animationOffset / this->aspectRatio, 0.8f);
			this->menuButtons[i]->setVisible(true);
			this->menuButtons[i]->setHighlight(false);
		}
	}

	this->menuButtons[menuSelectionID]->setHighlight(true);
	oldSelection = this->menuSelectionID;
}

void MainMenu::setVisible(bool visibleStatus)
{
	for (int i = 0; i < this->buttonCount; i++)
	{
		this->menuButtons[i]->setVisible(visibleStatus);
	}
	this->visible = visibleStatus;
}

void MainMenu::unloadResources()
{
	std::cout << "Unloading resources...\n";
	if (MainMenu::menuButtons == nullptr)
	{
		std::fprintf(stdout, "Main Menu trying to unload resources when they are already unloaded!\n");
		return;
	}

	int numToDelete = MainMenu::buttonCount;

	for (int i = 0; i < numToDelete; i++)
	{
		delete MainMenu::menuButtons[i]; INCR_DEL
	}

	delete[] MainMenu::menuButtons; MainMenu::menuButtons = nullptr; INCR_DEL

	GuiManager::clearGuisToRender();
	std::cout << "Resources unloaded!\n";
}

void MainMenu::resetAnimation()
{
	this->animating = false;
	this->animationTime = 0;
}

Menu* MainMenu::step()
{
	Menu* retVal = nullptr;

	this->setVisible(true);

	//bool shouldGoUp    = false;
	//bool shouldGoDown  = false;
	//bool shouldGoLeft  = false;
	//bool shouldGoRight = false;
	bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
	//bool pressedBack   = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);

	static int xPrev;
	//static int yPrev;

	int moveX = (int)round(Input::inputs.INPUT_X);
	//int moveY = (int)round(Input::inputs.INPUT_Y);

	if (moveX == -1 && moveX != xPrev && menuSelectionID > 0)
	{
		xPrev = -1;
		menuSelectionID--;
		AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
	}
	else if (moveX == 1 && moveX != xPrev && menuSelectionID < (this->buttonCount - 1))
	{
		xPrev = 1;
		menuSelectionID++;
		AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
	}
		
	if (moveX == 0)
	{
		xPrev = 0;
	}

	if (pressedSelect)
	{
		this->resetAnimation();
		switch (this->menuSelectionID)
		{
			case mission:
				AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
				retVal = new MissionMenu; INCR_NEW
				this->setVisible(false);
				break;

			case config:
				break;

			case exit:
				Global::gameState = STATE_EXITING;
				break;
		}
	}
	
	this->draw();

	return retVal;
}
