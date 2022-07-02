#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "lostchao.hpp"
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

std::list<TexturedModel*> LostChao::models;

LostChao::LostChao()
{

}

LostChao::~LostChao()
{
    if (sourceCry != nullptr)
    {
        sourceCry->stop();
        sourceCry = nullptr;
    }
}

LostChao::LostChao(float x, float y, float z, float rot)
{
    position.set(x, y, z);
    rotY = rot;
    scale = 1;
    visible = true;
    updateTransformationMatrix();
    position.y += 3; //so that collision checking is done from center of chao
}

void LostChao::step()
{
    if (!Global::gameIsChaoMode)
    {
        Global::deleteEntity(this);
    }
    
    float distToPlayerSquared = (Global::gameMainPlayer->position - position).lengthSquared();

    if (distToPlayerSquared < 800.0f*800.0f)
    {
        if (sourceCry == nullptr)
        {
            sourceCry = AudioPlayer::play(61, &position, 1.0f, true);
        }
    }
    else
    {
        if (sourceCry != nullptr)
        {
            sourceCry->stop();
            sourceCry = nullptr;
        }
    }

    if (distToPlayerSquared < 10.0f*10.0f &&
        Global::finishStageTimer < 0.0f)
    {
        Global::mainHudTimer->freeze(true);
        Global::finishStageTimer = 0.0f;
    }

    if (Global::finishStageTimer >= 0.0f)
    {
        visible = false;

        if (Global::finishStageTimer < 1.0f)
        {
            if (sourceCry != nullptr)
            {
                sourceCry->setVolume(AudioPlayer::soundLevelSFX*(1.0f - Global::finishStageTimer));
            }
        }
        else
        {
            if (sourceCry != nullptr)
            {
                sourceCry->stop();
                sourceCry = nullptr;
            }
        }
    }
}

std::list<TexturedModel*>* LostChao::getModels()
{
    return &LostChao::models;
}

void LostChao::loadStaticModels()
{
    if (LostChao::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading LostChao static models...\n");
    #endif

    ModelLoader::loadModel(&LostChao::models, "res/Models/Objects/Chao/", "Chao");
}

void LostChao::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting LostChao static models...\n");
    #endif

    Entity::deleteModels(&LostChao::models);
}
