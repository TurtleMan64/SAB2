#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "goalring.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "dummy.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../menu/timer.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> GoalRing::modelsRing;
std::list<TexturedModel*> GoalRing::modelsText;

GoalRing::GoalRing()
{

}

GoalRing::GoalRing(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    scale = 1;
    visible = true;

    text = new Dummy(&GoalRing::modelsText); INCR_NEW("Entity");
    text->setVisible(true);
    text->setPosition(&position);
    text->renderOrder = 1;
    Main_addEntity(text);
}

void GoalRing::step()
{
    extern float dt;
    increaseRotation(0, dt*100, 0);
    text->setRotation(0, getRotY(), 0, 0);

    float xDiff = Global::gameMainPlayer->getX()-position.x;
    float yDiff = Global::gameMainPlayer->getY()-position.y;
    float zDiff = Global::gameMainPlayer->getZ()-position.z;

    const float pad = 35.0f;
    if (fabsf(xDiff) < pad &&
        fabsf(yDiff) < pad &&
        fabsf(zDiff) < pad)
    {
        float radDiff = xDiff*xDiff + yDiff*yDiff + zDiff*zDiff;
        const float ringRadius = 30.0f;
        if (radDiff <= ringRadius*ringRadius)
        {
            Global::mainHudTimer->freeze(true);
            Global::finishStageTimer = 0.0f;
            //Global::menuManager.
            visible = false;
            text->setVisible(false);
        }
    }

    updateTransformationMatrix();
    text->updateTransformationMatrix();
}

std::list<TexturedModel*>* GoalRing::getModels()
{
    return &GoalRing::modelsRing;
}

void GoalRing::loadStaticModels()
{
    if (GoalRing::modelsRing.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading Boostpad static models...\n");
    #endif

    loadModel(&GoalRing::modelsRing, "res/Models/Objects/GoalRing/", "Pass1");
    loadModel(&GoalRing::modelsText, "res/Models/Objects/GoalRing/", "Pass2");
}

void GoalRing::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting Boostpad static models...\n");
    #endif

    Entity::deleteModels(&GoalRing::modelsRing);
    Entity::deleteModels(&GoalRing::modelsText);
}
