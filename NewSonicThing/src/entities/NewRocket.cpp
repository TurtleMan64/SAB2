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
    //get the positions of the start and end points
    pointPos1 = getPointPosition(point1ID);
    pointPos2 = getPointPosition(point2ID);

    //set some variables to their initial values
    position = pointPos1;
    timeOffset = 0;
	startupTimer = 0;
	canMove = true;
	isMoving = false;
	rocketAppearSoundPlayed = false;
    visible = true;

    setupRocketBase();

    setupCollisionModel();

    position.y += 10; //move Rocket to be above the platform

    //for rotating the rocket to face the end, as well as for the actual movement
    //the z axis also needs to be inverted for this to work properly
    rocketToEndDifference = pointPos2 - position;
	rocketToEndDifference.z = -(rocketToEndDifference.z);

    //calculate the actual rotation values using these position differences
    rotY = calculateRocketYRotation();
	rotZ = calculateRocketZRotation();
	base->setRotY(rotY); //make the base point the same direction

	rocketToEndDifferenceNormalized = rocketToEndDifference;
	rocketToEndDifferenceNormalized.normalize();

    rocketToEndDifferenceLength = rocketToEndDifference.length();

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

            //get the players current position and store it, just for convenience
            Vector3f* playerPos = Global::gameMainVehicle->getPosition();

            
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

//Returns the position of the point with the specified ID
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
    //fix slight position disparity from the position given with q
	//base->setY(base->getY() - 1);

}

void Rocket::setupCollisionModel()
{
    collideModelOriginal = Rocket::cmBase;
	collideModelTransformed = loadCollisionModel("Models/Objects/Rocket/", "RocketPlatformCollision");
	CollisionChecker::addCollideModel(collideModelTransformed);
	updateCollisionModel();
}

float Rocket::calculateRocketYRotation()
{
    return Maths::toDegrees(atan2(rocketToEndDifference.z, rocketToEndDifference.x));
}

float Rocket::calculateRocketZRotation()
{
    return Maths::toDegrees(atan2(rocketToEndDifference.y, 
            sqrt(rocketToEndDifference.x*rocketToEndDifference.x +
            rocketToEndDifference.z*rocketToEndDifference.z)));
}

//functions used for step() start here