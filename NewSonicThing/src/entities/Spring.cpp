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
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> Spring::models;

Spring::Spring()
{

}

Spring::Spring(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float cooldownMax)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->dir.set(dirX, dirY, dirZ);
    this->dir.normalize();
	this->springPower = fmaxf(100.0f, myPower);
	this->cooldownTimer = 0.0f;
	this->cooldownTimerMax = fmaxf(0.1f, cooldownMax);
	this->scale = 1;
	this->visible = true;

    hitCenter = position + dir.scaleCopy(10.0f);

    //calculate angles
    rotX = 0;
    rotY = Maths::toDegrees(atan2f(-dir.z, dir.x));
    rotZ = Maths::toDegrees(atan2f(dir.y, sqrtf(dir.x*dir.x + dir.z*dir.z)));
    rotRoll = 0;

	updateTransformationMatrix();
}

extern float dt;

void Spring::step()
{
	cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

	if (fabsf(hitCenter.y - Global::gameMainVehicle->position.y) < 40 &&
		fabsf(hitCenter.z - Global::gameMainVehicle->position.z) < 40 &&
		fabsf(hitCenter.x - Global::gameMainVehicle->position.x) < 40 &&
		cooldownTimer == 0.0f)
	{
        if ((Global::gameMainVehicle->getCenterPosition() - hitCenter).lengthSquared() < (10.83f*10.83f)+(4.0f*4.0f)) //10.83 = radius of spring, 4 = radius of sonic
		{
            Global::gameMainVehicle->position = hitCenter;
		    Global::gameMainVehicle->hitSpring(&dir, springPower, cooldownTimerMax);
		    AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

		    cooldownTimer = cooldownTimerMax;
        }
	}

	//increaseRotation(1, 0, 0);
	//updateTransformationMatrix();
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

const bool Spring::canHomingAttackOn()
{
	return true;
}

const Vector3f Spring::getHomingCenter()
{
	return hitCenter;
}
