#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "gumdrop.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../entities/controllableplayer.hpp"
#include "../../toolbox/maths.hpp"
#include "../../audio/audioplayer.hpp"

#include <list>
#include <vector>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> Gumdrop::models;

Gumdrop::Gumdrop()
{

}

Gumdrop::Gumdrop(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower)
{
    position.x = x;
    position.y = y;
    position.z = z;
    dir.set(dirX, dirY, dirZ);
    dir.normalize();
    springPower = fmaxf(100.0f, myPower);
    cooldownTimer = 0.0f;
    scale = 1;
    visible = true;

    hitCenter = position + dir.scaleCopy(13.0f);

    //calculate angles
    rotX = 0;
    rotY = Maths::toDegrees(atan2f(-dir.z, dir.x));
    rotZ = Maths::toDegrees(atan2f(dir.y, sqrtf(dir.x*dir.x + dir.z*dir.z)));
    rotRoll = 0;

    updateTransformationMatrix();
}

extern float dt;

void Gumdrop::step()
{
    bool redraw = (cooldownTimer > 0.0f);
    cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

    if (redraw)
    {
        updateTransformationMatrix(1.0f - (cooldownTimer / cooldownTimerMax), 1.0f, 1.0f);
    }

    if (std::abs(hitCenter.y - Global::gameMainPlayer->position.y) < 40 &&
        std::abs(hitCenter.z - Global::gameMainPlayer->position.z) < 40 &&
        std::abs(hitCenter.x - Global::gameMainPlayer->position.x) < 40 &&
        cooldownTimer == 0.0f)
    {
        if ((Global::gameMainPlayer->getCenterPosition() - hitCenter).lengthSquared() < (25.76f*25.76f)+(4.0f*4.0f)) //25.76 = radius of spring, 4 = radius of sonic
        {
            Global::gameMainPlayer->hitSpringYellow(&dir, springPower, 0.0f);
            AudioPlayer::play(6, &position, 0.5f + (springPower*0.00013333f));

            cooldownTimer = cooldownTimerMax;
        }
    }
}

std::vector<TexturedModel*>* Gumdrop::getModels()
{
    return &Gumdrop::models;
}

void Gumdrop::loadStaticModels()
{
    if (Gumdrop::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Gumdrop static models...\n");
    #endif

    ModelLoader::loadModel(&Gumdrop::models, "res/Models/Levels/SweetMountain/Objects/", "Gumdrop");
}

void Gumdrop::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Gumdrop static models...\n");
    #endif

    Entity::deleteModels(&Gumdrop::models);
}

bool Gumdrop::canHomingAttackOn() const
{
    return true;
}

Vector3f Gumdrop::getHomingCenter() const
{
    return hitCenter;
}
