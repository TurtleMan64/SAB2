#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "spikeball.hpp"
//#include "../renderEngine/renderEngine.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/playersonic.hpp"
#include "../toolbox/maths.hpp"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::vector<TexturedModel*> Spikeball::models;

Spikeball::Spikeball()
{

}

Spikeball::Spikeball(float x, float y, float z, float radius, float angVel)
{
	position.x = x;
	position.y = y;
	position.z = z;
    center = position;

	this->radius = radius;
	this->angVel = angVel;
	updateTransformationMatrix();
}

void Spikeball::step()
{
    extern float dt;
	rotY += angVel*dt;

	position.x = (center.x + radius*cosf(Maths::toRadians(rotY)));
	position.z = (center.z - radius*sinf(Maths::toRadians(rotY)));

    Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;

	if (diff.lengthSquared() < (13+5)*(13+5) && Global::gameMainPlayer->isVulnerable())
	{
		Global::gameMainPlayer->takeDamage(&position);
	}

	updateTransformationMatrix();
}

std::vector<TexturedModel*>* Spikeball::getModels()
{
	return &Spikeball::models;
}

void Spikeball::loadStaticModels()
{
	if (Spikeball::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	printf("Loading Spikeball static models...\n");
	#endif

	ModelLoader::loadModel(&Spikeball::models, "res/Models/Objects/Spikeball/", "Spikeball");
}

void Spikeball::deleteStaticModels()
{
	#ifdef DEV_MODE
	printf("Deleting Spikeball static models...\n");
	#endif

	Entity::deleteModels(&Spikeball::models);
}
