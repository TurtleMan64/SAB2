//note: ported very poorly by NoThisIsStupider from SAB1

#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "dashpad.h"
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

std::list<TexturedModel*> Dashpad::models;

Dashpad::Dashpad()
{

}

Dashpad::Dashpad(float x, float y, float z, float rotX, float rotY, float rotZ, float myPower, float myCamYawTarget, int time)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->rotX = rotX;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->hitRadius = 6.5f;
	this->power = myPower * 40; //* 40 roughly gives the dashpads enough power, ideally this would just be done in the lvl file probably
	this->cooldownTimer = 0;
	this->cooldownTimerMax = time;
	this->scale = 1;
	this->visible = true;
	this->camYawTarget = myCamYawTarget;
	updateTransformationMatrixSADX();
}

void Dashpad::step()
{
	cooldownTimer = std::max(cooldownTimer - 1, 0);

	if (Global::gameMainVehicle->getX() > getX() - hitRadius - Global::gameMainVehicle->getHitboxHorizontal() && Global::gameMainVehicle->getX() < getX() + hitRadius + Global::gameMainVehicle->getHitboxHorizontal() &&
		Global::gameMainVehicle->getZ() > getZ() - hitRadius - Global::gameMainVehicle->getHitboxHorizontal() && Global::gameMainVehicle->getZ() < getZ() + hitRadius + Global::gameMainVehicle->getHitboxHorizontal() &&
		Global::gameMainVehicle->getY() > getY() - hitRadius - Global::gameMainVehicle->getHitboxVertical()   && Global::gameMainVehicle->getY() < getY() + hitRadius)
	{
		if (cooldownTimer == 0 && Global::gameMainVehicle->isOnGround())
		{
			float dx =  cosf(Maths::toRadians(getRotY()));
			float dz = -sinf(Maths::toRadians(getRotY()));

			float spindashPower = 0;

			Global::gameMainVehicle->setX(getX());
			Global::gameMainVehicle->setY(getY());
			Global::gameMainVehicle->setZ(getZ());

			Global::gameMainVehicle->setVelocity(0, 0, 0);
			Global::gameMainVehicle->setHoverTimer(0);
			//Global::gameMainVehicle->setCameraTargetYaw(-(camYawTarget)+90);

			AudioPlayer::play(1, getPosition());

			cooldownTimer = cooldownTimerMax;

			//Global::gameMainVehicle->setCanMoveTimer(cooldownTimerMax);

			if (Global::gameMainVehicle->isChargingSpindash())
			{
				//spindashPower = Global::gameMainVehicle->calculateSpindashSpeed(Global::gameMainVehicle->getSpindashTimer());
				Global::gameMainVehicle->setIsBall(true);
				Global::gameMainVehicle->setSpindashTimer(0);

				//TODO: get stored spindash speed and set if < that?
			}

			//Global::gameMainVehicle->setxVelAir(dx*power + dx*spindashPower);
			//Global::gameMainVehicle->setzVelAir(dz*power + dz*spindashPower);
			//Global::gameMainVehicle->setGroundSpeed(dx*power + dx*spindashPower, dz*power + dz*spindashPower);
			Global::gameMainVehicle->setVelocity(dx*power + dx*spindashPower, 0, dz*power + dz*spindashPower);
		}
	}
}

std::list<TexturedModel*>* Dashpad::getModels()
{
	return &Dashpad::models;
}

void Dashpad::loadStaticModels()
{
	if (Dashpad::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Dashpad static models...\n");
	#endif

	loadModel(&Dashpad::models, "res/Models/Objects/Dashpad/", "Dashpad");
}

void Dashpad::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Dashpad static models...\n");
	#endif

	Entity::deleteModels(&Dashpad::models);
}