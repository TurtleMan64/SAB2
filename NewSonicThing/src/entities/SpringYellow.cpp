#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "springyellow.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"

#include <list>
#include <vector>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> SpringYellow::models;

SpringYellow::SpringYellow()
{

}

SpringYellow::SpringYellow(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float cooldownMax, float radius)
{
    position.x = x;
    position.y = y;
    position.z = z;
    dir.set(dirX, dirY, dirZ);
    dir.normalize();
    springPower = myPower;
    springRadius = radius;
    cooldownTimer = 0.0f;
    cooldownTimerMax = fmaxf(0.0f, cooldownMax);
    scale = springRadius;
    visible = true;

    hitCenter = position + dir.scaleCopy(1.10878f*springRadius);

    //calculate angles
    rotX = 0;
    rotY = Maths::toDegrees(atan2f(-dir.z, dir.x));
    rotZ = Maths::toDegrees(atan2f(dir.y, sqrtf(dir.x*dir.x + dir.z*dir.z)));
    rotRoll = 0;

    fastCheckRadius = springRadius*2;
    collisionP1 = position + dir.scaleCopy(0.92225f*springRadius - 4); //4 = collision radius of sonic
    collisionP2 = position + dir.scaleCopy(1.31792f*springRadius + 4);

    updateTransformationMatrix();
}

extern float dt;

void SpringYellow::step()
{
    cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

    if (fabsf(hitCenter.y - Global::gameMainPlayer->position.y) < fastCheckRadius &&
        fabsf(hitCenter.z - Global::gameMainPlayer->position.z) < fastCheckRadius &&
        fabsf(hitCenter.x - Global::gameMainPlayer->position.x) < fastCheckRadius &&
        cooldownTimer == 0.0f)
    {
        Vector3f playerCenter = Global::gameMainPlayer->getCenterPosition();
        if (Maths::pointIsInCylinder(&playerCenter, &collisionP1, &collisionP2, springRadius + 4))
        {
            Global::gameMainPlayer->hitSpringYellow(&dir, springPower, cooldownTimerMax);
            AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

            cooldownTimer = 0.3f;
        }
    }
}

std::list<TexturedModel*>* SpringYellow::getModels()
{
    return &SpringYellow::models;
}

void SpringYellow::loadStaticModels()
{
    if (SpringYellow::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading SpringYellow static models...\n");
    #endif

    ObjLoader::loadModel(&SpringYellow::models, "res/Models/Objects/Spring/", "SpringYellow");
}

void SpringYellow::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting SpringYellow static models...\n");
    #endif

    Entity::deleteModels(&SpringYellow::models);
}

bool SpringYellow::canHomingAttackOn() const
{
    return true;
}

Vector3f SpringYellow::getHomingCenter() const
{
    return hitCenter;
}
