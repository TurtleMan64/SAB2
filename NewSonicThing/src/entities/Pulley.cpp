#include "pulley.h"
#include "entity.h"
#include "camera.h"
#include "../engineTester/main.h"
#include "../models/models.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../toolbox/maths.h"
#include "../collision/collisionmodel.h"
#include "../collision/collisionchecker.h"

#include <algorithm>

std::list<TexturedModel*> Pulley::modelsHandle;
std::list<TexturedModel*> Pulley::modelsRope;
std::list<TexturedModel*> Pulley::modelsTop;
CollisionModel* Pulley::cmTop;

Pulley::Pulley() {}

Pulley::Pulley(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	rotY = 0;
	rotZ = 0;

	scale = 1;
	visible = true;

    ropeYAxisScale = 1;
	
	updateTransformationMatrix(1, ropeYAxisScale, 1);
	
}

void Pulley::step() 
{
    
}

std::list<TexturedModel*>* Pulley::getModels()
{
	return &Pulley::modelsHandle;
}

void Pulley::loadStaticModels()
{
	if (Pulley::modelsHandle.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Pulley static models...\n");
	#endif

	loadModel(&Pulley::modelsHandle, "res/Models/Objects/Pulley/", "PulleyHandle");
	loadModel(&Pulley::modelsRope,   "res/Models/Objects/Pulley/", "PulleyRope");
	loadModel(&Pulley::modelsTop,    "res/Models/Objects/Pulley/", "PulleyTop");

	if (Pulley::cmTop == nullptr)
	{
		Pulley::cmTop = loadCollisionModel("Models/Objects/Pulley/", "PulleyTopCollision");
	}
}

void Pulley::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Pulley static models...\n");
	#endif

	Entity::deleteModels(&Pulley::modelsHandle);
	Entity::deleteModels(&Pulley::modelsRope);
	Entity::deleteModels(&Pulley::modelsTop);
}
