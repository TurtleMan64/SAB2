#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ffmushroom.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../entities/controllableplayer.hpp"
#include "../../toolbox/maths.hpp"
#include "../../audio/audioplayer.hpp"

#include <vector>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> FF_Mushroom::models;

FF_Mushroom::FF_Mushroom()
{

}

FF_Mushroom::FF_Mushroom(float x, float y, float z, float yRot)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotY = yRot - 90;

    cooldownTimer = 0.0f;
    visible = true;

    hitCenter = Vector3f(0, 19.55f, -30.0f);
    hitCenter = Maths::rotatePoint(&hitCenter, &Y_AXIS, Maths::toRadians(rotY));
    hitCenter = hitCenter + position;

    updateTransformationMatrix();
}

extern float dt;

void FF_Mushroom::step()
{
    cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

    if (std::abs(hitCenter.y - Global::gameMainPlayer->position.y) < 70 &&
        std::abs(hitCenter.z - Global::gameMainPlayer->position.z) < 70 &&
        std::abs(hitCenter.x - Global::gameMainPlayer->position.x) < 70 &&
        cooldownTimer == 0.0f)
    {
        Vector3f diff = (Global::gameMainPlayer->getCenterPosition() - hitCenter);
        if (diff.y >= 0.0f && diff.lengthSquared() < springRadius*springRadius)
        {
            Global::gameMainPlayer->hitSpringYellow(&Y_AXIS, springPower, cooldownTimerMax);
            Global::gameMainPlayer->vel.y = 0;
            Global::gameMainPlayer->vel.scale(0.1f);
            Global::gameMainPlayer->vel.y = springPower;

            AudioPlayer::play(6, &position, 0.4f + (springPower*0.00013333f));

            cooldownTimer = cooldownTimerMax;
        }
    }
}

std::vector<TexturedModel*>* FF_Mushroom::getModels()
{
    return &FF_Mushroom::models;
}

void FF_Mushroom::loadStaticModels()
{
    if (FF_Mushroom::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading FF_Mushroom static models...\n");
    #endif

    ModelLoader::loadModel(&FF_Mushroom::models, "res/Models/Levels/FrogForest/Objects/", "Mushroom");
}

void FF_Mushroom::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting FF_Mushroom static models...\n");
    #endif

    Entity::deleteModels(&FF_Mushroom::models);
}

bool FF_Mushroom::canHomingAttackOn() const
{
    return false;
}

