#include "gfvine.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../controllableplayer.hpp"

std::list<TexturedModel*> GF_Vine::models;

GF_Vine::GF_Vine()
{

}

GF_Vine::GF_Vine(
    float x, float y, float z,
    float endX, float endY, float endZ,
    float rotAxisX, float rotAxisY, float rotAxisZ,
    float numRotations,
    float launchX, float launchY, float launchZ,
    float rotSpd,
    float launchSpd,
    float inputLock)
{
    position.set(x, y, z);
    Vector3f endPos(endX, endY, endZ);
    initialDirection = endPos - position;
    length = initialDirection.length();
    initialDirection.normalize();
    rotationAxis.set(rotAxisX, rotAxisY, rotAxisZ);
    numRotationsUntilFinish = numRotations;
    launchDirection.set(launchX, launchY, launchZ);
    launchDirection.normalize();
    rotateSpeed = rotSpd;
    launchSpeed = launchSpd;
    inputLockTime = inputLock;

    currentDirection = initialDirection;
    angleChangeSpeed = 0;
    currentRotations = 0;
    playerIsHoldingOn = false;

    visible = true;

    Maths::sphereAnglesFromPosition(&currentDirection, &rotY, &rotZ);
    updateTransformationMatrix(length, 1, 1);

    const float pad = 10.0f;
    minX = fminf(endPos.x, position.x) - pad;
    maxX = fmaxf(endPos.x, position.x) + pad;
    minY = fminf(endPos.y, position.y) - pad;
    maxY = fmaxf(endPos.y, position.y) + pad;
    minZ = fminf(endPos.z, position.z) - pad;
    maxZ = fmaxf(endPos.z, position.z) + pad;
}

extern float dt;

void GF_Vine::step()
{
    if (!playerIsHoldingOn)
    {
        Vector3f pos = Global::gameMainPlayer->position;
        if (minX < pos.x && pos.x < maxX &&
            minZ < pos.z && pos.z < maxZ &&
            minY < pos.y && pos.y < maxY)
        {
            Vector3f playerPos = Global::gameMainPlayer->getCenterPosition();
            Vector3f endPos = position + currentDirection.scaleCopy(length);
            if (Maths::pointIsInCylinder(&playerPos, &position, &endPos, 9.0f))
            {
                playerIsHoldingOn = true;
                currentRotations = 0;
                angleChangeSpeed = fmaxf(rotateSpeed, Global::gameMainPlayer->vel.length());
                Global::gameMainPlayer->startGrabbing();
            }
        }
    }
    else
    {
        float circum = 2*length*Maths::PI;
        float distanceTraveled = angleChangeSpeed*dt;
        float rotationsTraveled = distanceTraveled/circum;
        currentRotations += rotationsTraveled;
        currentDirection = Maths::rotatePoint(&initialDirection, &rotationAxis, Maths::toRadians(360*currentRotations));

        Vector3f endPos = position + currentDirection.scaleCopy(length);

        Global::gameMainPlayer->position = endPos;
        Global::gameMainPlayer->vel = calcForwardDirection().scaleCopy(angleChangeSpeed);
        Global::gameMainPlayer->relativeUp = currentDirection.scaleCopy(-1);
        Global::gameMainPlayer->animate();
        Global::gameMainPlayer->refreshCamera();

        if (currentRotations > numRotationsUntilFinish)
        {
            playerIsHoldingOn = false;
            currentRotations = 0;
            currentDirection = initialDirection;
            Global::gameMainPlayer->stopGrabbing();
            Global::gameMainPlayer->vel = launchDirection.scaleCopy(launchSpeed);
            Global::gameMainPlayer->canMoveTimer = inputLockTime;
        }

        Maths::sphereAnglesFromPosition(&currentDirection, &rotY, &rotZ);
        updateTransformationMatrix(length, 1, 1);
    }
}

Vector3f GF_Vine::calcForwardDirection()
{
    Vector3f forward = rotationAxis.cross(&currentDirection);
    forward.normalize();
    return forward;
}

std::list<TexturedModel*>* GF_Vine::getModels()
{
    return &GF_Vine::models;
}

void GF_Vine::loadStaticModels()
{
    if (GF_Vine::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GF_Vine static models...\n");
    #endif

    ModelLoader::loadModel(&GF_Vine::models, "res/Models/Levels/GreenForest/Objects/", "Vine");
}

void GF_Vine::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GF_Vine static models...\n");
    #endif

    Entity::deleteModels(&GF_Vine::models);
}
