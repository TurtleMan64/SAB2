#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "springtriple.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> SpringTriple::models;

SpringTriple::SpringTriple()
{

}

SpringTriple::SpringTriple(float x, float y, float z, float dirX, float dirZ, float myPower, float cooldownMax)
{
    position.x = x;
    position.y = y;
    position.z = z;
    dir.set(dirX, 0.0f, dirZ);
    dir.normalize();
    springPower = fmaxf(100.0f, myPower);
    cooldownTimer = 0.0f;
    cooldownTimerMax = fmaxf(0.1f, cooldownMax);
    scale = 1;
    visible = true;

    //calculate angles
    rotX = 0;
    rotY = Maths::toDegrees(atan2f(-dir.z, dir.x));
    rotZ = 0;
    rotRoll = 0;

    Vector3f yAxis(0, 1, 0);
    Vector3f side(0, 0, 1);
    side = Maths::rotatePoint(&side, &yAxis, atan2f(-dir.z, dir.x));

    hitCenter1 = position + dir.scaleCopy(5.88928f) + side.scaleCopy(15.0f);
    hitCenter2 = position + dir.scaleCopy(5.88928f);
    hitCenter3 = position + dir.scaleCopy(5.88928f) + side.scaleCopy(-15.0f);
    hitCenter1.y += 5.29734f;
    hitCenter2.y += 5.29734f;
    hitCenter3.y += 5.29734f;

    const float playerRadius = 4.0f;

    end1 = position + dir.scaleCopy(4.6f) + yAxis.scaleCopy(2.65f) + side.scaleCopy(23.5f + playerRadius);
    end2 = position + dir.scaleCopy(4.6f) + yAxis.scaleCopy(2.65f) + side.scaleCopy(-23.5f - playerRadius);

    updateTransformationMatrix();
}

extern float dt;

void SpringTriple::step()
{
    cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

    if (fabsf(hitCenter2.y - Global::gameMainPlayer->position.y) < 40 &&
        fabsf(hitCenter2.z - Global::gameMainPlayer->position.z) < 40 &&
        fabsf(hitCenter2.x - Global::gameMainPlayer->position.x) < 40 &&
        cooldownTimer == 0.0f)
    {
        Vector3f playerCenter = Global::gameMainPlayer->getCenterPosition();
        if (Maths::pointIsInCylinder(&playerCenter, &end1, &end2, 8.25f + 4.0f)) //8.25 = radius of cylinder, 4 = radius of sonic
        {
            Vector3f closestOf1and2 = Maths::getCloserPoint(&hitCenter1, &hitCenter2, &playerCenter);
            Global::gameMainPlayer->position = Maths::getCloserPoint(&closestOf1and2, &hitCenter3, &playerCenter);
            Global::gameMainPlayer->hitSpringTriple(&dir, springPower, cooldownTimerMax);
            AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

            cooldownTimer = cooldownTimerMax;
        }
    }
}

std::list<TexturedModel*>* SpringTriple::getModels()
{
    return &SpringTriple::models;
}

void SpringTriple::loadStaticModels()
{
    if (SpringTriple::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading triple spring static models...\n");
    #endif

    loadModel(&SpringTriple::models, "res/Models/Objects/Spring/", "TripleSpring");
}

void SpringTriple::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting triple spring static models...\n");
    #endif

    Entity::deleteModels(&SpringTriple::models);
}

const bool SpringTriple::canHomingAttackOn()
{
    return true;
}

const Vector3f SpringTriple::getHomingCenter()
{
    return hitCenter2;
}
