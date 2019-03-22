#include <glad/glad.h>

#include "../entity.h"
#include "srstagemanager.h"
#include "../../engineTester/main.h"
#include "../dummy.h"
#include "../car.h"
#include "../../objLoader/objLoader.h"
#include "../camera.h"
#include "../../toolbox/maths.h"
#include "../../particles/particle.h"
#include "../../particles/particleresources.h"

std::list<TexturedModel*> SRStageManager::modelsSkyPass1;
std::list<TexturedModel*> SRStageManager::modelsSkyPass2;
std::list<TexturedModel*> SRStageManager::modelsSkyPass3;
std::list<TexturedModel*> SRStageManager::modelsSkyTransparent;

SRStageManager::SRStageManager()
{
	visible = true;

    skyPass2       = new Dummy(&SRStageManager::modelsSkyPass2);       INCR_NEW("Entity");
    skyPass3       = new Dummy(&SRStageManager::modelsSkyPass3);       INCR_NEW("Entity");
    skyTransparent = new Dummy(&SRStageManager::modelsSkyTransparent); INCR_NEW("Entity");

    skyPass2->setVisible(true);
    skyPass3->setVisible(true);
    skyTransparent->setVisible(true);

    Main_addEntityPass2(skyPass2);
    Main_addEntityPass3(skyPass3);
    Main_addTransparentEntity(skyTransparent);
}

void SRStageManager::step()
{
	//set the position of the background sky dome
	position.set(Global::gameMainVehicle->getPosition());
	updateTransformationMatrix();

    skyPass2->setPosition(&position);
    skyPass3->setPosition(&position);
    skyTransparent->setPosition(&position);

    skyPass2->updateTransformationMatrix();
    skyPass3->updateTransformationMatrix();
    skyTransparent->updateTransformationMatrix();
}

std::list<TexturedModel*>* SRStageManager::getModels()
{
	return &SRStageManager::modelsSkyPass1;
}

void SRStageManager::loadStaticModels()
{
	if (SRStageManager::modelsSkyPass1.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading SRStageManager static models...\n");
	#endif

	loadModel(&SRStageManager::modelsSkyPass1,       "res/Models/Levels/SkyRail/Sky/", "Pass1");
    loadModel(&SRStageManager::modelsSkyPass2,       "res/Models/Levels/SkyRail/Sky/", "Pass2");
    loadModel(&SRStageManager::modelsSkyPass3,       "res/Models/Levels/SkyRail/Sky/", "Pass3");
    loadModel(&SRStageManager::modelsSkyTransparent, "res/Models/Levels/SkyRail/Sky/", "Transparent");
}

void SRStageManager::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting SRStageManager static models...\n");
	#endif

	Entity::deleteModels(&SRStageManager::modelsSkyPass1);
    Entity::deleteModels(&SRStageManager::modelsSkyPass2);
    Entity::deleteModels(&SRStageManager::modelsSkyPass3);
    Entity::deleteModels(&SRStageManager::modelsSkyTransparent);
}
