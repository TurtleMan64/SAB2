#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "speedramp.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/playersonic.hpp"
#include "../toolbox/maths.hpp"
#include "camera.hpp"
#include "../collision/collisionmodel.hpp"
#include "../collision/collisionchecker.hpp"
#include "../audio/audioplayer.hpp"

#include <list>
#include <iostream>
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

        AudioPlayer::play(21, &position);
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
    printf("Loading SpeedRamp static models...\n");
    #endif

    ModelLoader::loadModel(&SpeedRamp::models, "res/Models/Objects/BigJump/", "BigJump");

    if (SpeedRamp::cmOriginal == nullptr)
    {
        SpeedRamp::cmOriginal = ModelLoader::loadCollisionModel("Models/Objects/BigJump/", "BigJump");
    }
}

void SpeedRamp::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SpeedRamp static models...\n");
    #endif

    Entity::deleteModels(&SpeedRamp::models);
    Entity::deleteCollisionModel(&SpeedRamp::cmOriginal);
}
