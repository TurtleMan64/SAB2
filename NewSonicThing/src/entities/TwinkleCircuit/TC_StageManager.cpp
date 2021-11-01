#include <glad/glad.h>

#include "../entity.h"
#include "tcstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../loading/modelloader.h"
#include "../camera.h"

std::list<TexturedModel*> TC_StageManager::modelsSkydome;

TC_StageManager::TC_StageManager()
{
    scale = 8.0f;
}

void TC_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    updateTransformationMatrix();
}

std::list<TexturedModel*>* TC_StageManager::getModels()
{
    return &TC_StageManager::modelsSkydome;
}

void TC_StageManager::loadStaticModels()
{
    if (TC_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading TC_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&TC_StageManager::modelsSkydome,  "res/Models/Levels/TwinkleCircuit/Sky/", "Sky");
}

void TC_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting TC_StageManager static models...\n");
    #endif

    Entity::deleteModels(&TC_StageManager::modelsSkydome);
}
