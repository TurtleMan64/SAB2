#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "spring.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/car.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Spring::models;

extern float dt;

Spring::Spring()
{

}

Spring::Spring(float x, float y, float z, float rotY, float rotZ, float myPower, float cooldownMax)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->rotX = 0;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->springPower = myPower;
	this->cooldownTimer = 0;
	this->cooldownTimerInitialValue = cooldownMax;
	this->scale = 1;
	this->visible = true;

	updateTransformationMatrix();
}

void Spring::step()
{
	cooldownTimer -= dt;

	if (playerIsInRange() && cooldownTimer <= 0)
	{
		directionOfMovement = calculateDirectionOfMovement();

		setPlayerPositionToAboveSpring();
		setPlayerVelocityToSpringAway();

		Global::gameMainVehicle->setOnGround(false);
		Global::gameMainVehicle->setHoverTimer(0);
		Global::gameMainVehicle->setCanMoveTimer(cooldownTimerInitialValue);

		playSpringSound();

		cooldownTimer = cooldownTimerInitialValue;
	}

	increaseRotation(1, 0, 0); //this makes the spring turn
	updateTransformationMatrix();
}

float Spring::getSpringPower()
{
	return springPower;
}

std::list<TexturedModel*>* Spring::getModels()
{
	return &Spring::models;
}

void Spring::loadStaticModels()
{
	if (Spring::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading spring static models...\n");
	#endif

	loadModel(&Spring::models, "res/Models/Objects/Spring/", "Spring");
}

void Spring::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting spring static models...\n");
	#endif

	Entity::deleteModels(&Spring::models);
}

std::string Spring::getName()
{
	return "spring";
}

const bool Spring::canHomingAttackOn()
{
	return true;
}

const Vector3f Spring::getHomingCenter()
{
	return Vector3f(getPosition());
}

//functions for step() start here

bool Spring::playerIsInRange()
{
	return Global::gameMainVehicle->getX() > getX() - SPRING_RADIUS - Global::gameMainVehicle->getHitboxHorizontal() && Global::gameMainVehicle->getX() < getX() + SPRING_RADIUS + Global::gameMainVehicle->getHitboxHorizontal() &&
		Global::gameMainVehicle->getZ() > getZ() - SPRING_RADIUS - Global::gameMainVehicle->getHitboxHorizontal() && Global::gameMainVehicle->getZ() < getZ() + SPRING_RADIUS + Global::gameMainVehicle->getHitboxHorizontal() &&
		Global::gameMainVehicle->getY() > getY() - SPRING_RADIUS - Global::gameMainVehicle->getHitboxVertical()   && Global::gameMainVehicle->getY() < getY() + SPRING_RADIUS;
}

Vector3f Spring::calculateDirectionOfMovement()
{
	Vector3f directionOfMovementToReturn = Vector3f(0,0,0);
	directionOfMovementToReturn.x =  cosf(Maths::toRadians(getRotY()))*cosf(Maths::toRadians(getRotZ()));
	directionOfMovementToReturn.z = -sinf(Maths::toRadians(getRotY()))*cosf(Maths::toRadians(getRotZ()));
	directionOfMovementToReturn.y =  sinf(Maths::toRadians(getRotZ()));
	return directionOfMovementToReturn;
}

void Spring::setPlayerPositionToAboveSpring()
{
	Global::gameMainVehicle->setX(getX() + directionOfMovement.x * 15);
	Global::gameMainVehicle->setY(getY() + directionOfMovement.y * 15);
	Global::gameMainVehicle->setZ(getZ() + directionOfMovement.z * 15);
}

void Spring::setPlayerVelocityToSpringAway()
{
	Global::gameMainVehicle->setVelocity(directionOfMovement.x * springPower, directionOfMovement.y * springPower, directionOfMovement.z * springPower);
}

void Spring::playSpringSound()
{
	AudioPlayer::play(6, getPosition(), 1 + ((springPower/60)*0.008f));
}