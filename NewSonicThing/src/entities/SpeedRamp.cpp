#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "speedramp.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/playersonic.h"
#include "../toolbox/maths.h"
#include "camera.h"
#include "../collision/collisionmodel.h"
#include "../collision/collisionchecker.h"
#include "../audio/audioplayer.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> SpeedRamp::models;
CollisionModel* SpeedRamp::cmOriginal;

SpeedRamp::SpeedRamp()
{

}

SpeedRamp::SpeedRamp(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float inputLock)
{
    position.x = x;
    position.y = y;
    position.z = z;
    scale = 1;
    visible = true;
    power = myPower;
    inputLockDuration = inputLock;

    Vector3f forward(dirX, dirY, dirZ);
    forward.normalize();

    Vector3f up(0, 1, 0);
    Vector3f perpen = forward.cross(&up);
    launchDirection = Maths::rotatePoint(&forward, &perpen, Maths::toRadians(20.0f));
    launchDirection.normalize();

    //calculate angles
    rotX = Maths::toDegrees(atan2f(forward.y, sqrtf(forward.x*forward.x + forward.z*forward.z)));
    rotY = Maths::toDegrees(atan2f(-forward.x, -forward.z));
    rotZ = 0;
    rotRoll = 0;

    updateTransformationMatrix();

    collideModelOriginal = SpeedRamp::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModelXYZ();
}

void SpeedRamp::step()
{
    if (collideModelTransformed->playerIsOn)
    {
        Global::gameMainPlayer->hitSpeedRamp(&launchDirection, power, inputLockDuration);

        AudioPlayer::play(21, getPosition());
    }
}

std::list<TexturedModel*>* SpeedRamp::getModels()
{
    return &SpeedRamp::models;
}

void SpeedRamp::loadStaticModels()
{
    if (SpeedRamp::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading SpeedRamp static models...\n");
    #endif

    loadModel(&SpeedRamp::models, "res/Models/Objects/BigJump/", "BigJump");

    if (SpeedRamp::cmOriginal == nullptr)
    {
        SpeedRamp::cmOriginal = loadCollisionModel("Models/Objects/BigJump/", "BigJump");
    }
}

void SpeedRamp::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting SpeedRamp static models...\n");
    #endif

    Entity::deleteModels(&SpeedRamp::models);
    Entity::deleteCollisionModel(&SpeedRamp::cmOriginal);
}
