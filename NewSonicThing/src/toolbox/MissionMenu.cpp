#include "missionmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <iostream>
#include <algorithm>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
#include "../fontMeshCreator/guitext.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../entities/camera.h"
#include "../guis/guitextureresources.h"
#include "../guis/guimanager.h"
#include "../guis/guitexture.h"
#include "button.h"

MissionMenu::MissionMenu()
{
	std::cout << "Initializing Mission Menu\n";
	index = 0;
	this->init();
	this->loadResources();
	std::cout << "Mission Menu initialized\n";
}

void MissionMenu::init()
{
	fontSize = 0.05f;
	textureParallelogram = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/Parallelogram.png");
	textureParallelogramBackdrop = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramBackdrop.png");
	textureParallelogramHalf = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramHalf.png");
	textureParallelogramHalfBackdrop = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramHalfBackdrop.png");
	textureParallelogramHalf2 = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramHalf2.png");
	textureParallelogramHalf2Backdrop = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/ParallelogramHalf2Backdrop.png");
	textureRankA = Loader::loadTexture("res/Images/MainMenu/RankA.png");
	textureRankB = Loader::loadTexture("res/Images/MainMenu/RankB.png");
	textureRankC = Loader::loadTexture("res/Images/MainMenu/RankC.png");
	textureRankD = Loader::loadTexture("res/Images/MainMenu/RankD.png");
	textureRankE = Loader::loadTexture("res/Images/MainMenu/RankE.png");
	textureRankBlank = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/NoRank.png");
	textureMissionSelect = Loader::loadTextureNoInterpolation(
		"res/Images/MainMenu/MissionSelect.png");
	textureNPCicon = Loader::loadTexture(
		"res/Images/MainMenu/NPCicon.png");

	font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"),
		"res/Fonts/vipnagorgialla.fnt"); INCR_NEW
}

MissionMenu::~MissionMenu()
{
	std::cout << "Deleting Mission Menu\n";
	this->unloadResources();
	std::cout << "Mission Menu deleted.\n";
}

void MissionMenu::loadResources()
{
	levelButton = new Button*[Global::gameLevelData.size()];
	counter = 0;
	for (Level level : Global::gameLevelData)
	{
		std::string buttonText = "";
		buttonText = level.displayName;
		levelButton[counter] = new Button(level.displayName, font, textureParallelogram, textureParallelogramBackdrop, 0.5f, 0.5f + (0.25f * counter), 0.56f / GuiManager::getAspectRatio(), 0.07f, true);
		counter++;
	}
}

void MissionMenu::unloadResources()
{
	std::cout << "Unloading Mission Menu resources.\n";
	GuiManager::clearGuisToRender();
	for (int i = 0; i < counter; i++)
	{
		delete levelButton[i];
	}
	delete levelButton;
	std::cout << "Mission Menu resources deleted.\n";
}

void MissionMenu::selectButton()
{
	GuiManager::clearGuisToRender();
	
	for (int i = 0; i < this->counter; i++)
	{
		this->levelButton[i]->setPos(0.5f, 0.5f + (0.25f * (i - index)));
		this->levelButton[i]->setVisible(true);
	}
}

int MissionMenu::step()
{
	bool shouldGoUp = false;
	bool shouldGoDown = false;
	bool shouldGoLeft = false;
	bool shouldGoRight = false;
	bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
	bool pressedBack = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);

	int moveX = (int)round(Input::inputs.INPUT_X);
	int moveY = (int)round(Input::inputs.INPUT_Y);

	if (moveX != moveXPrevious)
	{
		if (moveX == 1)
		{
			shouldGoRight = true;
		}
		else if (moveX == -1)
		{
			shouldGoLeft = true;
		}
	}

	if (moveY != moveYPrevious)
	{
		if (moveY == -1)
		{
			shouldGoUp = true;
		}
		else if (moveY == 1)
		{
			shouldGoDown = true;
		}
	}

	moveXPrevious = moveX;
	moveYPrevious = moveY;

	if (moveY == -1)
	{
		holdUpTimer++;
	}
	else
	{
		holdUpTimer = 0;
	}

	if (moveY == 1)
	{
		holdDownTimer++;
	}
	else
	{
		holdDownTimer = 0;
	}

	if (holdUpTimer >= 20)
	{
		if (holdUpTimer % 5 == 0)
		{
			shouldGoUp = true;
		}
	}

	if (holdDownTimer >= 20)
	{
		if (holdDownTimer % 5 == 0)
		{
			shouldGoDown = true;
		}
	}


	if (shouldGoUp)
	{
		if (index > 0)
		{
			index = index - 1;
		}
	}
	if (shouldGoDown)
	{
		if (index < (this->counter - 1))
		{
			index = index + 1;
		}
	}

	if (pressedSelect)
	{
		AudioPlayer::play(38, Global::gameCamera->getFadePosition1());

		Level* currentLevel = &Global::gameLevelData[index];
		Global::levelName = currentLevel->fileName;
		Global::levelNameDisplay = currentLevel->displayName;
		Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];

		Global::isNewLevel = true;
		Global::shouldLoadLevel = true;
		Global::gameState = STATE_RUNNING;
		return 1;
	}
	else if (pressedBack)
	{
		return 1;
	}

	this->selectButton();
	return 0;
}