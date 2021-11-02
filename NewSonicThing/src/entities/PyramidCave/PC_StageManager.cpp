#include <glad/glad.h>

#include "../entity.hpp"
#include "pcstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::list<TexturedModel*> PC_StageManager::modelsSkydome;

PC_StageManager::PC_StageManager()
{
    scale = 1.0f;
    position.set(-2560, -2560, -11920);
    updateTransformationMatrix();
    visible = true;
}

void PC_StageManager::step()
{
    if (Global::gameMainPlayer->position.z < -9694.91f && Global::gameMainPlayer->position.z > -13314.9f)
    {
        visible = true;
    }
    else
    {
        visible = false;
    }
}

std::list<TexturedModel*>* PC_StageManager::getModels()
{
    return &PC_StageManager::modelsSkydome;
}

void PC_StageManager::loadStaticModels()
{
    if (PC_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading PC_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&PC_StageManager::modelsSkydome,  "res/Models/Levels/PyramidCave/Sky/", "Sky");
}

void PC_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting PC_StageManager static models...\n");
    #endif

    Entity::deleteModels(&PC_StageManager::modelsSkydome);
}
