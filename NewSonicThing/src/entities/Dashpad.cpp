#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "dashpad.h"
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
#include "../collision/collisionchecker.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Dashpad::models;

Dashpad::Dashpad()
{

}

Dashpad::Dashpad(
    float x,     float y,     float z,
    float power, float controlLockTime,
    float rotX, float rotY, float rotZ)
{
    position.x = x;
    position.y = y;
    position.z = z;
    scale = 1;
    visible = true;
    playerIsIn = false;

    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;
    this->rotRoll = 0;
    this->power = power;
    this->controlLockTime = controlLockTime;
    updateTransformationMatrixYXZ();

    forward = Vector3f(0, 0, 1);
    up = Vector3f(0, 1, 0);
    Vector3f xAxis(1, 0, 0);
    Vector3f yAxis(0, 1, 0);
    Vector3f zAxis(0, 0, 1);
    forward = Maths::rotatePoint(&forward, &yAxis, Maths::toRadians(rotY));
    forward = Maths::rotatePoint(&forward, &xAxis, Maths::toRadians(rotX));
    forward = Maths::rotatePoint(&forward, &zAxis, Maths::toRadians(rotZ));
    up = Maths::rotatePoint(&up, &yAxis, Maths::toRadians(rotY));
    up = Maths::rotatePoint(&up, &xAxis, Maths::toRadians(rotX));
    up = Maths::rotatePoint(&up, &zAxis, Maths::toRadians(rotZ));
}

void Dashpad::step()
{
    if (fabsf(position.x - Global::gameMainPlayer->position.x) < 50 && 
        fabsf(position.z - Global::gameMainPlayer->position.z) < 50 && 
        fabsf(position.y - Global::gameMainPlayer->position.y) < 50)
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
                Global::gameMainPlayer->camDir = forward;
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

std::list<TexturedModel*>* Dashpad::getModels()
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
    std::fprintf(stdout, "Loading Dashpad static models...\n");
    #endif

    ObjLoader::loadModel(&Dashpad::models,  "res/Models/Objects/Dashpad/", "Dashpad");
}

void Dashpad::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting Dashpad static models...\n");
    #endif

    Entity::deleteModels(&Dashpad::models);
}
