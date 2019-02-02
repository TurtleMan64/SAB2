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


Rocket::Rocket()
{

}

Rocket::Rocket(int point1ID, int point2ID)
{
	extern std::list<Entity*> gameEntitiesToAdd;

	for (auto e : gameEntitiesToAdd)
	{
		if (e->isPoint())
		{
			Point* thisPoint = (Point*)e;
			if (thisPoint->getID() == point1ID)
			{
				pointPos1 = thisPoint->getPosition();
			}
			else if (thisPoint->getID() == point2ID)
			{
				pointPos2 = thisPoint->getPosition();
			}
		}
	}

	position = pointPos1;
	scale = 1;
	visible = true;
	//storedSonicSpeed = 0;
	updateTransformationMatrix();

	base = new Body(&Rocket::modelsBase);
	base->setVisible(true);
	INCR_NEW
	Main_addEntity(base);
	base->setPosition(&position);
	//fix slight position disparity from the position given with q
	base->setY(base->getY() - 1);

	collideModelOriginal = Rocket::cmBase;
	collideModelTransformed = loadCollisionModel("Models/Objects/SpeedRamp/", "SpeedRamp");
	//collideModelTransformed = loadCollisionModel("Models/Objects/Rocket/", "RocketPlatformCollision");
	CollisionChecker::addCollideModel(collideModelTransformed);
	updateCollisionModel();

	//rocket should be above the platform, now that the platform's position is set we can move it
	position.y += 10;

	timeOffset = 0;
	startupTimer = 0;

	canMove = true;
	isMoving = false;

	rocketAppearSoundPlayed = false;

	//used for rotation
	float xDiff =  (pointPos2.x - position.x);
	float yDiff =  (pointPos2.y - position.y);
	float zDiff = -(pointPos2.z - position.z);

	rotY = Maths::toDegrees(atan2(zDiff, xDiff));
	rotZ = Maths::toDegrees(atan2(yDiff, sqrt(xDiff*xDiff + zDiff*zDiff)));
	base->setRotY(rotY);
	
	pointDifference = pointPos2 - pointPos1;
	pointDifferenceNormalized = pointDifference;
	pointDifferenceNormalized.normalize();

	pointLength = pointDifference.length();

	cmBase->transformModel(collideModelTransformed, base->getPosition());
	base->updateTransformationMatrix();
    Global::gameMainVehicle->updateTransformationMatrix(); //fixes a bug where the player's position isn't synced with the rocket
}

