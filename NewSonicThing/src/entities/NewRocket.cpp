#include <glad/glad.h>

#include "./entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "rocket.h"
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

std::list<TexturedModel*> Rocket::modelsRocket;
std::list<TexturedModel*> Rocket::modelsBase;
CollisionModel* Rocket::cmBase;

extern float dt;

Rocket::Rocket()
{

}

Rocket::Rocket(int point1ID, int point2ID)
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

    position.y += 10; //move Rocket to be above the platform

    //for rotating the rocket to face the end, as well as for the actual movement
	rocketStartToEndDifference = pointPositionEnd - position;

    //calculate the actual rotation values using these position differences
    rotY = calculateRocketYRotation();
	rotZ = calculateRocketZRotation();
	base->setRotY(rotY); //make the base point the same direction

	rocketStartToEndDifferenceNormalized = rocketStartToEndDifference;
	rocketStartToEndDifferenceNormalized.normalize();

    rocketStartToEndDifferenceLength = rocketStartToEndDifference.length();

    //update transforms for both models and the collision since they were changed
    updateTransformationMatrix();
    cmBase->transformModel(collideModelTransformed, base->getPosition());
	base->updateTransformationMatrix();
}

void Rocket::step()
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

			playerToRocketDifference = playerPos - position;

			playerToRocketDifferenceHorizontalSquared = getPlayerToRocketDifferenceHorizontalSquared();

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

				setupPlayerVariables();

				if (!rocketStartedMoving()) //rocket is starting up
				{
					makeDirtParticles(PARTICLE_POSITION_OFFSET_ROCKET_STARTUP);

					setPlayerPositionToHoldRocketHandleStartup();

					startupTimer -= dt;
				}
				else //rocket is moving
				{
					makeDirtParticles(PARTICLE_POSITION_OFFSET_ROCKET_MOVING);

					calculateNewRocketPosition();

					setPlayerPositionToHoldRocketHandleMoving();

					percentOfPathCompleted += calculateNewPercentOfPathCompletedValue();
				}
			
			}

			updateTransformationMatrix();
			//update players transformation matrix here
        }
    }
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
}

//functions used for the constructor start here

Vector3f Rocket::getPointPosition(int pointID)
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

void Rocket::setupRocketBase()
{
    base = new Body(&Rocket::modelsBase);
	base->setVisible(true);
	INCR_NEW
	Main_addEntity(base);
	base->setPosition(&position);

	collideModelOriginal = Rocket::cmBase;
	collideModelTransformed = loadCollisionModel("Models/Objects/Rocket/", "RocketPlatformCollision");
	CollisionChecker::addCollideModel(collideModelTransformed);
	updateCollisionModel();
}

float Rocket::calculateRocketYRotation()
{
    return Maths::toDegrees(atan2(-rocketStartToEndDifference.z, rocketStartToEndDifference.x));
}

float Rocket::calculateRocketZRotation()
{
    return Maths::toDegrees(atan2(rocketStartToEndDifference.y, 
            sqrt(rocketStartToEndDifference.x*rocketStartToEndDifference.x +
            -rocketStartToEndDifference.z*-rocketStartToEndDifference.z)));
}

//functions used for step() start here
float Rocket::getPlayerToRocketDifferenceHorizontalSquared()
{
	return playerToRocketDifference.x*playerToRocketDifference.x + playerToRocketDifference.z*playerToRocketDifference.z;

}

bool Rocket::playerWithinAppearSoundRange()
{
	return (playerToRocketDifferenceHorizontalSquared <= pow(HITBOX_RADIUS * 30, 2)
			&& fabsf(playerToRocketDifference.y) < (HITBOX_HEIGHT * 10));
}

bool Rocket::playerOutsideAppearSoundResetRange()
{
	return (playerToRocketDifferenceHorizontalSquared >= pow(HITBOX_RADIUS * 150, 2)
			&& fabsf(playerToRocketDifference.y) < (HITBOX_HEIGHT * 50));
}

bool Rocket::playerWithinRocketHitbox()
{
	return (playerToRocketDifferenceHorizontalSquared <= pow(HITBOX_RADIUS, 2)
			&& fabsf(playerToRocketDifference.y) < HITBOX_HEIGHT);
}

void Rocket::makeDirtParticles(float particlePositionOffset)
{
	int dirtToMake = 5;
	while (dirtToMake > 0)
		{
			//put the particle in the right position
			Vector3f particlePosition(getX() + (rocketStartToEndDifferenceNormalized.x * particlePositionOffset),
						 getY() + (rocketStartToEndDifferenceNormalized.y * particlePositionOffset),
						 getZ() + (rocketStartToEndDifferenceNormalized.z * particlePositionOffset));

			//setup the particle velocities so they fly in different directions to make a ball
			Vector3f particleVelocity(rocketStartToEndDifferenceNormalized);
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

void Rocket::setupPlayerVariables()
{
	Global::gameMainVehicle->setVelocity(0, 0, 0);
	Global::gameMainVehicle->setOnRocket(true);
	Global::gameMainVehicle->setIsBall(false);
	Global::gameMainVehicle->setRotY(rotY);
	Global::gameMainVehicle->setCanMoveTimer(1000);
	Global::gameMainVehicle->setOnGround(false);
}

bool Rocket::rocketStartedMoving()
{
	return startupTimer <= 0;
}

void Rocket::setPlayerPositionToHoldRocketHandleStartup()
{
	Global::gameMainVehicle->setPosition(
			position.x - 8.75f*rocketStartToEndDifference.x,
			position.y - 0,
			position.z - 8.75f*rocketStartToEndDifference.z);

	if (rotZ > 70)
	{
		Global::gameMainVehicle->increasePosition(0, 2, 0);
	}
}

void Rocket::setPlayerPositionToHoldRocketHandleMoving()
{
	Global::gameMainVehicle->setPosition(
			position.x + ROCKET_SPEED*rocketStartToEndDifferenceNormalized.x - 8.75f*rocketStartToEndDifferenceNormalized.x,
			position.y - 0,
			position.z + ROCKET_SPEED*rocketStartToEndDifferenceNormalized.z - 8.75f*rocketStartToEndDifferenceNormalized.z);

	if (rotZ > 70)
	{
		Global::gameMainVehicle->increasePosition(0, 14, 0);
	}
}

void Rocket::calculateNewRocketPosition()
{
	position.x = pointPositionStart.x + (rocketStartToEndDifference.x * percentOfPathCompleted);
	position.y = pointPositionStart.y + (rocketStartToEndDifference.y * percentOfPathCompleted);
	position.z = pointPositionStart.z + (rocketStartToEndDifference.z * percentOfPathCompleted);
}

float Rocket::calculateNewPercentOfPathCompletedValue()
{
	return ROCKET_SPEED / rocketStartToEndDifferenceLength;
}