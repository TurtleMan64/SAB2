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
#include "../audio/source.h"
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

GoalRing::~GoalRing()
{
    if (sourceRing != nullptr)
    {
        sourceRing->stop();
        sourceRing = nullptr;
    }
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
    Global::addEntity(text);
}

void GoalRing::step()
{
    if (Global::gameIsRingMode ||
        Global::gameIsChaoMode)
    {
        Global::deleteEntity(this);
        Global::deleteEntity(text);
    }

    if (Global::gameIsRaceMode && Global::mainHudTimer != nullptr)
    {
        if (Global::mainHudTimer->totalTime >= Global::gameRaceTimeLimit)
        {
            Global::deleteEntity(this);
            Global::deleteEntity(text);

            Global::gameMainPlayer->die();
        }
    }

    extern float dt;
    increaseRotation(0, dt*100, 0);
    text->setRotation(0, rotY, 0, 0);
    
    float distToPlayerSquared = (Global::gameMainPlayer->position - position).lengthSquared();

    if (distToPlayerSquared < 800.0f*800.0f)
    {
        if (sourceRing == nullptr)
        {
            sourceRing = AudioPlayer::play(42, &position, 1.0f, true);
        }
    }
    else
    {
        if (sourceRing != nullptr)
        {
            sourceRing->stop();
            sourceRing = nullptr;
        }
    }

    if (distToPlayerSquared < 30.0f*30.0f &&
        Global::finishStageTimer < 0.0f)
    {
        Global::mainHudTimer->freeze(true);
        Global::finishStageTimer = 0.0f;
    }

    if (Global::finishStageTimer >= 0.0f)
    {
        visible = false;
        text->setVisible(false);

        if (Global::finishStageTimer < 1.0f)
        {
            if (sourceRing != nullptr)
            {
                sourceRing->setVolume(AudioPlayer::soundLevelSFX*(1.0f - Global::finishStageTimer));
            }
        }
        else
        {
            if (sourceRing != nullptr)
            {
                sourceRing->stop();
                sourceRing = nullptr;
            }
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
    std::fprintf(stdout, "Loading GoalRing static models...\n");
    #endif

    loadModel(&GoalRing::modelsRing, "res/Models/Objects/GoalRing/", "Pass1");
    loadModel(&GoalRing::modelsText, "res/Models/Objects/GoalRing/", "Pass2");
}

void GoalRing::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting GoalRing static models...\n");
    #endif

    Entity::deleteModels(&GoalRing::modelsRing);
    Entity::deleteModels(&GoalRing::modelsText);
}
