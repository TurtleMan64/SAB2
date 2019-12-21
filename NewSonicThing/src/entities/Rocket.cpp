#include <glad/glad.h>

#include "./entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "rocket.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "dummy.h"
#include "camera.h"
#include "./camera.h"
#include "./point.h"
#include "../collision/collisionmodel.h"
#include "../collision/collisionchecker.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particlemaster.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

extern float dt;

std::list<TexturedModel*> Rocket::modelsRocket;
std::list<TexturedModel*> Rocket::modelsBase;
CollisionModel* Rocket::cmBase;

Rocket::Rocket()
{

}

Rocket::Rocket(Vector3f* point1, Vector3f* point2)
{
    //set the positions of the start and end points
    pointPositionStart.set(point1);
    pointPositionEnd.set(point2);

    //set some variables to their initial values
    position = pointPositionStart;
    canActivate = true;
    isActive = false;
    percentOfPathCompleted = 0;
    startupTimer = STARTUP_TIMER_INITIAL_VALUE;

    rocketAppearSoundPlayed = false; //these are outside that function so it can also be used for the end of the rocket's launch
    visible = true;

    setupRocketBase();

    position.y += ROCKET_OFFSET_HEIGHT; //place the rocket above the base of the rocket

    //for rotating the rocket to face the end, as well as for the actual movement
    rocketPathPositionDifference = pointPositionEnd - position;

    rotY = calculateRocketYRotation();
    rotZ = calculateRocketZRotation();
    base->setRotY(rotY);

    rocketPathPositionDifferenceNormalized = rocketPathPositionDifference;
    rocketPathPositionDifferenceNormalized.normalize();

    rocketPathPositionDifferenceLength = rocketPathPositionDifference.length();

    //update transforms for both models and the collision since they were changed
    updateTransformationMatrix();
    cmBase->transformModel(collideModelTransformed, base->getPosition());
    base->updateTransformationMatrix();
}

void Rocket::step()
{
    //The players current position as of this frame
    Vector3f playerPos = Global::gameMainPlayer->getPosition();

    playerToRocketPositionDifference = playerPos - position;

    playerToRocketPositionDifferenceHorizontalSquared = getPlayerToRocketDifferenceHorizontalSquared();

    if (rocketAppearSoundCanPlay())
    {
        rocketAppearSoundPlayed = true;
        AudioPlayer::play(54, getPosition(), 1, false);
    }
    else if (rocketAppearSoundCanReset())
    {
        rocketAppearSoundPlayed = false;
    }

    if (!isActive && canActivate && playerWithinRocketHitbox())
    {
        //activate rocket
        isActive = true;
        canActivate = false;

        playRocketLaunchSoundStart();
    }

    if (isActive)
    {
        playRocketLaunchSoundLoop();

        //Rocket state disables player movement based on velocity, velocity here is set purely for camera reasons
        Global::gameMainPlayer->vel.set(
            rocketPathPositionDifferenceNormalized.x * ROCKET_SPEED, 
            rocketPathPositionDifferenceNormalized.y * ROCKET_SPEED, 
            rocketPathPositionDifferenceNormalized.z * ROCKET_SPEED);
        Global::gameMainPlayer->grabRocket();

        if (!rocketStartedMoving()) //rocket is starting up
        {
            //makeExhaustParticles(0.0f);

            setPlayerPositionToHoldRocketHandle();

            startupTimer -= dt;
        }
        else //rocket is moving
        {
            calculateNewRocketPosition();

            makeExhaustParticles(ROCKET_SPEED);

            setPlayerPositionToHoldRocketHandle();

            calculateNewPercentOfPathCompletedValue();    
        }    
    }

    if (fullPathTraveled()) //stop moving and deactivate rocket
    {
        //velocity here is set so the player faces the correct direction at the end of the rocket path
        Global::gameMainPlayer->vel.set(rocketPathPositionDifferenceNormalized.x, rocketPathPositionDifferenceNormalized.y, rocketPathPositionDifferenceNormalized.z);
        Global::gameMainPlayer->releaseRocket();

        resetRocketVariables();
    }

    updateTransformationMatrix();
    Global::gameMainPlayer->animate();
    Global::gameMainPlayer->refreshCamera();
}

std::list<TexturedModel*>* Rocket::getModels()
{
    return &Rocket::modelsRocket;
}

void Rocket::loadStaticModels()
{
    if (Rocket::modelsRocket.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading Rocket static models...\n");
    #endif

    loadModel(&Rocket::modelsRocket, "res/Models/Objects/Rocket/", "Rocket");
    loadModel(&Rocket::modelsBase,   "res/Models/Objects/Rocket/", "RocketPlatform");

    if (Rocket::cmBase == nullptr)
    {
        Rocket::cmBase = loadCollisionModel("Models/Objects/Rocket/", "RocketPlatformCollision");
    }
}

void Rocket::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting Rocket static models...\n");
    #endif

    Entity::deleteModels(&Rocket::modelsRocket);
    Entity::deleteModels(&Rocket::modelsBase);

    Entity::deleteCollisionModel(&Rocket::cmBase);
}

//functions used for the constructor start here

void Rocket::setupRocketBase()
{
    base = new Dummy(&Rocket::modelsBase); INCR_NEW("Entity");
    base->setVisible(true);
    Main_addEntity(base);
    base->setPosition(&position);

    collideModelOriginal = Rocket::cmBase;
    collideModelTransformed = Rocket::cmBase->duplicateMe();
    CollisionChecker::addCollideModel(collideModelTransformed);
    updateCollisionModel();
}

