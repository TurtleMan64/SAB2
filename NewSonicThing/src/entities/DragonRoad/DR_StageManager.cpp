#include <glad/glad.h>

#include "../entity.hpp"
#include "drstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::list<TexturedModel*> DR_StageManager::modelsSkydome;

DR_StageManager::DR_StageManager()
{
    scale = 15.0f;
    visible = true;
}

void DR_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);

    if (position.z < -9670.49f && position.x > 22151.3f)
    {
        Global::waterHeight = -8787.54f;
        position.y = -8910.0f;
    }
    else
    {
        Global::waterHeight = -3495.0f;
        position.y = -4010.0f;
    }

    if (Global::gameMainPlayer->position.y < Global::waterHeight - 10)
    {
        Global::gameMainPlayer->die();
    }

    updateTransformationMatrix();
}

std::list<TexturedModel*>* DR_StageManager::getModels()
{
    return &DR_StageManager::modelsSkydome;
}

void DR_StageManager::loadStaticModels()
{
    if (DR_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DR_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&DR_StageManager::modelsSkydome,  "res/Models/Levels/DragonRoad/Sky/", "Sky");
}

void DR_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DR_StageManager static models...\n");
    #endif

    Entity::deleteModels(&DR_StageManager::modelsSkydome);
}
