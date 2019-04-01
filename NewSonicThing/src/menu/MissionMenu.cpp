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
#include "mainmenu.h"
#include "../toolbox/maths.h"

MissionMenu::MissionMenu()
{
	std::cout << "Initializing Mission Menu\n";
	this->init();
	this->loadResources();
	this->visible = true;
	this->offsetCurr = 0.0f;
	this->offsetTarget = 0.0f;
	std::cout << "Mission Menu initialized\n";
	index = 0;
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

	//font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"),
	//	"res/Fonts/vipnagorgialla.fnt"); INCR_NEW
}

MissionMenu::~MissionMenu()
{
	std::cout << "Deleting Mission Menu\n";
	this->unloadResources();
	std::cout << "Mission Menu deleted.\n";
}

void MissionMenu::loadResources()
{
    if (counter != 0)
    {
        std::fprintf(stdout, "Warning: MissionMenu loading resources when they are already loaded.\n");
    }

	extern unsigned int SCR_WIDTH;
	extern unsigned int SCR_HEIGHT;
	float aspectRatio = (float)SCR_WIDTH / SCR_HEIGHT;
	levelButton = new Button*[Global::gameLevelData.size()]; INCR_NEW("Button");
	counter = 0;
	for (Level level : Global::gameLevelData)
	{
		std::string buttonText = "";
		buttonText = level.displayName;
		levelButton[counter] = new Button(level.displayName, Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.5f, 0.5f + (0.2f * counter), 0.56f / aspectRatio, 0.07f, true); INCR_NEW("Button");
		counter++;
	}
}

void MissionMenu::unloadResources()
{
	std::cout << "Unloading Mission Menu resources.\n";
    if (counter == 0)
    {
        std::fprintf(stdout, "Warning: MissionMenu unloading resources when they are empty.\n");
    }

	GuiManager::clearGuisToRender();
	for (int i = 0; i < counter; i++)
	{
		delete levelButton[i]; INCR_DEL("Button");
	}
	delete[] levelButton; INCR_DEL("Button");
    counter = 0;
	std::cout << "Mission Menu resources deleted.\n";
}

void MissionMenu::draw()
{
	extern float dt;

	offsetTarget = -index*0.15f;
	offsetCurr = Maths::approach(offsetCurr, offsetTarget, 15.0f, dt);
	if (fabsf(offsetTarget-offsetCurr) < 0.002f)
	{
		offsetCurr = offsetTarget;
	}

	//if (oldSelection != this->index)
	{
		//animationTime = 0.0f;
		//animating = true;
		//if (oldSelection < this->index)
		{
			//animationDirection = 1;
		}
		//else
		{
			//animationDirection = -1;
		}
	}

	//if (animating)
	{
		//animationTime += dt;
	}

	//if (animationTime >= 0.25f)
	{
		//animationTime = 0.25f;
		//animating = false;
	}

	GuiManager::clearGuisToRender();
	
	if (this->visible)
	{
		//float animationOffset = animationDirection * 2.4f * (animationTime - 0.25f) * (animationTime - 0.25f);

		for (int i = 0; i < this->counter; i++)
		{
			//this->levelButton[i]->setPos(0.5f, 0.5f + animationOffset + (0.15f * (i - this->index))); offsetCurr
			this->levelButton[i]->setPos(0.5f, 0.5f + offsetCurr + 0.15f*i);
			this->levelButton[i]->setVisible(true);
			this->levelButton[i]->setHighlight(false);
		}
	}

	this->levelButton[index]->setHighlight(true);

	//oldSelection = this->index;
}

void MissionMenu::setVisible(bool visibleStatus)
{
	for (int i = 0; i < this->counter; i++)
	{
		this->levelButton[i]->setVisible(visibleStatus);
		this->levelButton[i]->setHighlight(visibleStatus);
	}
	this->visible = visibleStatus;
}

Menu* MissionMenu::step()
{
	Menu* retVal = nullptr;

	extern float dt;
	static float holdTick;

	bool shouldGoUp = false;
	bool shouldGoDown = false;
	bool pressedSelect = false;
	bool pressedBack = false;

	int moveY = (int)round(Input::inputs.INPUT_Y);

	this->setVisible(true);

	if ((Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || 
		(Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START))
	{
		pressedSelect = true;
	}

	if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2)
	{
		pressedBack = true;
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

	moveYPrevious = moveY;

	if (moveY == -1)
	{
		holdUpTimer += dt;
	}
	else
	{
		holdUpTimer = 0.0f;
	}

	if (moveY == 1)
	{
		holdDownTimer += dt;
	}
	else
	{
		holdDownTimer = 0.0f;
	}

	if (holdUpTimer >= 0.25f)
	{
		holdTick += dt;
		if (holdTick >= 0.15f)
		{
			shouldGoUp = true;
			holdTick -= 0.15f;
		}
	}

	if (holdDownTimer >= 0.25f)
	{
		holdTick += dt;
		if (holdTick >= 0.15f)
		{
			shouldGoDown = true;
			holdTick -= 0.15f;
		}
	}


	if (shouldGoUp)
	{
		if (MissionMenu::index > 0)
		{
			MissionMenu::index = MissionMenu::index - 1;
			AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
		}
	}
	if (shouldGoDown)
	{
		if (MissionMenu::index < (this->counter - 1))
		{
			MissionMenu::index = MissionMenu::index + 1;
			AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
		}
	}

	this->draw();

	if (pressedSelect)
	{
		this->setVisible(false);
 		AudioPlayer::play(38, Global::gameCamera->getFadePosition1());

		Level* currentLevel = &Global::gameLevelData[MissionMenu::index];
		Global::levelName = currentLevel->fileName;
		Global::levelNameDisplay = currentLevel->displayName;
		Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];
		
		Global::createTitleCard();

		Global::isNewLevel = true;
		Global::shouldLoadLevel = true;
		Global::gameState = STATE_RUNNING;
		retVal = SwitchStack::get();
	}
	else if (pressedBack)
	{
		AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
		retVal = PopMenu::get();
	}

	return retVal;
}
