#include <glad/glad.h>

#include "../entity.hpp"
#include "shstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"
#include "../../toolbox/maths.hpp"

std::vector<TexturedModel*> SH_StageManager::modelsSkyPass1;
std::vector<TexturedModel*> SH_StageManager::modelsSkyPass2;

SH_StageManager::SH_StageManager()
{
    skyPass2 = new Dummy(&SH_StageManager::modelsSkyPass2); INCR_NEW("Entity");

    visible = true;
    skyPass2->visible = true;

    scale = 6.5f;
    skyPass2->scale = 6.48f;

    Global::addEntity(skyPass2);
}

void SH_StageManager::step()
{
    //setting the global water heights
    if (Global::gameCamera->eye.x >    257 && Global::gameCamera->eye.x <   1500 && 
        Global::gameCamera->eye.z > -31414 && Global::gameCamera->eye.z < -29139)
    {
        Global::waterHeight = -400.0f;
        Global::deathHeight = -500.0f;
    }
    else
    {
        Global::waterHeight = 0.0f;
        Global::deathHeight = -100.0f;
    }

    //set the position of the background sky dome
    position.set(&Global::gameMainPlayer->position);
    position.y = 0;
    updateTransformationMatrix();

    skyPass2->setPosition(&position);
    skyPass2->updateTransformationMatrix();
}

std::vector<TexturedModel*>* SH_StageManager::getModels()
{
    return &SH_StageManager::modelsSkyPass1;
}

void SH_StageManager::loadStaticModels()
{
    if (SH_StageManager::modelsSkyPass1.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SH_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&SH_StageManager::modelsSkyPass1, "res/Models/Levels/DelfinoPlaza/Sky/", "Blue");
    ModelLoader::loadModel(&SH_StageManager::modelsSkyPass2, "res/Models/Levels/DelfinoPlaza/Sky/", "Pass2");
}

void SH_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SH_StageManager static models...\n");
    #endif

    Entity::deleteModels(&SH_StageManager::modelsSkyPass1);
    Entity::deleteModels(&SH_StageManager::modelsSkyPass2);
}
