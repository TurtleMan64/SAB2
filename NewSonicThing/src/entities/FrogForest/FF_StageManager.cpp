#include <glad/glad.h>

#include "../entity.h"
#include "ffstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"

std::list<TexturedModel*> FF_StageManager::modelsSkydome;

FF_StageManager::FF_StageManager()
{
    visible = true;
}

void FF_StageManager::step()
{
    //set the position of the background sky dome
    position.y = Global::gameMainPlayer->getY();
    position.x = Global::gameMainPlayer->getX();
    position.z = Global::gameMainPlayer->getZ();
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
    std::fprintf(stdout, "Loading FF_StageManager static models...\n");
    #endif

    ObjLoader::loadModel(&FF_StageManager::modelsSkydome, "res/Models/Levels/FrogForest/", "Skybox");
}

void FF_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting FF_StageManager static models...\n");
    #endif

    Entity::deleteModels(&FF_StageManager::modelsSkydome);
}