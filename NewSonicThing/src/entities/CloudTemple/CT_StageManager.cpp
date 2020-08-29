#include <glad/glad.h>

#include "../entity.h"
#include "ctstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../camera.h"

std::list<TexturedModel*> CT_StageManager::modelsSkydome;

CT_StageManager::CT_StageManager()
{
    scale = 55000.0f;
    visible = true;
}

void CT_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    position.y = -5000.0f;
    updateTransformationMatrix();
}

std::list<TexturedModel*>* CT_StageManager::getModels()
{
    return &CT_StageManager::modelsSkydome;
}

void CT_StageManager::loadStaticModels()
{
    if (CT_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading CT_StageManager static models...\n");
    #endif

    ObjLoader::loadModel(&CT_StageManager::modelsSkydome,  "res/Models/Levels/CloudTemple/Sky/", "Sky");
}

void CT_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting CT_StageManager static models...\n");
    #endif

    Entity::deleteModels(&CT_StageManager::modelsSkydome);
}
