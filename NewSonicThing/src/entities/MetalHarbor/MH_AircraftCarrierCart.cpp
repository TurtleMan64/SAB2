#include "mhaircraftcarriercart.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../controllableplayer.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../dummy.hpp"

extern float dt;

std::vector<TexturedModel*> MH_AircraftCarrierCart::models;
std::vector<TexturedModel*> MH_AircraftCarrierCart::modelsWheelFront;
std::vector<TexturedModel*> MH_AircraftCarrierCart::modelsWheelBack;
std::vector<TexturedModel*> MH_AircraftCarrierCart::modelsTransparent;
CollisionModel* MH_AircraftCarrierCart::cmOriginal;

MH_AircraftCarrierCart::MH_AircraftCarrierCart() {}

MH_AircraftCarrierCart::MH_AircraftCarrierCart(float x, float y, float z, float dirX, float dirZ, float displacementMax, float speed)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotZ = 0;

    scale = 1;
    visible = true;

    platformMoves = true;
    this->speed = speed;
    this->displacementMax = displacementMax;
    displacementCurrent = 0;
    wheelMovementDirectionMultiplier = 1;
    positionInitial = &position;

    directionVector.set(dirX, 0.0f, dirZ);
    directionVector.normalize();

    rotY = Maths::toDegrees(atan2f(directionVector.x, directionVector.z));

    if (displacementMax == 0)
    {
        platformMoves = false;
    }

    stateCurrent = MOVING_FORWARD;

    setupModelWheelFront();
    setupModelWheelBack();
    setupModelTransparent();

    syncBodyPositionsAbsolute();

    updateTransformationMatrix();
    wheelFront->updateTransformationMatrix();
    wheelBack->updateTransformationMatrix();
    bodyTransparent->updateTransformationMatrix();

    collideModelOriginal = MH_AircraftCarrierCart::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void MH_AircraftCarrierCart::step() 
{
    Vector3f playerPos = Global::gameMainPlayer->position;
    Vector3f playerDistanceSquared = (playerPos - position) * (playerPos - position);
    if (platformMoves && playerDistanceSquared.x <= MAX_UPDATE_DISTANCE_SQUARED
                      && playerDistanceSquared.y <= MAX_UPDATE_DISTANCE_SQUARED
                      && playerDistanceSquared.z <= MAX_UPDATE_DISTANCE_SQUARED)
    {    
        Vector3f movementAmount;
        switch (stateCurrent)
        {
            case MOVING_FORWARD:
                movementAmount = calculateMovementAmount(directionVector);
                position = position + movementAmount;
                displacementCurrent += movementAmount.length();
                syncBodyPositionsRelative(movementAmount);
                spinWheels();

                pushSonicAway(true, false);

                if (collideModelTransformed->playerIsOn)
                {
                    movePlayer(movementAmount);
                }
                
                if (displacementCurrent >= displacementMax)
                {
                    displacementCurrent = displacementMax;
                    wheelMovementDirectionMultiplier *= -1;
                    
                    stateCurrent = STOPPED;
                    positionStopped = position;
                    stoppedTimer = 0;
                    shakeTimer = SHAKE_TIMER_MIN;
                }
                break;                
            case MOVING_BACKWARDS:
                movementAmount = calculateMovementAmount(Vector3f() - directionVector);
                position = position + movementAmount;
                displacementCurrent -= movementAmount.length();
                syncBodyPositionsRelative(movementAmount);
                spinWheels();

                pushSonicAway(false, true);

                if (collideModelTransformed->playerIsOn)
                {
                    movePlayer(movementAmount);
                }

                if (displacementCurrent <= 0)
                {
                    displacementCurrent = 0;
                    position = positionInitial;
                    syncBodyPositionsAbsolute();
                    wheelMovementDirectionMultiplier *= -1;
                    
                    stateCurrent = STOPPED;
                    positionStopped = position;
                    stoppedTimer = 0;
                    shakeTimer = SHAKE_TIMER_MIN;
                }
                break;
            case STOPPED:
                stoppedTimer += dt;

                movementAmount = shakePlatform();
                position = position + movementAmount;
                syncBodyPositionsRelative(movementAmount);

                if (shakeTimer < SHAKE_TIMER_MAX && sinf(shakeTimer)/shakeTimer * 5 > 0)
                {
                    pushSonicAway(true, false);
                }
                else if (shakeTimer < SHAKE_TIMER_MAX && sinf(shakeTimer)/shakeTimer * 5 < 0)
                {
                    pushSonicAway(false, true);
                }

                if (collideModelTransformed->playerIsOn)
                {
                    movePlayer(movementAmount);
                }

                if (stoppedTimer > 2)
                {
                    if (displacementCurrent == displacementMax)
                    {
                        stateCurrent = MOVING_BACKWARDS;
                    }
                    else
                    {
                        stateCurrent = MOVING_FORWARD;
                    }
                }
                break;
        }

        updateTransformationMatrix();
        wheelFront->updateTransformationMatrix();
        wheelBack->updateTransformationMatrix();
        bodyTransparent->updateTransformationMatrix();
        updateCollisionModel();

        Global::gameMainPlayer->animate();
        Global::gameMainPlayer->refreshCamera();
    }
}

