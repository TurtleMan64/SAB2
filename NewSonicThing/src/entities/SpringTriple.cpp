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

    hitCenter = position + dir.scaleCopy(HITBOX_DISTANCE_FROM_ORIGIN_HORIZONTAL);
    hitCenter.y += HITBOX_DISTANCE_FROM_ORIGIN_VERTICAL;

	Vector3f upVector = Vector3f(0,1,0);
	Vector3f dirSideways = dir;
	dirSideways = Maths::rotatePoint(&dir, &upVector, Maths::toRadians(90));
	hitRight = hitCenter + dirSideways.scaleCopy(SIDE_SPRING_OFFSET);
	hitLeft = hitCenter + dirSideways.scaleCopy(-SIDE_SPRING_OFFSET);

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

	if (fabsf(hitCenter.y - Global::gameMainPlayer->position.y) < 100 &&
		fabsf(hitCenter.z - Global::gameMainPlayer->position.z) < 100 &&
		fabsf(hitCenter.x - Global::gameMainPlayer->position.x) < 100 &&
		cooldownTimer == 0.0f)
	{
        if (collisionCheckCylinder(hitCenter, HITBOX_RADIUS, HITBOX_HEIGHT))
		{
            doSpringBounce(hitCenter);
        }
		else if (collisionCheckCylinder(hitRight, HITBOX_RADIUS, HITBOX_HEIGHT))
		{
			doSpringBounce(hitRight);
		}
		else if (collisionCheckCylinder(hitLeft, HITBOX_RADIUS, HITBOX_HEIGHT))
		{
			doSpringBounce(hitLeft);
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

inline bool SpringTriple::collisionCheckCylinder(Vector3f collisionCenterPos, float hitboxRadius, float hitboxHeight)
{
	Vector3f playerPos = Global::gameMainPlayer->position;
	Vector3f playerToCenterDistance = playerPos - collisionCenterPos;
	float playerToCenterDistanceSquared = playerToCenterDistance.x * playerToCenterDistance.x + playerToCenterDistance.z * playerToCenterDistance.z;
	if (playerToCenterDistanceSquared <= hitboxRadius * hitboxRadius && fabs(playerToCenterDistance.y) < hitboxHeight)
	{
		return true;
	}
	return false;
}

inline void SpringTriple::doSpringBounce(Vector3f hitboxPoint)
{
	Global::gameMainPlayer->position = hitboxPoint;
	Global::gameMainPlayer->hitSpringTriple(&dir, springPower, cooldownTimerMax);
	AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

	cooldownTimer = cooldownTimerMax;
}