#include <glad/glad.h>

#include "../entity.hpp"
#include "smstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"

std::list<TexturedModel*> SM_StageManager::modelsSky0;
std::list<TexturedModel*> SM_StageManager::modelsSky1;
std::list<TexturedModel*> SM_StageManager::modelsSky2;
std::list<TexturedModel*> SM_StageManager::modelsSky3;
std::list<TexturedModel*> SM_StageManager::modelsSky4;
std::list<TexturedModel*> SM_StageManager::modelsSky5;
std::list<TexturedModel*> SM_StageManager::modelsSky6;

SM_StageManager::SM_StageManager()
{
    sky1 = new Dummy(&SM_StageManager::modelsSky1); INCR_NEW("Entity");
    sky2 = new Dummy(&SM_StageManager::modelsSky2); INCR_NEW("Entity");
    sky3 = new Dummy(&SM_StageManager::modelsSky3); INCR_NEW("Entity");
    sky4 = new Dummy(&SM_StageManager::modelsSky4); INCR_NEW("Entity");
    sky5 = new Dummy(&SM_StageManager::modelsSky5); INCR_NEW("Entity");

    //sky1->visible = false;
    //sky2->visible = false;
    //sky3->visible = false;
    //sky4->visible = false;
    //sky5->visible = false;
    //visible = false;

    Global::addEntity(sky1);
    Global::addEntity(sky2);
    Global::addEntity(sky3);
    Global::addEntity(sky4);
    Global::addEntity(sky5);

    sky1->baseAlpha = 0.55f;
    sky3->baseAlpha = 0.55f;
    sky4->baseAlpha = 1.5f;
    sky5->baseAlpha = 0.75f;
}

void SM_StageManager::step()
{
    extern float dt;

    //set the position of the background sky dome
    position.set(&Global::gameCamera->eye);
    position.y = 400;

    sky1->position = position;
    sky2->position = position;
    sky3->position = position;
    sky4->position = position;
    sky5->position = position;

    rotY -= 0.5f*dt;
    sky1->rotY += 0.25f*dt;
    sky2->rotY += 0.75f*dt;
    sky3->rotY -= 1.25f*dt;
    sky4->rotY += 1.5f*dt;
    sky5->rotY -= 1.0f*dt;

    updateTransformationMatrix();
    sky1->updateTransformationMatrix();
    sky2->updateTransformationMatrix();
    sky3->updateTransformationMatrix();
    sky4->updateTransformationMatrix();
    sky5->updateTransformationMatrix();
}

std::list<TexturedModel*>* SM_StageManager::getModels()
{
    return &SM_StageManager::modelsSky0;
}

void SM_StageManager::loadStaticModels()
{
    if (SM_StageManager::modelsSky0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SM_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&SM_StageManager::modelsSky0, "res/Models/Levels/Candy/Sky/", "0");
    ModelLoader::loadModel(&SM_StageManager::modelsSky1, "res/Models/Levels/Candy/Sky/", "1");
    ModelLoader::loadModel(&SM_StageManager::modelsSky2, "res/Models/Levels/Candy/Sky/", "2");
    ModelLoader::loadModel(&SM_StageManager::modelsSky3, "res/Models/Levels/Candy/Sky/", "3");
    ModelLoader::loadModel(&SM_StageManager::modelsSky4, "res/Models/Levels/Candy/Sky/", "4");
    ModelLoader::loadModel(&SM_StageManager::modelsSky5, "res/Models/Levels/Candy/Sky/", "5");
}

void SM_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SM_StageManager static models...\n");
    #endif

    Entity::deleteModels(&SM_StageManager::modelsSky0);
    Entity::deleteModels(&SM_StageManager::modelsSky1);
    Entity::deleteModels(&SM_StageManager::modelsSky2);
    Entity::deleteModels(&SM_StageManager::modelsSky3);
    Entity::deleteModels(&SM_StageManager::modelsSky4);
    Entity::deleteModels(&SM_StageManager::modelsSky5);
}