std::vector<TexturedModel*>* MH_AircraftCarrierCart::getModels()
{
    return &MH_AircraftCarrierCart::models;
}

void MH_AircraftCarrierCart::loadStaticModels()
{
    if (MH_AircraftCarrierCart::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading MH_AircraftCarrierCart static models...\n");
    #endif

    ModelLoader::loadModel(&MH_AircraftCarrierCart::models, "res/Models/Objects/MetalHarbor/AircraftCarrierCart/", "AircraftCarrierCart");
    ModelLoader::loadModel(&MH_AircraftCarrierCart::modelsWheelFront, "res/Models/Objects/MetalHarbor/AircraftCarrierCart/", "AircraftCarrierCartWheelFront");
    ModelLoader::loadModel(&MH_AircraftCarrierCart::modelsWheelBack, "res/Models/Objects/MetalHarbor/AircraftCarrierCart/", "AircraftCarrierCartWheelBack");
    ModelLoader::loadModel(&MH_AircraftCarrierCart::modelsTransparent, "res/Models/Objects/MetalHarbor/AircraftCarrierCart/", "AircraftCarrierCartTransparent");

    setModelsRenderOrder(&MH_AircraftCarrierCart::modelsWheelFront,  1);
    setModelsRenderOrder(&MH_AircraftCarrierCart::modelsWheelBack,   1);
    setModelsRenderOrder(&MH_AircraftCarrierCart::modelsTransparent, 1);

    if (MH_AircraftCarrierCart::cmOriginal == nullptr)
    {
        MH_AircraftCarrierCart::cmOriginal = ModelLoader::loadCollisionModel("Models/Objects/MetalHarbor/AircraftCarrierCart/", "AircraftCarrierCartCollision");
    }
}

void MH_AircraftCarrierCart::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting MH_AircraftCarrierCart static models...\n");
    #endif

    Entity::deleteModels(&MH_AircraftCarrierCart::models);
    Entity::deleteModels(&MH_AircraftCarrierCart::modelsWheelFront);
    Entity::deleteModels(&MH_AircraftCarrierCart::modelsWheelBack);
    Entity::deleteModels(&MH_AircraftCarrierCart::modelsTransparent);
    Entity::deleteCollisionModel(&MH_AircraftCarrierCart::cmOriginal);
}

inline void MH_AircraftCarrierCart::setupModelWheelFront()
{
    wheelFront = new Dummy(&MH_AircraftCarrierCart::modelsWheelFront); INCR_NEW("Entity");
    wheelFront->visible = (true);
    Global::addEntity(wheelFront);
    wheelFront->setPosition(&position);
    wheelFront->rotY = (rotY);
}

inline void MH_AircraftCarrierCart::setupModelWheelBack()
{
    wheelBack = new Dummy(&MH_AircraftCarrierCart::modelsWheelBack); INCR_NEW("Entity");
    wheelBack->visible = (true);
    Global::addEntity(wheelBack);

    wheelBack->setPosition(&position);
    wheelBack->rotY = (rotY);
}

inline void MH_AircraftCarrierCart::setupModelTransparent()
{
    bodyTransparent = new Dummy(&MH_AircraftCarrierCart::modelsTransparent); INCR_NEW("Entity");
    bodyTransparent->visible = (true);
    Global::addEntity(bodyTransparent);
    bodyTransparent->rotY = (rotY);
}

inline Vector3f MH_AircraftCarrierCart::calculateMovementAmount(Vector3f directionVectorLocal)
{
    return directionVectorLocal.scaleCopy(speed * dt);
}

inline void MH_AircraftCarrierCart::syncBodyPositionsRelative(Vector3f movementAmount)
{
    bodyTransparent->position = position;
    
    wheelFront->position = wheelFront->position + movementAmount;
    wheelBack->position = wheelBack->position + movementAmount;
}

inline void MH_AircraftCarrierCart::syncBodyPositionsAbsolute()
{
    bodyTransparent->position = position;

    wheelFront->position = position + directionVector.scaleCopy(WHEEL_OFFSET_FRONT_HORIZONTAL);
    wheelFront->position.y += WHEEL_OFFSET_FRONT_VERTICAL;

    wheelBack->position = position + directionVector.scaleCopy(WHEEL_OFFSET_BACK_HORIZONTAL);
    wheelBack->position.y += WHEEL_OFFSET_BACK_VERTICAL;
}

