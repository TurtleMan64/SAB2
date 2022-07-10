#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "goalring.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../audio/source.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../menu/timer.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> GoalRing::models;

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
}

void GoalRing::step()
{
    if (Global::gameIsRingMode ||
        Global::gameIsChaoMode)
    {
        Global::deleteEntity(this);
  
    }

    if (Global::gameIsRaceMode && Global::mainHudTimer != nullptr)
    {
        if (Global::mainHudTimer->totalTime >= Global::gameRaceTimeLimit)
        {
            Global::deleteEntity(this);

            Global::gameMainPlayer->die();
        }
    }

    extern float dt;
    rotY += dt*100;
    
    float distToPlayerSquared = (Global::gameMainPlayer->getCenterPosition() - position).lengthSquared();

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

    if (distToPlayerSquared < 31.7177f*31.7177f && //25.7177 from ring + 6 from sonic
        Global::finishStageTimer < 0.0f)
    {
        Global::mainHudTimer->freeze(true);
        Global::finishStageTimer = 0.0f;
    }

    if (Global::finishStageTimer >= 0.0f)
    {
        if (Global::finishStageTimer < 1.0f)
        {
            float size = 1.0f - 2.4f*Global::finishStageTimer;

            scale = size;

            if (sourceRing != nullptr)
            {
                sourceRing->setVolume(AudioPlayer::soundLevelSFX*(1.0f - Global::finishStageTimer));
            }
        }
        else
        {
            visible = false;

            if (sourceRing != nullptr)
            {
                sourceRing->stop();
                sourceRing = nullptr;
            }
        }
    }

    updateTransformationMatrix();
}

std::list<TexturedModel*>* GoalRing::getModels()
{
    return &GoalRing::models;
}

void GoalRing::loadStaticModels()
{
    if (GoalRing::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GoalRing static models...\n");
    #endif

    ModelLoader::loadModel(&GoalRing::models, "res/Models/Objects/GoalRing/", "GoalRing");
}

void GoalRing::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GoalRing static models...\n");
    #endif

    Entity::deleteModels(&GoalRing::models);
}
