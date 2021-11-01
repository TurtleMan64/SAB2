#include <glad/glad.h>

#include "../entity.h"
#include "dlstagemanager.h"
#include "../../engineTester/main.h"
#include "../dummy.h"
#include "../controllableplayer.h"
#include "../../loading/modelloader.h"
#include "../camera.h"

std::list<TexturedModel*> DL_StageManager::modelsSkydome;

DL_StageManager::DL_StageManager()
{
    visible = true;
}

void DL_StageManager::step()
{
    //setting the global water heights
    if (Global::gameCamera->eye.z > 666)
    {
        if (Global::gameCamera->eye.x > 676)
        {
            Global::waterHeight = 58.5f;
        }
        else
        {
            Global::waterHeight = -1.5f;
        }
    }
    else
    {
        Global::waterHeight = -1.5f;
    }

    //set the position of the background sky dome
    position.y = 0.0f;
    position.x = Global::gameCamera->eye.x;
    position.z = Global::gameCamera->eye.z;
    updateTransformationMatrix();
}

std::list<TexturedModel*>* DL_StageManager::getModels()
{
    return &DL_StageManager::modelsSkydome;
}

void DL_StageManager::loadStaticModels()
{
    if (DL_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading DL_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&DL_StageManager::modelsSkydome,  "res/Models/Levels/DryLagoon/", "Skydrum");
}

void DL_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting DL_StageManager static models...\n");
    #endif

    Entity::deleteModels(&DL_StageManager::modelsSkydome);
}
