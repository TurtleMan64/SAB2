#include <glad/glad.h>

#include "../entity.h"
#include "csstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../camera.h"

std::list<TexturedModel*> CS_StageManager::modelsSkydome;

CS_StageManager::CS_StageManager()
{
    scale = 50000.0f;
    visible = true;
}

void CS_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    position.y = -13000.0f;
    updateTransformationMatrix();
}

std::list<TexturedModel*>* CS_StageManager::getModels()
{
    return &CS_StageManager::modelsSkydome;
}

void CS_StageManager::loadStaticModels()
{
    if (CS_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading CS_StageManager static models...\n");
    #endif

    loadModel(&CS_StageManager::modelsSkydome,  "res/Models/Levels/CloudStage/Sky/", "Sky");
}

void CS_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting CS_StageManager static models...\n");
    #endif

    Entity::deleteModels(&CS_StageManager::modelsSkydome);
}
