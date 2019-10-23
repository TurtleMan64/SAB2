#include "checkpoint.h"
#include "playersonic.h"
#include "camera.h"
#include "../engineTester/main.h"
#include "../models/models.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../toolbox/maths.h"

#include <algorithm>

std::list<TexturedModel*> Checkpoint::models;

Checkpoint::Checkpoint()
{

}

Checkpoint::Checkpoint(
		float rotationY, float x, float y, float z,
		float scaleX, float scaleY, float scaleZ,
		int number)
{
	position.x = x;
	position.y = y;
	position.z = z;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->scaleZ = scaleZ;
	ID = number;
	rotX = 0;
	rotY = rotationY;
	rotZ = 0;

	sinT = sinf(Maths::toRadians(rotY));
	cosT = cosf(Maths::toRadians(rotY));

	scale = 1;
	visible = false;
	updateTransformationMatrix(scaleX, scaleY, scaleZ);
}

void Checkpoint::step()
{
	
}

bool Checkpoint::isPointInsideMe(Vector3f* test)
{
	if (fabsf(test->y - getY()) <= scaleY)
	{
		//Rotate test coords by negative yrot to get in box coordinate system
		float xDiff = test->x - position.x;
		float zDiff = test->z - position.z;
		
		float newX = xDiff*cosT - zDiff*sinT;
		float newZ = zDiff*cosT + xDiff*sinT;

		if (fabsf(newX) <= scaleX &&
			fabsf(newZ) <= scaleZ)
		{
			return true;
		}
	}

	return false;
}

std::list<TexturedModel*>* Checkpoint::getModels()
{
	return &Checkpoint::models;
}

void Checkpoint::loadStaticModels()
{
	if (Checkpoint::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Checkpoint static models...\n");
	#endif

	loadModel(&Checkpoint::models, "res/Models/Misc/Box/", "Box");
}

void Checkpoint::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Checkpoint static models...\n");
	#endif

	Entity::deleteModels(&Checkpoint::models);
}
