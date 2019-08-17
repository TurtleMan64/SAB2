#include <glad/glad.h>

#include "../entity.h"
#include "ghstagemanager.h"
#include "../../engineTester/main.h"
#include "../playersonic.h"
#include "../dummy.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"

std::list<TexturedModel*> GH_StageManager::modelsSkyCylinder;
std::list<TexturedModel*> GH_StageManager::modelsSkyClouds;
std::list<TexturedModel*> GH_StageManager::modelsWaterFlat;
std::list<TexturedModel*> GH_StageManager::modelsWaterVertical;

GH_StageManager::GH_StageManager()
{
	visible = true;

    skyClouds     = new Dummy(&GH_StageManager::modelsSkyClouds);     INCR_NEW("Entity");
    waterFlat     = new Dummy(&GH_StageManager::modelsWaterFlat);     INCR_NEW("Entity");
    waterVertical = new Dummy(&GH_StageManager::modelsWaterVertical); INCR_NEW("Entity");

    skyClouds->setVisible(true);
    waterFlat->setVisible(true);
    waterVertical->setVisible(true);

    Main_addEntity(skyClouds);
    Main_addEntity(waterFlat);
    Main_addEntity(waterVertical);

    position.set(0,0,0);
    skyClouds->setPosition(&position);
    waterFlat->setPosition(&position);
    waterVertical->setPosition(&position);

    skyClouds->updateTransformationMatrix();
    waterFlat->updateTransformationMatrix();
    waterVertical->updateTransformationMatrix();
}

void GH_StageManager::step()
{
    extern float dt;

	//set the position of the background sky dome
	position.set(Global::gameMainPlayer->getPosition());
    position.y = 0;
	updateTransformationMatrix();

    skyClouds->setPosition(&position);
    skyClouds->increaseRotation(0, 5*dt, 0);
    skyClouds->updateTransformationMatrix();
}

std::list<TexturedModel*>* GH_StageManager::getModels()
{
	return &GH_StageManager::modelsSkyCylinder;
}

void GH_StageManager::loadStaticModels()
{
	if (GH_StageManager::modelsSkyCylinder.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading GH_StageManager static models...\n");
	#endif

	loadModel(&GH_StageManager::modelsSkyCylinder,   "res/Models/Levels/GreenHillNew/Sky/",   "SkyCylinder");
    loadModel(&GH_StageManager::modelsSkyClouds,     "res/Models/Levels/GreenHillNew/Sky/",   "SkyClouds");
    loadModel(&GH_StageManager::modelsWaterFlat,     "res/Models/Levels/GreenHillNew/Water/", "StaticWater");
    loadModel(&GH_StageManager::modelsWaterVertical, "res/Models/Levels/GreenHillNew/Water/", "FlowingWater");
}

void GH_StageManager::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting GH_StageManager static models...\n");
	#endif

	Entity::deleteModels(&GH_StageManager::modelsSkyCylinder);
    Entity::deleteModels(&GH_StageManager::modelsSkyClouds);
    Entity::deleteModels(&GH_StageManager::modelsWaterFlat);
    Entity::deleteModels(&GH_StageManager::modelsWaterVertical);
}
