#include <glad/glad.h>

#include "../entity.hpp"
#include "rhstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::list<TexturedModel*> RH_StageManager::modelsSkydome;

RH_StageManager::RH_StageManager()
{
    scale = 1.0f;
    visible = true;
}

void RH_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    updateTransformationMatrix();
}

std::list<TexturedModel*>* RH_StageManager::getModels()
{
    return &RH_StageManager::modelsSkydome;
}

void RH_StageManager::loadStaticModels()
{
    if (RH_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading RH_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&RH_StageManager::modelsSkydome,  "res/Models/Levels/RadicalHighway/Sky/", "Sky");
}

void RH_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting RH_StageManager static models...\n");
    #endif

    Entity::deleteModels(&RH_StageManager::modelsSkydome);
}
