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

enum ButtonID { story, extras, quit };

MainMenu::MainMenu()
{
	this->menuButtons = nullptr;
	this->font = nullptr;
	this->aspectRatio = GuiManager::getAspectRatio();
	this->buttonCount = 0;
	this->menuSelectionID = 0;
	this->textureParallelogram = GL_NONE;
	this->textureParallelogramBackdrop = GL_NONE;
	MainMenu::init();
}

MainMenu::~MainMenu()
{
	MainMenu::unloadResources();
}

void MainMenu::init()
{
	printf("Main Menu initializing...\n");

	MainMenu::aspectRatio = GuiManager::getAspectRatio();

	font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"),
		"res/Fonts/vipnagorgialla.fnt"); INCR_NEW

	this->textureParallelogram = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/Parallelogram.png");
	this->textureParallelogramBackdrop = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramBackdrop.png");

	MainMenu::loadResources();

	MainMenu::selectMenuRoot(ButtonID::story);

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

	float fontScale = 0.06f;

	// Adding a string here *should* add a button.
	std::vector<std::string> buttonNames(
		{
		"ARCADE",
		"EXTRAS",
		"EXIT"
		}
	);

	int numButtonsToMake = (int) buttonNames.size();
	int numRows = (numButtonsToMake + 2 - 1) / 2;
	std::cout << "NumRows: " << numRows << std::endl;
	float buttonYSpacing = 1.0f / ((float) (numRows + 1)); // Used to center menu vertically

	MainMenu::menuButtons = new Button*[numButtonsToMake];

	// Used to decide how to lay out the buttons
	if (numButtonsToMake % 2 == 0)
	{
		// Two columns
		for (int i = 0; i < numButtonsToMake; i++)
		{
			std::cout << "Initializing button " << buttonNames[i] << std::endl;
			// Left column
			if (i % 2 == 0)
			{
				MainMenu::menuButtons[i] = new Button(buttonNames[i], 
					MainMenu::font, MainMenu::textureParallelogram, MainMenu::textureParallelogramBackdrop,
					0.5f - (0.25f / MainMenu::aspectRatio), 1.0f - (buttonYSpacing * (numRows - (i / 2))), 
					fontScale * 8.0f / MainMenu::aspectRatio, fontScale, true);
			}
			// Right column
			else
			{
				MainMenu::menuButtons[i] = new Button(buttonNames[i], 
					MainMenu::font, MainMenu::textureParallelogram, MainMenu::textureParallelogramBackdrop,
					0.5f + (0.25f / MainMenu::aspectRatio), 1.0f - (buttonYSpacing * (numRows - (i / 2))), 
					fontScale * 8.0f / MainMenu::aspectRatio, fontScale, true);
			}
		}
	}
	else
	{
		// Two columns
		for (int i = 0; i < numButtonsToMake - 1; i++)
		{
			std::cout << "Initializing button " << buttonNames[i] << std::endl;
			// Left column
			if (i % 2 == 0)
			{
				MainMenu::menuButtons[i] = new Button(buttonNames[i],
					MainMenu::font, MainMenu::textureParallelogram, MainMenu::textureParallelogramBackdrop,
					0.5f - (0.25f / MainMenu::aspectRatio), 1.0f - (buttonYSpacing * (numRows - (i / 2))),
					fontScale * 8.0f / MainMenu::aspectRatio, fontScale, true);
			}
			// Right column
			else
			{
				MainMenu::menuButtons[i] = new Button(buttonNames[i],
					MainMenu::font, MainMenu::textureParallelogram, MainMenu::textureParallelogramBackdrop,
					0.5f + (0.25f / MainMenu::aspectRatio), 1.0f - (buttonYSpacing * (numRows - (i / 2))),
					fontScale * 8.0f / MainMenu::aspectRatio, fontScale, true);
			}
		}
		// Final button at the bottom
		std::cout << "Initializing button " << buttonNames[numButtonsToMake - 1] << std::endl;
		MainMenu::menuButtons[numButtonsToMake - 1] = new Button(buttonNames[numButtonsToMake - 1], 
			MainMenu::font, MainMenu::textureParallelogram, MainMenu::textureParallelogramBackdrop,
			0.5f, 1.0f - buttonYSpacing, 
			fontScale * 8.0f / MainMenu::aspectRatio, fontScale, true);
	}
	std::cout << "Initialized all buttons!\n";
	MainMenu::buttonCount = numButtonsToMake;

	if (menuSelectionID <= ButtonID::story)
	{
		MainMenu::selectMenuRoot(menuSelectionID);
	}

	printf("Resources loaded\n");
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
		delete MainMenu::menuButtons[i];
	}

	delete[] MainMenu::menuButtons; MainMenu::menuButtons = nullptr;

	GuiManager::clearGuisToRender();
	std::cout << "Resources unloaded!\n";
}

void MainMenu::selectMenuRoot(int newSelection)
{
	MainMenu::menuButtons[MainMenu::menuSelectionID]->setHighlight(false);

	MainMenu::menuButtons[newSelection]->setHighlight(true);

	MainMenu::menuSelectionID = newSelection;

	AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
}

// Run defined button code
void MainMenu::buttonClick()
{
	switch (this->menuSelectionID)
	{
	case story:
	{
		Global::levelID = LVL_WINDY_VALLEY;
		//Global::gameMissionNumber = 0;
		Global::gameIsArcadeMode = true;
		Global::gameArcadePlaytime = 0;

		AudioPlayer::play(38, Global::gameCamera->getFadePosition1());

		Level* currentLevel = &Global::gameLevelData[Global::levelID];
		Global::levelName = currentLevel->fileName;
		Global::levelNameDisplay = currentLevel->displayName;
		Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];

		Global::isNewLevel = true;
		Global::shouldLoadLevel = true;
		MainMenu::unloadResources();
		Global::gameState = STATE_RUNNING;
		break;
	}
	case extras:
		break;
	case quit:
	{
		Global::gameState = STATE_EXITING;
		break;
	}
	default: break;
	}
}

int MainMenu::step()
{
	bool shouldGoUp    = false;
	bool shouldGoDown  = false;
	bool shouldGoLeft  = false;
	bool shouldGoRight = false;
	bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
	bool pressedBack   = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);

	int moveX = (int)round(Input::inputs.INPUT_X);
	int moveY = (int)round(Input::inputs.INPUT_Y);

	// Logic for figuring out what direction to move in the menu
	if (Global::gameState == STATE_TITLE)
	{
		if (menuSelectionID >= 0 && menuSelectionID < MainMenu::buttonCount)
		{
			if (shouldGoUp)
			{
				if (MainMenu::menuSelectionID - 2 >= 0)
				{
					MainMenu::selectMenuRoot(menuSelectionID - 2);
				}
			}
			else if (shouldGoDown)
			{
				if (MainMenu::menuSelectionID + 2 < MainMenu::buttonCount)
				{
					MainMenu::selectMenuRoot(menuSelectionID + 2);
				}
			}
			else if (shouldGoLeft)
			{
				if (MainMenu::menuSelectionID % 2 == 1)
				{
					MainMenu::selectMenuRoot(menuSelectionID - 1);
				}
			}
			else if (shouldGoRight)
			{
				if (MainMenu::menuSelectionID % 2 == 0 && MainMenu::menuSelectionID + 1 < MainMenu::buttonCount)
				{
					MainMenu::selectMenuRoot(menuSelectionID + 1);
				}
			}
			else if (pressedSelect)
			{
				buttonClick();
			}
		}
	}
	return 0;
}