void Rocket::step()
{
	if (abs(getX() - Global::gameCamera->eye.x) > ENTITY_RENDER_DIST)
	{
		setVisible(false);
		base->setVisible(false);
	}
	else
	{
		if (abs(getZ() - Global::gameCamera->eye.z) > ENTITY_RENDER_DIST)
		{
			setVisible(false);
			base->setVisible(false);
		}
		else
		{
			setVisible(true);
			base->setVisible(true);

			Vector3f* playerPos = Global::gameMainVehicle->getPosition();

			float xDiff    =       playerPos->x - position.x;
			float yDiffAbs = fabsf(playerPos->y - position.y);
			float zDiff    =       playerPos->z - position.z;
			float diffSquared = xDiff*xDiff + zDiff*zDiff;

			if (!rocketAppearSoundPlayed && 
				 diffSquared <= 22500    &&  //pow(radius * 30, 2) 
				 yDiffAbs    <  150)         //(height * 20 / 2)
			{
				rocketAppearSoundPlayed = true;
				rocketSource = AudioPlayer::play(54, getPosition(), 1, false);
			}

			if (!(diffSquared <= 562500 && //pow(radius * 150, 2)
				  yDiffAbs    <  750))     //(height * 100 / 2)
			{
				rocketAppearSoundPlayed = false;
				rocketSource = nullptr;
			}
			
			if (!isMoving && canMove && 
				diffSquared <= 45    &&  //pow(radius, 2)
				yDiffAbs    <  11.0f)
			{
				//start moving
				isMoving = true;
				canMove = false;

				rocketSource = AudioPlayer::play(55, getPosition(), 1, false);
			}

			if (timeOffset > 1 && isMoving) //stop moving
			{
				Global::gameMainVehicle->setVelocity(0, 0, 0);
				Global::gameMainVehicle->setOnRocket(false);
				Global::gameMainVehicle->setCanMoveTimer(0);

				position.set(&pointPos1);
				position.y += 10;
				canMove = true;
				isMoving = false;
				timeOffset = 0;
				startupTimer = 0;

				//if (rocketSource != nullptr) //stop the sound currently playing if there is one
				//{
					//rocketSource->stop();
				//}

				//rocketSource = AudioPlayer::play(56, getPosition(), 1, false);
				//rocketSource->setPosition(pointPos2.x, pointPos2.y, pointPos2.z);
			}

			if (isMoving)
			{
				if (rocketSource != nullptr)
				{
					if (!rocketSource->isPlaying())
					{
						rocketSource = AudioPlayer::play(56, getPosition(), 1, false);
					}
				}
				else
				{
					rocketSource = AudioPlayer::play(56, getPosition(), 1, false);
				}

				int dirtToMake = 5;
				float offset = 1;
				if (startupTimer >= 30)
				{
					offset = speed;
				}
				while (dirtToMake > 0)
				{
					
					Vector3f pos(getX() + (pointDifferenceNormalized.x * offset), getY() + (pointDifferenceNormalized.y * offset), getZ() + (pointDifferenceNormalized.z * offset));
					Vector3f vel(pointDifferenceNormalized);
					vel.scale(-3);
					vel.x +=   (Maths::random() - 0.5f);
					vel.y +=   (Maths::random() - 0.5f);
					vel.z +=   (Maths::random() - 0.5f);
					pos.x += 4*(Maths::random() - 0.5f);
					pos.y += 4*(Maths::random() - 0.5f);
					pos.z += 4*(Maths::random() - 0.5f);

					//new Particle(ParticleResources::textureDust, &pos, &vel, 0.08f, 60, 0, 4 * Maths::random() + 0.5f, 0, false);
					//new Particle(ParticleResources::textureDust, &pos, &vel, 0.08f, 60, 0, 4 * Maths::random() + 0.5f, 0, false);

					dirtToMake--;
				}

				Global::gameMainVehicle->setVelocity(0, 0, 0);
				Global::gameMainVehicle->setOnRocket(true);
				Global::gameMainVehicle->setIsBall(false);
				Global::gameMainVehicle->setRotY(rotY);
				Global::gameMainVehicle->setCanMoveTimer(1000);
				Global::gameMainVehicle->setOnGround(false);
				//Global::gameMainVehicle->setOnPlanePrevious(false);

				if (startupTimer < 30)
				{
					Global::gameMainVehicle->setPosition(
						position.x - 8.75f*pointDifferenceNormalized.x,
						position.y - 10.65f,
						position.z - 8.75f*pointDifferenceNormalized.z);

					if (rotZ > 70)
					{
						Global::gameMainVehicle->increasePosition(0, 2, 0);
					}

					startupTimer += 1;
				}
				else
				{
					position.x = pointPos1.x + (pointDifference.x * timeOffset);
					position.y = pointPos1.y + (pointDifference.y * timeOffset) + 10;
					position.z = pointPos1.z + (pointDifference.z * timeOffset);
					
					Global::gameMainVehicle->setPosition(
						position.x + speed*pointDifferenceNormalized.x - 8.75f*pointDifferenceNormalized.x,
						position.y - 9.75f,
						position.z + speed*pointDifferenceNormalized.z - 8.75f*pointDifferenceNormalized.z);

					if (rotZ > 70)
					{
						Global::gameMainVehicle->increasePosition(0, 14, 0);
					}

					timeOffset += speed / pointLength;
				}
			}

			updateTransformationMatrix();
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

	loadModel(&Rocket::modelsRocket, "Models/Objects/SpeedRamp/", "SpeedRamp");
	loadModel(&Rocket::modelsBase,   "Models/Objects/SpeedRamp/", "SpeedRamp");

	//loadModel(&Rocket::modelsRocket, "res/Models/Objects/Rocket/", "Rocket");
	//loadModel(&Rocket::modelsBase,   "res/Models/Objects/Rocket/", "RocketPlatform");

	if (Rocket::cmBase == nullptr)
	{
		//Rocket::cmBase = loadCollisionModel("Models/Objects/Rocket/", "RocketPlatformCollision");
		Rocket::cmBase = loadCollisionModel("Models/Objects/SpeedRamp/", "SpeedRamp");
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