#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "spring.h"
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

std::list<TexturedModel*> Spring::models;

Spring::Spring()
{

}

Spring::Spring(float x, float y, float z, float dirX, float dirY, float dirZ, float myPower, float cooldownMax, bool setsCam)
{
	position.x = x;
	position.y = y;
	position.z = z;
	dir.set(dirX, dirY, dirZ);
    dir.normalize();
	springPower = fmaxf(100.0f, myPower);
	cooldownTimer = 0.0f;
	cooldownTimerMax = fmaxf(0.1f, cooldownMax);
	scale = 1;
	visible = true;
    resetsCamera = setsCam;

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

	if (fabsf(hitCenter.y - Global::gameMainPlayer->position.y) < 40 &&
		fabsf(hitCenter.z - Global::gameMainPlayer->position.z) < 40 &&
		fabsf(hitCenter.x - Global::gameMainPlayer->position.x) < 40 &&
		cooldownTimer == 0.0f)
	{
        if ((Global::gameMainPlayer->getCenterPosition() - hitCenter).lengthSquared() < (10.83f*10.83f)+(4.0f*4.0f)) //10.83 = radius of spring, 4 = radius of sonic
		{
            Global::gameMainPlayer->position = hitCenter;
		    Global::gameMainPlayer->hitSpring(&dir, springPower, cooldownTimerMax, resetsCamera);
		    AudioPlayer::play(6, &position, 1 + (springPower*0.00013333f));

		    cooldownTimer = cooldownTimerMax;
        }
	}
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
