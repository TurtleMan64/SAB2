#include <glad/glad.h>

#include "../entity.hpp"
#include "ghstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../dummy.hpp"
#include "../../loading/modelloader.hpp"

std::vector<TexturedModel*> GH_StageManager::modelsSkyCylinder;
std::vector<TexturedModel*> GH_StageManager::modelsSkyClouds;
std::vector<TexturedModel*> GH_StageManager::modelsWaterFlat;
std::vector<TexturedModel*> GH_StageManager::modelsWaterVertical;

GH_StageManager::GH_StageManager()
{
    visible = true;

    skyClouds     = new Dummy(&GH_StageManager::modelsSkyClouds);     INCR_NEW("Entity");
    waterFlat     = new Dummy(&GH_StageManager::modelsWaterFlat);     INCR_NEW("Entity");
    waterVertical = new Dummy(&GH_StageManager::modelsWaterVertical); INCR_NEW("Entity");

    skyClouds->visible = (true);
    waterFlat->visible = (true);
    waterVertical->visible = (true);

    Global::addEntity(skyClouds);
    Global::addEntity(waterFlat);
    Global::addEntity(waterVertical);

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

std::vector<TexturedModel*>* GH_StageManager::getModels()
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
    printf("Loading GH_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&GH_StageManager::modelsSkyCylinder,   "res/Models/Levels/GreenHillZone/Sky/",   "SkyCylinder");
    ModelLoader::loadModel(&GH_StageManager::modelsSkyClouds,     "res/Models/Levels/GreenHillZone/Sky/",   "SkyClouds");
    ModelLoader::loadModel(&GH_StageManager::modelsWaterFlat,     "res/Models/Levels/GreenHillZone/Water/", "StaticWater");
    ModelLoader::loadModel(&GH_StageManager::modelsWaterVertical, "res/Models/Levels/GreenHillZone/Water/", "FlowingWater");
}

void GH_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_StageManager static models...\n");
    #endif

    Entity::deleteModels(&GH_StageManager::modelsSkyCylinder);
    Entity::deleteModels(&GH_StageManager::modelsSkyClouds);
    Entity::deleteModels(&GH_StageManager::modelsWaterFlat);
    Entity::deleteModels(&GH_StageManager::modelsWaterVertical);
}
