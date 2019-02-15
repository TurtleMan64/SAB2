#include <glad/glad.h>

#include "./entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "rocketgeneral.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/car.h"
#include "../toolbox/maths.h"
#include "../animation/body.h"
#include "../entities/camera.h"
#include "./camera.h"
#include "./point.h"
#include "../collision/collisionmodel.h"
#include "../collision/collisionchecker.h"
#include "../audio/audioplayer.h"
#include "../audio/source.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

extern float dt;

std::list<TexturedModel*> RocketGeneral::modelsRocket;
std::list<TexturedModel*> RocketGeneral::modelsBase;
CollisionModel* RocketGeneral::cmBase;

void RocketGeneral::rocketConstructor(int point1ID, int point2ID)
{
    //get the positions of the start and end points
    pointPositionStart = getPointPosition(point1ID);
    pointPositionEnd = getPointPosition(point2ID);

    //set some variables to their initial values
    position = pointPositionStart;
    percentOfPathCompleted = 0;
	startupTimer = STARTUP_TIMER_INITIAL_VALUE;
	canActivate = true;
	isActive = false;
	rocketAppearSoundPlayed = false;
    visible = true;

    setupRocketBase();

    position.y += 10; //move RocketGeneral to be above the platform

    //for rotating the rocket to face the end, as well as for the actual movement
	rocketPathPositionDifference = pointPositionEnd - position;

    //calculate the actual rotation values using these position differences
    rotY = calculateRocketYRotation();
	rotZ = calculateRocketZRotation();
	base->setRotY(rotY); //make the base point the same direction

	rocketPathPositionDifferenceNormalized = rocketPathPositionDifference;
	rocketPathPositionDifferenceNormalized.normalize();

    rocketPathPositionDifferenceLength = rocketPathPositionDifference.length();

    //update transforms for both models and the collision since they were changed
    updateTransformationMatrix();
    cmBase->transformModel(collideModelTransformed, base->getPosition());
	base->updateTransformationMatrix();
}

void RocketGeneral::rocketStep()
{
    if (abs(getX() - Global::gameCamera->eye.x) > ENTITY_RENDER_DIST) //not within visible range on the x-axis
	{
		setVisible(false);
		base->setVisible(false);
	}
	else
	{
		if (abs(getZ() - Global::gameCamera->eye.z) > ENTITY_RENDER_DIST)//not within visible range on the z-axis
		{
			setVisible(false);
			base->setVisible(false);
		}
		else //is within visible range
		{
            setVisible(true);
            base->setVisible(true);

			//The players current position as of this frame
            Vector3f playerPos = Global::gameMainVehicle->getPosition();

			playerToRocketPositionDifference = playerPos - position;

			playerToRocketPositionDifferenceHorizontalSquared = getPlayerToRocketDifferenceHorizontalSquared();

			if (!rocketAppearSoundPlayed && playerWithinAppearSoundRange())
			{
				rocketAppearSoundPlayed = true;
				rocketAudioSource = AudioPlayer::play(54, getPosition(), 1, false);
			}
			else if (playerOutsideAppearSoundResetRange())
			{
				rocketAppearSoundPlayed = false;
				rocketAudioSource = nullptr;
			}

			if (!isActive && canActivate && playerWithinRocketHitbox())
			{
				//activate rocket
				isActive = true;
				canActivate = false;

				//sound of the rocket launching
				rocketAudioSource = AudioPlayer::play(55, getPosition(), 1, false);
			}

			if (isActive)
			{
				playRocketLaunchSound();

				setPlayerVariablesRocketActive();

				if (!rocketStartedMoving()) //rocket is starting up
				{
					makeDirtParticles(PARTICLE_POSITION_OFFSET_ROCKET_STARTUP);

					setPlayerPositionToHoldRocketHandle();

					startupTimer -= dt;
				}
				else //rocket is moving
				{
					makeDirtParticles(PARTICLE_POSITION_OFFSET_ROCKET_MOVING);

					calculateNewRocketPosition();

					setPlayerPositionToHoldRocketHandle();

					calculateNewPercentOfPathCompletedValue();	
				}	
			}

			if (fullPathTraveled())
			{
				//stop moving and deactivate rocket
				setPlayerVariablesRocketStopping();

				resetRocketVariables();
			}

			updateTransformationMatrix();
			Global::gameMainVehicle->animate();
        }
    }
}

