#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "help.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../toolbox/maths.h"
#include "../toolbox/split.h"
#include "camera.h"
#include "../fontMeshCreator/guitext.h"
#include "controllableplayer.h"
#include "../guis/guimanager.h"
#include "../fontMeshCreator/fonttype.h"

#include <list>
#include <iostream>
#include <cstring>
#include <string>

std::list<TexturedModel*> Help::models;
GUIText* Help::messageGUI1 = nullptr;
GUIText* Help::messageGUI2 = nullptr;

Help::Help()
{
	
}

Help::~Help()
{
	if (messageGUI1 != nullptr && (*messageGUI1->getTextString()) == myMessage1)
	{
		messageGUI1->deleteMe();
		delete messageGUI1; INCR_DEL("GUIText")
		messageGUI1 = nullptr;
	}
			
	if (messageGUI2 != nullptr && (*messageGUI2->getTextString()) == myMessage2)
	{
		messageGUI2->deleteMe();
		delete messageGUI2; INCR_DEL("GUIText")
		messageGUI2 = nullptr;
	}
}

Help::Help(float x, float y, float z, std::string message)
{
	position.set(x, y, z);
	visible = true;
    baseColour.set(Maths::nextUniform(), Maths::nextUniform(), Maths::nextUniform());
	updateTransformationMatrix();

	char msgBuf[512];
	memset(msgBuf, 0, 512);
	memcpy(msgBuf, message.c_str(), message.size());

	int splitLength = 0;
	char** msg = split(msgBuf, ';', &splitLength);

	myMessage1 = "";
	myMessage2 = "";

	if (splitLength == 1)
	{
		myMessage2 = msg[0];
	}
	else if (splitLength > 1)
	{
		myMessage1 = msg[0];
		myMessage2 = msg[1];
	}

	free(msg);
}

void Help::step()
{
    extern float dt;
    rotY += 360*dt;
    updateTransformationMatrix();

    Vector3f diff = Global::gameMainPlayer->position - position;
    float magSquared = diff.lengthSquared(); 

	if (magSquared < 20*20 && diff.y > -3 && diff.y < 12)
	{
		if (messageGUI1 == nullptr && myMessage1 != "")
		{
			float size = 3.0f/myMessage1.length();
			size = std::fminf(size, 0.09f);

            messageGUI1 = new GUIText(myMessage1, size, Global::fontVipnagorgialla, 0.5f, 0.785f, 7, true); INCR_NEW("GUIText")
		}
			
		if (messageGUI2 == nullptr && myMessage2 != "")
		{
			float size = 3.0f/myMessage2.length();
			size = std::fminf(size, 0.09f);

            messageGUI2 = new GUIText(myMessage2, size, Global::fontVipnagorgialla, 0.5f, 0.875f, 7, true); INCR_NEW("GUIText")
		}
	}
	else
	{
		if (messageGUI1 != nullptr && (*messageGUI1->getTextString()) == myMessage1)
		{
			messageGUI1->deleteMe();
			delete messageGUI1; INCR_DEL("GUIText")
			messageGUI1 = nullptr;
		}
			
		if (messageGUI2 != nullptr && (*messageGUI2->getTextString()) == myMessage2)
		{
			messageGUI2->deleteMe();
			delete messageGUI2; INCR_DEL("GUIText")
			messageGUI2 = nullptr;
		}
	}
}

std::list<TexturedModel*>* Help::getModels()
{
	return &Help::models;
}

void Help::loadStaticModels()
{
	if (Help::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Help static models...\n");
	#endif

	loadModel(&Help::models, "res/Models/Objects/Help/", "Help");
}

void Help::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Help static models...\n");
	#endif

	Entity::deleteModels(&Help::models);

	if (messageGUI1 != nullptr)
	{
		messageGUI1->deleteMe();
		delete messageGUI1; INCR_DEL("GUIText")
		messageGUI1 = nullptr;
	}

	if (messageGUI2 != nullptr)
	{
		messageGUI2->deleteMe();
		delete messageGUI2; INCR_DEL("GUIText")
		messageGUI2 = nullptr;
	}
}
