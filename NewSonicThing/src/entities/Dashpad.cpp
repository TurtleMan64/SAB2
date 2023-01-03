#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "dashpad.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "dummy.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../collision/collisionchecker.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> Dashpad::models;

Dashpad::Dashpad()
{

}

Dashpad::Dashpad(
    float x,     float y,     float z,
    float power, float controlLockTime,
    float rotX, float rotY, float rotZ,
    float camScale)
{
    position.x = x;
    position.y = y;
    position.z = z;

    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;
    this->power = power;
    this->controlLockTime = controlLockTime;
    this->camScale = camScale;
    updateTransformationMatrix();

    forward = Vector3f(1, 0, 0);
    up = Vector3f(0, 1, 0);
    Vector3f xAxis(1, 0, 0);
    Vector3f yAxis(0, 1, 0);
    Vector3f zAxis(0, 0, 1);
    forward = Maths::rotatePoint(&forward, &xAxis, Maths::toRadians(rotX));
    forward = Maths::rotatePoint(&forward, &zAxis, Maths::toRadians(rotZ));
    forward = Maths::rotatePoint(&forward, &yAxis, Maths::toRadians(rotY));
    up = Maths::rotatePoint(&up, &xAxis, Maths::toRadians(rotX));
    up = Maths::rotatePoint(&up, &zAxis, Maths::toRadians(rotZ));
    up = Maths::rotatePoint(&up, &yAxis, Maths::toRadians(rotY));
}

void Dashpad::step()
{
    if (std::abs(position.x - Global::gameMainPlayer->position.x) < 50 && 
        std::abs(position.z - Global::gameMainPlayer->position.z) < 50 && 
        std::abs(position.y - Global::gameMainPlayer->position.y) < 50)
    {
        Vector3f diff = Global::gameMainPlayer->position - position;

        if (diff.lengthSquared() < 13.0f*13.0f && Global::gameMainPlayer->onGround)
        {
            if (!playerIsIn)
            {
                AudioPlayer::play(1, &position);
                Global::gameMainPlayer->vel = forward.scaleCopy(power);
                Global::gameMainPlayer->vel = Maths::projectOntoPlane(&Global::gameMainPlayer->vel, &Global::gameMainPlayer->relativeUp);
                Global::gameMainPlayer->vel.setLength(power);
                Global::gameMainPlayer->camDir = forward.scaleCopy(camScale);
                Global::gameMainPlayer->canMoveTimer = controlLockTime;
                Global::gameMainPlayer->hitDashpad();

                Vector3f pointUp = position + up.scaleCopy(5.0f);
                Vector3f pointDown = position + up.scaleCopy(-5.0f);
                if (CollisionChecker::checkCollision(&pointUp, &pointDown))
                {
                    Global::gameMainPlayer->position.set(CollisionChecker::getCollidePosition());
                    Global::gameMainPlayer->position = Global::gameMainPlayer->position + Global::gameMainPlayer->relativeUp.scaleCopy(0.1f);
                }
                else
                {
                    Global::gameMainPlayer->position = position;
                }
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

std::vector<TexturedModel*>* Dashpad::getModels()
{
    return &Dashpad::models;
}

void Dashpad::loadStaticModels()
{
    if (Dashpad::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Dashpad static models...\n");
    #endif

    ModelLoader::loadModel(&Dashpad::models,  "res/Models/Objects/Dashpad/", "Dashpad");
}

void Dashpad::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Dashpad static models...\n");
    #endif

    Entity::deleteModels(&Dashpad::models);
}