inline void MH_AircraftCarrierCart::spinWheels()
{
    wheelFront->rotX += WHEEL_SPEED_FRONT * wheelMovementDirectionMultiplier * speed/40*60 * dt;
    wheelBack->rotX += WHEEL_SPEED_BACK * wheelMovementDirectionMultiplier * speed/40*60 * dt;
}

inline void MH_AircraftCarrierCart::movePlayer(Vector3f movementAmount)
{
    Vector3f newPlayerPos = movementAmount + Global::gameMainPlayer->getPosition();
    Global::gameMainPlayer->position = newPlayerPos;
}

inline Vector3f MH_AircraftCarrierCart::shakePlatform()
{
    Vector3f distanceFromPositionStopped = positionStopped - position;

    if (shakeTimer > SHAKE_TIMER_MAX)
    {
        return distanceFromPositionStopped;
    }
    shakeTimer += dt * 30;
    return directionVector.scaleCopy(sinf(shakeTimer)/shakeTimer * 5) + distanceFromPositionStopped;
}

inline void MH_AircraftCarrierCart::pushSonicAway(bool /*frontHitboxes*/, bool /*backHitboxes*/)
{
    //first check if sonic is even close to the platform
    if (collisionCheckCylinder(position, 100, 100))
    {
        Vector3f sidewaysVector = Maths::rotatePoint(&directionVector, &Y_AXIS, Maths::toRadians(90));
        Vector3f collisionCenterPos1;
        Vector3f collisionCenterPos2;
        Vector3f collisionCenterPos3;
        Vector3f collisionCenterPos4;

        //collisions on front of platform
        collisionCenterPos1 = position + directionVector.scaleCopy(COLLISION_POSITION_FORWARD) + sidewaysVector.scaleCopy(COLLISION_POSITION_SIdEWAYS);
        collisionCenterPos2 = position + directionVector.scaleCopy(COLLISION_POSITION_FORWARD);
        collisionCenterPos3 = position + directionVector.scaleCopy(COLLISION_POSITION_FORWARD) + sidewaysVector.scaleCopy(COLLISION_POSITION_SIdEWAYS * -1);
        if (collisionCheckCylinder(collisionCenterPos1, COLLISION_RADIUS, COLLISION_HEIGHT) || collisionCheckCylinder(collisionCenterPos2, COLLISION_RADIUS, COLLISION_HEIGHT) || collisionCheckCylinder(collisionCenterPos3, COLLISION_RADIUS, COLLISION_HEIGHT))
        {
            Global::gameMainPlayer->increasePosition(directionVector.x * speed/30, directionVector.y * speed/30, directionVector.z * speed/30);
        }

        //collisions on back of platform
        collisionCenterPos1 = position + directionVector.scaleCopy(COLLISION_POSITION_BACKWARD_OUTER) + sidewaysVector.scaleCopy(COLLISION_POSITION_SIdEWAYS);
        collisionCenterPos2 = position + directionVector.scaleCopy(COLLISION_POSITION_BACKWARD_INNER) + sidewaysVector.scaleCopy(COLLISION_POSITION_BACKSIdEWAYS_INNER);
        collisionCenterPos3 = position + directionVector.scaleCopy(COLLISION_POSITION_BACKWARD_INNER) + sidewaysVector.scaleCopy(COLLISION_POSITION_BACKSIdEWAYS_INNER * -1);
        collisionCenterPos4 = position + directionVector.scaleCopy(COLLISION_POSITION_BACKWARD_OUTER) + sidewaysVector.scaleCopy(COLLISION_POSITION_SIdEWAYS * -1);
        if (collisionCheckCylinder(collisionCenterPos1, COLLISION_RADIUS, COLLISION_HEIGHT) || collisionCheckCylinder(collisionCenterPos2, COLLISION_RADIUS, COLLISION_HEIGHT) || collisionCheckCylinder(collisionCenterPos3, COLLISION_RADIUS, COLLISION_HEIGHT) || collisionCheckCylinder(collisionCenterPos4, COLLISION_RADIUS, COLLISION_HEIGHT))
        {
            Global::gameMainPlayer->increasePosition(directionVector.x * -speed/30, directionVector.y * -speed/30, directionVector.z * -speed/30);
        }
    }    
}

inline bool MH_AircraftCarrierCart::collisionCheckCylinder(Vector3f collisionCenterPos, float hitboxRadius, float hitboxHeight)
{
    Vector3f playerPos = Global::gameMainPlayer->position;
    Vector3f playerToCenterDistance = playerPos - collisionCenterPos;
    float playerToCenterDistanceSquared = playerToCenterDistance.x * playerToCenterDistance.x + playerToCenterDistance.z * playerToCenterDistance.z;
    if (playerToCenterDistanceSquared <= hitboxRadius * hitboxRadius && fabs(playerToCenterDistance.y) < hitboxHeight)
    {
        return true;
    }
    return false;
}
