#include <glad/glad.h>

#include "../entity.h"
#include "tstagemanager.h"
#include "../../engineTester/main.h"
#include "../dummy.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"

std::list<TexturedModel*> T_StageManager::modelsSkyPass1;
std::list<TexturedModel*> T_StageManager::modelsSkyPass2;
std::list<TexturedModel*> T_StageManager::modelsSkyTransparent;

T_StageManager::T_StageManager()
{
    skyPass2       = new Dummy(&T_StageManager::modelsSkyPass2);       INCR_NEW("Entity");
    skyTransparent = new Dummy(&T_StageManager::modelsSkyTransparent); INCR_NEW("Entity");

    visible = true;
    skyPass2->visible = true;
    skyTransparent->visible = false;

    scale = 3.0f*0.6f;
    skyPass2->scale = 3.0f*0.6f;
    skyTransparent->scale = 1.8f*0.6f;

    Main_addEntityPass2(skyPass2);
    Main_addTransparentEntity(skyTransparent);
}

void T_StageManager::step()
{
	//set the position of the background sky dome
	position.set(&Global::gameMainPlayer->position);
    position.y = Global::waterHeight;
	updateTransformationMatrix();

    skyPass2->setPosition(&position);
    skyTransparent->setPosition(&position);

    skyPass2->updateTransformationMatrix();
    skyTransparent->updateTransformationMatrix();
}

std::list<TexturedModel*>* T_StageManager::getModels()
{
	return &T_StageManager::modelsSkyPass1;
}

void T_StageManager::loadStaticModels()
{
	if (T_StageManager::modelsSkyPass1.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading T_StageManager static models...\n");
	#endif

	loadModel(&T_StageManager::modelsSkyPass1,       "res/Models/Levels/DelfinoPlaza/Sky/", "Blue");
    loadModel(&T_StageManager::modelsSkyPass2,       "res/Models/Levels/DelfinoPlaza/Sky/", "Pass2");
    loadModel(&T_StageManager::modelsSkyTransparent, "res/Models/Levels/DelfinoPlaza/Sky/", "Trans");
}

void T_StageManager::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting T_StageManager static models...\n");
	#endif

	Entity::deleteModels(&T_StageManager::modelsSkyPass1);
    Entity::deleteModels(&T_StageManager::modelsSkyPass2);
    Entity::deleteModels(&T_StageManager::modelsSkyTransparent);
}
