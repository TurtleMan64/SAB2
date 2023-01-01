#include <glad/glad.h>

#include "../entity.hpp"
#include "ffstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"

std::list<TexturedModel*> FF_StageManager::modelsSkydome;

FF_StageManager::FF_StageManager()
{
    visible = true;
}

void FF_StageManager::step()
{
    //set the position of the background sky dome
    position.y = Global::gameMainPlayer->position.y;
    position.x = Global::gameMainPlayer->position.x;
    position.z = Global::gameMainPlayer->position.z;
    updateTransformationMatrix();
}

std::list<TexturedModel*>* FF_StageManager::getModels()
{
    return &FF_StageManager::modelsSkydome;
}

void FF_StageManager::loadStaticModels()
{
    if (FF_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading FF_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&FF_StageManager::modelsSkydome, "res/Models/Levels/FrogForest/", "Skybox");
}

void FF_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting FF_StageManager static models...\n");
    #endif

    Entity::deleteModels(&FF_StageManager::modelsSkydome);
}