float Rocket::calculateRocketYRotation()
{
    return Maths::toDegrees(atan2(-rocketPathPositionDifference.z, rocketPathPositionDifference.x));
}

float Rocket::calculateRocketZRotation()
{
    return Maths::toDegrees(atan2(rocketPathPositionDifference.y, 
            sqrt(rocketPathPositionDifference.x*rocketPathPositionDifference.x +
            -rocketPathPositionDifference.z*-rocketPathPositionDifference.z)));
}

//functions used for step() start here
float Rocket::getPlayerToRocketDifferenceHorizontalSquared()
{
    return playerToRocketPositionDifference.x * playerToRocketPositionDifference.x + playerToRocketPositionDifference.z*playerToRocketPositionDifference.z;
}

bool Rocket::rocketAppearSoundCanPlay()
{
    return !rocketAppearSoundPlayed && (playerToRocketPositionDifferenceHorizontalSquared <= pow(HITBOX_RADIUS * 30, 2)
            && fabsf(playerToRocketPositionDifference.y) < (HITBOX_HEIGHT * 10));
}

bool Rocket::rocketAppearSoundCanReset()
{
    return (playerToRocketPositionDifferenceHorizontalSquared >= pow(HITBOX_RADIUS * 150, 2)
            && fabsf(playerToRocketPositionDifference.y) < (HITBOX_HEIGHT * 50));
}

bool Rocket::playerWithinRocketHitbox()
{
    return (playerToRocketPositionDifferenceHorizontalSquared <= pow(HITBOX_RADIUS, 2)
            && fabsf(playerToRocketPositionDifference.y) < HITBOX_HEIGHT);
}

void Rocket::makeExhaustParticles(float rocketSpeed)
{
    int dirtToMake = 5;
    while (dirtToMake > 0)
    {
        Vector3f down(0, -1, 0);
        Vector3f rotAxis = rocketPathPositionDifferenceNormalized.cross(&down);
        Vector3f up = Maths::rotatePoint(&rocketPathPositionDifferenceNormalized, &rotAxis, -Maths::PI/2);
        up.normalize();

        //put the particle in the right position
        Vector3f particlePosition = 
            position + 
            rocketPathPositionDifferenceNormalized.scaleCopy(PARTICLE_POSITION_OFFSET_FORWARD) +
            up.scaleCopy(PARTICLE_POSITION_OFFSET_UP);

        //setup the particle velocities so they fly in different directions to make a ball
        Vector3f particleVelocity(&rocketPathPositionDifferenceNormalized);
        particleVelocity.setLength(rocketSpeed);
        particleVelocity.scale(0.25f);
        particleVelocity.x +=   (Maths::random() - 0.5f);
        particleVelocity.y +=   (Maths::random() - 0.5f);
        particleVelocity.z +=   (Maths::random() - 0.5f);
        particlePosition.x += 4*(Maths::random() - 0.5f);
        particlePosition.y += 4*(Maths::random() - 0.5f);
        particlePosition.z += 4*(Maths::random() - 0.5f);

        //create the particle using these calculated values
        ParticleMaster::createParticle(ParticleResources::textureDust, &particlePosition, &particleVelocity, 0.0f, 0.1f, 0, 2*Maths::random() + 0.5f, 0, false, false, 1.0f, true);
        ParticleMaster::createParticle(ParticleResources::textureDust, &particlePosition, &particleVelocity, 0.0f, 0.1f, 0, 2*Maths::random() + 0.5f, 0, false, false, 1.0f, true);

        dirtToMake--;
    }
}

void Rocket::playRocketLaunchSoundStart()
{
    rocketAudioSource = AudioPlayer::play(55, getPosition(), 1, false);
}

void Rocket::playRocketLaunchSoundLoop()
{
    if (rocketAudioSource != nullptr)
    {
        if (!rocketAudioSource->isPlaying())
        {
            rocketAudioSource = AudioPlayer::play(56, getPosition(), 1, true);
        }
        rocketAudioSource->setPosition(position.x, position.y, position.z);
    }
    else
    {
        rocketAudioSource = AudioPlayer::play(56, getPosition(), 1, true);
    }
}

bool Rocket::rocketStartedMoving()
{
    return startupTimer <= 0;
}

void Rocket::setPlayerPositionToHoldRocketHandle()
{
    Global::gameMainPlayer->setPosition(&position);
}

void Rocket::calculateNewRocketPosition()
{
    position.x = pointPositionStart.x + (rocketPathPositionDifference.x * percentOfPathCompleted);
    position.y = (pointPositionStart.y + ROCKET_OFFSET_HEIGHT) + (rocketPathPositionDifference.y * percentOfPathCompleted);
    position.z = pointPositionStart.z + (rocketPathPositionDifference.z * percentOfPathCompleted);
}

void Rocket::calculateNewPercentOfPathCompletedValue()
{
    percentOfPathCompleted += (ROCKET_SPEED * dt) / rocketPathPositionDifferenceLength;
}

bool Rocket::fullPathTraveled()
{
    return (percentOfPathCompleted >= 1);
}

void Rocket::resetRocketVariables()
{
    position = pointPositionStart;
    position.y += ROCKET_OFFSET_HEIGHT;
    canActivate = true;
    isActive = false;
    percentOfPathCompleted = 0;
    startupTimer = STARTUP_TIMER_INITIAL_VALUE;
    if (rocketAudioSource != nullptr)
    {
        rocketAudioSource->setLooping(false);
        rocketAudioSource = nullptr;
    }
}
