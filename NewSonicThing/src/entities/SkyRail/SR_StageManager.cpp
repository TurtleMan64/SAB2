#include <glad/glad.h>

#include "../entity.hpp"
#include "srstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"

std::vector<TexturedModel*> SR_StageManager::modelsSkyPass1;
std::vector<TexturedModel*> SR_StageManager::modelsSkyPass2;
std::vector<TexturedModel*> SR_StageManager::modelsSkyPass3;
std::vector<TexturedModel*> SR_StageManager::modelsSkyTransparent;

SR_StageManager::SR_StageManager()
{
    visible = true;

    skyPass2       = new Dummy(&SR_StageManager::modelsSkyPass2);       INCR_NEW("Entity");
    skyPass3       = new Dummy(&SR_StageManager::modelsSkyPass3);       INCR_NEW("Entity");
    skyTransparent = new Dummy(&SR_StageManager::modelsSkyTransparent); INCR_NEW("Entity");

    skyPass2->visible = true;
    skyPass3->visible = false;
    skyTransparent->visible = false;

    Global::addEntity(skyPass2);
    Global::addEntity(skyPass3);
    Global::addEntity(skyTransparent);
}

void SR_StageManager::step()
{
    //set the position of the background sky dome
    position.set(&Global::gameMainPlayer->position);
    updateTransformationMatrix();

    skyPass2->setPosition(&position);
    skyPass3->setPosition(&position);
    skyTransparent->setPosition(&position);

    skyPass2->updateTransformationMatrix();
    skyPass3->updateTransformationMatrix();
    skyTransparent->updateTransformationMatrix();
}

std::vector<TexturedModel*>* SR_StageManager::getModels()
{
    return &SR_StageManager::modelsSkyPass1;
}

void SR_StageManager::loadStaticModels()
{
    if (SR_StageManager::modelsSkyPass1.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SR_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&SR_StageManager::modelsSkyPass1,       "res/Models/Levels/SkyRail/Sky/", "Pass1");
    ModelLoader::loadModel(&SR_StageManager::modelsSkyPass2,       "res/Models/Levels/SkyRail/Sky/", "Pass2New");
    ModelLoader::loadModel(&SR_StageManager::modelsSkyPass3,       "res/Models/Levels/SkyRail/Sky/", "Pass3");
    ModelLoader::loadModel(&SR_StageManager::modelsSkyTransparent, "res/Models/Levels/SkyRail/Sky/", "Transparent");

    setModelsRenderOrder(&SR_StageManager::modelsSkyPass2,       1);
    setModelsRenderOrder(&SR_StageManager::modelsSkyPass3,       2);
    setModelsRenderOrder(&SR_StageManager::modelsSkyTransparent, 3);
}

void SR_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SRStageManager static models...\n");
    #endif

    Entity::deleteModels(&SR_StageManager::modelsSkyPass1);
    Entity::deleteModels(&SR_StageManager::modelsSkyPass2);
    Entity::deleteModels(&SR_StageManager::modelsSkyPass3);
    Entity::deleteModels(&SR_StageManager::modelsSkyTransparent);
}