std::list<TexturedModel*>* RocketGeneral::getModels()
{
	return &RocketGeneral::modelsRocket;
}

void RocketGeneral::loadStaticModels()
{
	if (RocketGeneral::modelsRocket.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Rocket static models...\n");
	#endif

	loadModel(&RocketGeneral::modelsRocket, "res/Models/Objects/Rocket/", "Rocket");
	loadModel(&RocketGeneral::modelsBase,   "res/Models/Objects/Rocket/", "RocketPlatform");

	if (RocketGeneral::cmBase == nullptr)
	{
		RocketGeneral::cmBase = loadCollisionModel("Models/Objects/Rocket/", "RocketPlatformCollision");
	}
}

void RocketGeneral::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Rocket static models...\n");
	#endif

	Entity::deleteModels(&RocketGeneral::modelsRocket);
	Entity::deleteModels(&RocketGeneral::modelsBase);
}

//functions used for the constructor start here

Vector3f RocketGeneral::getPointPosition(int pointID)
{
    Vector3f pointPos;
    extern std::list<Entity*> gameEntitiesToAdd;
	for (auto e : gameEntitiesToAdd)
	{
		if (e->isPoint())
		{
			Point* thisPoint = (Point*)e;
			if (thisPoint->getID() == pointID)
			{
				pointPos = thisPoint->getPosition();
                break;
			}
		}
	}
    return pointPos;
}

void RocketGeneral::setupRocketBase()
{
    base = new Body(&RocketGeneral::modelsBase);
	base->setVisible(true);
	INCR_NEW
	Main_addEntity(base);
	base->setPosition(&position);

	collideModelOriginal = RocketGeneral::cmBase;
	collideModelTransformed = loadCollisionModel("Models/Objects/RocketGeneral/", "RocketPlatformCollision");
	CollisionChecker::addCollideModel(collideModelTransformed);
	updateCollisionModel();
}

float RocketGeneral::calculateRocketYRotation()
{
    return Maths::toDegrees(atan2(-rocketPathPositionDifference.z, rocketPathPositionDifference.x));
}

float RocketGeneral::calculateRocketZRotation()
{
    return Maths::toDegrees(atan2(rocketPathPositionDifference.y, 
            sqrt(rocketPathPositionDifference.x*rocketPathPositionDifference.x +
            -rocketPathPositionDifference.z*-rocketPathPositionDifference.z)));
}

//functions used for step() start here
float RocketGeneral::getPlayerToRocketDifferenceHorizontalSquared()
{
	return playerToRocketPositionDifference.x*playerToRocketPositionDifference.x + playerToRocketPositionDifference.z*playerToRocketPositionDifference.z;

}

bool RocketGeneral::playerWithinAppearSoundRange()
{
	return (playerToRocketPositionDifferenceHorizontalSquared <= pow(HITBOX_RADIUS * 30, 2)
			&& fabsf(playerToRocketPositionDifference.y) < (HITBOX_HEIGHT * 10));
}

bool RocketGeneral::playerOutsideAppearSoundResetRange()
{
	return (playerToRocketPositionDifferenceHorizontalSquared >= pow(HITBOX_RADIUS * 150, 2)
			&& fabsf(playerToRocketPositionDifference.y) < (HITBOX_HEIGHT * 50));
}

bool RocketGeneral::playerWithinRocketHitbox()
{
	return (playerToRocketPositionDifferenceHorizontalSquared <= pow(HITBOX_RADIUS, 2)
			&& fabsf(playerToRocketPositionDifference.y) < HITBOX_HEIGHT);
}

