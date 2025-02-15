#include <glad/glad.h>

#include "../entity.hpp"
#include "tcstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::vector<TexturedModel*> TC_StageManager::modelsSkydome;

TC_StageManager::TC_StageManager()
{
    scale = 8.0f;
}

void TC_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    updateTransformationMatrix();
}

std::vector<TexturedModel*>* TC_StageManager::getModels()
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
    printf("Loading TC_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&TC_StageManager::modelsSkydome,  "res/Models/Levels/TwinkleCircuit/Sky/", "Sky");
}

void TC_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting TC_StageManager static models...\n");
    #endif

    Entity::deleteModels(&TC_StageManager::modelsSkydome);
}
