#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "springtriple.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> SpringTriple::models;

SpringTriple::SpringTriple()
{

}

SpringTriple::SpringTriple(float x, float y, float z, float dirX, float dirZ, float myPower, float cooldownMax)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->dir.set(dirX, 0.0f, dirZ);
    this->dir.normalize();
	this->springPower = fmaxf(100.0f, myPower);
	this->cooldownTimer = 0.0f;
	this->cooldownTimerMax = fmaxf(0.1f, cooldownMax);
	this->scale = 1;
	this->visible = true;

    hitCenter = position + dir.scaleCopy(5.88928f);
    hitCenter.y += 5.29734f;

    //calculate angles
    rotX = 0;
    rotY = Maths::toDegrees(atan2f(-dir.z, dir.x));
    rotZ = 0;
    rotRoll = 0;

	updateTransformationMatrix();
}

extern float dt;

void SpringTriple::step()
{
	cooldownTimer = std::fmaxf(cooldownTimer - dt, 0.0f);

	if (fabsf(hitCenter.y - Global::gameMainPlayer->position.y) < 40 &&
		fabsf(hitCenter.z - Global::gameMainPlayer->position.z) < 40 &&
		fabsf(hitCenter.x - Global::gameMainPlayer->position.x) < 40 &&
		cooldownTimer == 0.0f)
	{
        if ((Global::gameMainPlayer->getCenterPosition() - hitCenter).lengthSquared() < (8.44f*8.44f)+(4.0f*4.0f)) //8.44 = radius of spring, 4 = radius of sonic
		{
            Global::gameMainPlayer->position = hitCenter;
		    Global::gameMainPlayer->hitSpringTriple(&dir, springPower, cooldownTimerMax);
		    AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

		    cooldownTimer = cooldownTimerMax;
        }
	}
}

std::list<TexturedModel*>* SpringTriple::getModels()
{
	return &SpringTriple::models;
}

void SpringTriple::loadStaticModels()
{
	if (SpringTriple::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading triple spring static models...\n");
	#endif

    loadModel(&SpringTriple::models, "res/Models/Objects/Spring/", "TripleSpring");
}

void SpringTriple::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting triple spring static models...\n");
	#endif

	Entity::deleteModels(&SpringTriple::models);
}

const bool SpringTriple::canHomingAttackOn()
{
	return true;
}

const Vector3f SpringTriple::getHomingCenter()
{
	return hitCenter;
}
