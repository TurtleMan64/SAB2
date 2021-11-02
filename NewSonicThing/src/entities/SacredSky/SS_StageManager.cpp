#include <glad/glad.h>

#include "../entity.hpp"
#include "ssstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::list<TexturedModel*> SS_StageManager::modelsSkydome;

SS_StageManager::SS_StageManager()
{
    scale = 50000.0f;
    visible = true;
}

void SS_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    position.y = -13000.0f;
    updateTransformationMatrix();
}

std::list<TexturedModel*>* SS_StageManager::getModels()
{
    return &SS_StageManager::modelsSkydome;
}

void SS_StageManager::loadStaticModels()
{
    if (SS_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading CS_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&SS_StageManager::modelsSkydome,  "res/Models/Levels/SacredSky/Sky/", "Sky");
}

void SS_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting CS_StageManager static models...\n");
    #endif

    Entity::deleteModels(&SS_StageManager::modelsSkydome);
}
