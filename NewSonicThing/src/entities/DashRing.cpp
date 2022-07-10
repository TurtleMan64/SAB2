#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "dashring.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> DashRing::modelsDash;
std::list<TexturedModel*> DashRing::modelsRainbow;

DashRing::DashRing()
{

}

DashRing::DashRing(
    float x,     float y,     float z,
    float dx,    float dy,    float dz,
    float power, float controlLockTime,
    int changeCamera, int rainbow)
{
    position.x = x;
    position.y = y;
    position.z = z;

    forward = Vector3f(dx, dy, dz);
    forward.normalize();

    this->power = power;
    this->controlLockTime = 0.25f;//controlLockTime;
    this->changeCamera = (bool)changeCamera;
    isRainbow = (bool)rainbow;

    constexpr float sonicRadius = 3.0f;

    if (isRainbow)
    {
        ringRadius = 17.0f + 3.0f;

        top    = position + forward.scaleCopy( 4 + sonicRadius);
        bottom = position + forward.scaleCopy(-4 - sonicRadius);
    }
    else
    {
        ringRadius = 20.3f + 3.0f;

        top    = position + forward.scaleCopy( 9 + sonicRadius);
        bottom = position + forward.scaleCopy(-9 - sonicRadius);
    }

    if (power < 700)
    {
        this->power = 700.0f;
    }

    Maths::sphereAnglesFromPosition(&forward, &rotY, &rotZ);
    updateTransformationMatrix();
}

void DashRing::step()
{
    if (std::abs(position.x - Global::gameMainPlayer->position.x) < 50 && 
        std::abs(position.z - Global::gameMainPlayer->position.z) < 50 && 
        std::abs(position.y - Global::gameMainPlayer->position.y) < 50)
    {
        Vector3f diff = Global::gameMainPlayer->position - position;
        if (Maths::pointIsInCylinder(&Global::gameMainPlayer->position, &top, &bottom, ringRadius))
        {
            if (!playerIsIn)
            {
                if (isRainbow)
                {
                    AudioPlayer::play(77, &position, 0.8f);
                }
                else
                {
                    AudioPlayer::play(74, &position, 1.0f);
                }

                Global::gameMainPlayer->vel = forward.scaleCopy(power);
                //Global::gameMainPlayer->vel = Maths::projectOntoPlane(&Global::gameMainPlayer->vel, &Global::gameMainPlayer->relativeUp);
                //Global::gameMainPlayer->vel.setLength(power);
                if (changeCamera)
                {
                    Global::gameMainPlayer->camDir = forward;
                    Global::gameMainPlayer->camDir.y = 0;
                    Global::gameMainPlayer->camDir.normalize();
                }
                Global::gameMainPlayer->canMoveTimer = controlLockTime;
                //Global::gameMainPlayer->hitDashpad();
                Global::gameMainPlayer->onGround = false;
                Global::gameMainPlayer->position = position;
            }

            playerIsIn = true;
        }
        else
        {
            playerIsIn = false;
        }
    }
    else
    {
        playerIsIn = false;
    }
}

std::list<TexturedModel*>* DashRing::getModels()
{
    if (isRainbow)
    {
        return &DashRing::modelsRainbow;
    }

    return &DashRing::modelsDash;
}

void DashRing::loadStaticModels()
{
    if (DashRing::modelsDash.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DashRing static models...\n");
    #endif

    ModelLoader::loadModel(&DashRing::modelsDash,    "res/Models/Objects/DashRing/", "DashRing");
    ModelLoader::loadModel(&DashRing::modelsRainbow, "res/Models/Objects/RainbowRing/", "RainbowRing");
}

void DashRing::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DashRing static models...\n");
    #endif

    Entity::deleteModels(&DashRing::modelsDash);
    Entity::deleteModels(&DashRing::modelsRainbow);
}
