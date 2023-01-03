#include <glad/glad.h>

#include "../entity.hpp"
#include "ctstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::vector<TexturedModel*> CT_StageManager::modelsSkydome;

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

std::vector<TexturedModel*>* CT_StageManager::getModels()
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
    printf("Loading CT_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&CT_StageManager::modelsSkydome,  "res/Models/Levels/CloudTemple/Sky/", "Sky");
}

void CT_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting CT_StageManager static models...\n");
    #endif

    Entity::deleteModels(&CT_StageManager::modelsSkydome);
}