void RocketGeneral::makeDirtParticles(float particlePositionOffset)
{
	int dirtToMake = 5;
	while (dirtToMake > 0)
		{
			//put the particle in the right position
			Vector3f particlePosition(getX() + (rocketPathPositionDifferenceNormalized.x * particlePositionOffset),
						 getY() + (rocketPathPositionDifferenceNormalized.y * particlePositionOffset),
						 getZ() + (rocketPathPositionDifferenceNormalized.z * particlePositionOffset));

			//setup the particle velocities so they fly in different directions to make a ball
			Vector3f particleVelocity(rocketPathPositionDifferenceNormalized);
			particleVelocity.scale(-3);
			particleVelocity.x +=   (Maths::random() - 0.5f);
			particleVelocity.y +=   (Maths::random() - 0.5f);
			particleVelocity.z +=   (Maths::random() - 0.5f);
			particlePosition.x += 4*(Maths::random() - 0.5f);
			particlePosition.y += 4*(Maths::random() - 0.5f);
			particlePosition.z += 4*(Maths::random() - 0.5f);

			//create the particle using these calculated values
			new Particle(ParticleResources::textureDust, &particlePosition, &particleVelocity, 0.08f, 60, 0, 4 * Maths::random() + 0.5f, 0, false, true);
			new Particle(ParticleResources::textureDust, &particlePosition, &particleVelocity, 0.08f, 60, 0, 4 * Maths::random() + 0.5f, 0, false, true);

			dirtToMake--;
		}
}

void RocketGeneral::playRocketLaunchSound()
{
	if (rocketAudioSource != nullptr)
	{
		if (!rocketAudioSource->isPlaying())
		{
			rocketAudioSource = AudioPlayer::play(56, getPosition(), 1, false);
		}
	}
	else
	{
		rocketAudioSource = AudioPlayer::play(56, getPosition(), 1, false);
	}
}

void RocketGeneral::setPlayerVariablesRocketActive()
{
	Global::gameMainVehicle->setVelocity(rocketPathPositionDifferenceNormalized.x * 1000, rocketPathPositionDifferenceNormalized.y * 1000, rocketPathPositionDifferenceNormalized.z * 1000);
	Global::gameMainVehicle->setOnRocket(true);
	Global::gameMainVehicle->setIsBall(false);
	Global::gameMainVehicle->setCanMoveTimer(1000);
	Global::gameMainVehicle->setOnGround(false);
}

bool RocketGeneral::rocketStartedMoving()
{
	return startupTimer <= 0;
}

void RocketGeneral::setPlayerPositionToHoldRocketHandle()
{
	Global::gameMainVehicle->setPosition(
			position.x - 6*rocketPathPositionDifferenceNormalized.x,
			position.y - 5,
			position.z - 6*rocketPathPositionDifferenceNormalized.z);
}

void RocketGeneral::calculateNewRocketPosition()
{
	position.x = pointPositionStart.x + (rocketPathPositionDifference.x * percentOfPathCompleted);
	position.y = (pointPositionStart.y + 10) + (rocketPathPositionDifference.y * percentOfPathCompleted);
	position.z = pointPositionStart.z + (rocketPathPositionDifference.z * percentOfPathCompleted);
}

void RocketGeneral::calculateNewPercentOfPathCompletedValue()
{
	percentOfPathCompleted += (ROCKET_SPEED * dt) / rocketPathPositionDifferenceLength;
}

bool RocketGeneral::fullPathTraveled()
{
	return (percentOfPathCompleted >= 1);
}

void RocketGeneral::setPlayerVariablesRocketStopping()
{
	Global::gameMainVehicle->setVelocity(rocketPathPositionDifferenceNormalized.x, rocketPathPositionDifferenceNormalized.y, rocketPathPositionDifferenceNormalized.z);
	Global::gameMainVehicle->setOnRocket(false);
	Global::gameMainVehicle->setCanMoveTimer(0);
}

void RocketGeneral::resetRocketVariables()
{
	position.set(&pointPositionStart);
	position.y += 10;
	canActivate = true;
	isActive = false;
	percentOfPathCompleted = 0;
	startupTimer = STARTUP_TIMER_INITIAL_VALUE;
}