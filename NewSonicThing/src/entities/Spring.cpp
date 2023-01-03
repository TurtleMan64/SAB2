#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "spring.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"

#include <list>
#include <vector>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> Spring::models;

Spring::Spring()
{

}

Spring::Spring(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float cooldownMax, bool setsCam)
{
    position.x = x;
    position.y = y;
    position.z = z;
    dir.set(dirX, dirY, dirZ);
    dir.normalize();
    springPower = fmaxf(100.0f, myPower);
    cooldownTimer = 0.0f;
    cooldownTimerMax = fmaxf(0.1f, cooldownMax);
    scale = 1;
    visible = true;
    resetsCamera = setsCam;

    hitCenter = position + dir.scaleCopy(10.0f);

    //calculate angles
    rotX = 0;
    rotY = Maths::toDegrees(atan2f(-dir.z, dir.x));
    rotZ = Maths::toDegrees(atan2f(dir.y, sqrtf(dir.x*dir.x + dir.z*dir.z)));
    rotRoll = 0;

    updateTransformationMatrix();
}

extern float dt;

void Spring::step()
{
    cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

    if (std::abs(hitCenter.y - Global::gameMainPlayer->position.y) < 40 &&
        std::abs(hitCenter.z - Global::gameMainPlayer->position.z) < 40 &&
        std::abs(hitCenter.x - Global::gameMainPlayer->position.x) < 40 &&
        cooldownTimer == 0.0f)
    {
        if ((Global::gameMainPlayer->getCenterPosition() - hitCenter).lengthSquared() < (10.83f*10.83f)+(4.0f*4.0f)) //10.83 = radius of spring, 4 = radius of sonic
        {
            Global::gameMainPlayer->position = hitCenter;
            Global::gameMainPlayer->hitSpring(&dir, springPower, cooldownTimerMax, resetsCamera);
            AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

            cooldownTimer = cooldownTimerMax;
        }
    }
}

std::vector<TexturedModel*>* Spring::getModels()
{
    return &Spring::models;
}

void Spring::loadStaticModels()
{
    if (Spring::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading spring static models...\n");
    #endif

    ModelLoader::loadModel(&Spring::models, "res/Models/Objects/Spring/", "Spring");
}

void Spring::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting spring static models...\n");
    #endif

    Entity::deleteModels(&Spring::models);
}

bool Spring::canHomingAttackOn() const
{
    return true;
}

Vector3f Spring::getHomingCenter() const
{
    return hitCenter;
}
