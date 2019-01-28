#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "speedramp.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/car.h"
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

SpeedRamp::SpeedRamp(float x, float y, float z, float rotY, float rotZ, float myPower, int inputLockDuration)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->rotX = 0;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->scale = 1;
	this->visible = true;
	this->power = myPower * 60;
	this->inputLockDuration = inputLockDuration;
	updateTransformationMatrix();

	collideModelOriginal = SpeedRamp::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/Objects/SpeedRamp/", "SpeedRamp");

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModelWithZ();
}

void SpeedRamp::step()
{
	if (abs(getX() - Global::gameCamera->eye.x) > ENTITY_RENDER_DIST)
	{
		setVisible(false);
	}
	else
	{
		if (abs(getZ() - Global::gameCamera->eye.z) > ENTITY_RENDER_DIST)
		{
			setVisible(false);
		}
		else
		{
			setVisible(true);

			if (collideModelTransformed->playerIsOn)
			{
				//float dx =  power*cosf(toRadians(getRotY()));
				//float dz = -power*sinf(toRadians(getRotY()));
				//Global::gameMainVehicle->increaseGroundSpeed(dx, dz);

				Vector3f newSpeeds = Maths::spherePositionFromAngles(-Maths::toRadians(getRotY()), Maths::toRadians(getRotZ()+10), power);

				Global::gameMainVehicle->setOnGround(false);
				//Global::gameMainVehicle->setOnPlanePrevious(false);
				Global::gameMainVehicle->increasePosition(0, 4, 0);
                Global::gameMainVehicle->setVelocity(newSpeeds.x, newSpeeds.y, newSpeeds.z);
				//Global::gameMainVehicle->setxVelAir(newSpeeds.x);
				//Global::gameMainVehicle->setzVelAir(newSpeeds.z);
				//Global::gameMainVehicle->setyVel(newSpeeds.y);
				//Global::gameMainVehicle->setxVel(0);
				//Global::gameMainVehicle->setzVel(0);
				
                //Global::gameMainVehicle->setCanMoveTimer(inputLockDuration);
				//Global::gameMainVehicle->setGroundSpeed(0, 0);

				AudioPlayer::play(21, getPosition());
			}
		}
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

	loadModel(&SpeedRamp::models, "res/Models/Objects/SpeedRamp/", "SpeedRamp");

	if (SpeedRamp::cmOriginal == nullptr)
	{
		SpeedRamp::cmOriginal = loadCollisionModel("Models/Objects/SpeedRamp/", "SpeedRamp");
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