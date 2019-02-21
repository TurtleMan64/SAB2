#include "extras.h"
#include "mainmenu.h"
#include "../engineTester/main.h"

#include <cmath>
#include <algorithm>

#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../toolbox/input.h"
#include "../toolbox/levelloader.h"
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
#include "menumanager.h"
#include <iostream>

Extras::Extras()
{
	FontType * font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"),
		"res/Fonts/vipnagorgialla.fnt");

	timeText = new GUIText("Total Playtime:", 0.1f, font, 0.0f, 0.45f, 1.0f, true, false, true);
	jebaited = new GUIText("bro...........................", 0.01f, font, 0.0f, 0.6f, 1.0f, true, false, true);
}

Extras::~Extras()
{
	this->timeText->deleteMe(); delete this->timeText;
	this->jebaited->deleteMe(); delete this->jebaited;
}

int Extras::step()
{
	bool shouldGoUp = false;
	bool shouldGoDown = false;
	bool shouldGoLeft = false;
	bool shouldGoRight = false;
	bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
	bool pressedBack = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);

	int moveX = (int)round(Input::inputs.INPUT_X);
	int moveY = (int)round(Input::inputs.INPUT_Y);

	if (pressedBack)
	{
		
	}
}