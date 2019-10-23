#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "speedramp.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
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

SpeedRamp::SpeedRamp(float x, float y, float z, float rotY, float rotZ, float speedRampPower, float inputLockDuration)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->rotX = 0;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->scale = 1;
	this->visible = true;
	this->speedRampPower = speedRampPower;
	this->inputLockDuration = inputLockDuration;
	updateTransformationMatrix();

	setupCollisionModel();

	updateCollisionModelWithZ();
}

void SpeedRamp::step()
{
	if (collideModelTransformed->playerIsOn)
	{
		Vector3f newVelocity = calculateNewVelocity();

		Global::gameMainPlayer->increasePosition(0, 4, 0);
		Global::gameMainPlayer->vel.set(newVelocity.x, newVelocity.y, newVelocity.z);
		Global::gameMainPlayer->setCanMoveTimer(inputLockDuration);

		playSpeedRampSound();
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

//functions for the constructor start here

void SpeedRamp::setupCollisionModel()
{
	collideModelOriginal = SpeedRamp::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/Objects/SpeedRamp/", "SpeedRamp");
	CollisionChecker::addCollideModel(collideModelTransformed);
}

//functions for step() start here
		
Vector3f SpeedRamp::calculateNewVelocity()
{
	Vector3f newVelocity = Maths::spherePositionFromAngles(-Maths::toRadians(getRotY()), Maths::toRadians(getRotZ()+10), speedRampPower);
	return newVelocity;
}

void SpeedRamp::playSpeedRampSound()
{
	AudioPlayer::play(21, getPosition());
}