#include <glad/glad.h>

#include "../entity.h"
#include "rhstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../camera.h"

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
    std::fprintf(stdout, "Loading RH_StageManager static models...\n");
    #endif

    loadModel(&RH_StageManager::modelsSkydome,  "res/Models/Levels/RadicalHighway/Sky/", "Sky");
}

void RH_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting RH_StageManager static models...\n");
    #endif

    Entity::deleteModels(&RH_StageManager::modelsSkydome);
}
