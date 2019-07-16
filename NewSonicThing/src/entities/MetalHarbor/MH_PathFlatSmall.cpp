#include "mhpathflatsmall.h"
#include "../entity.h"
#include "../camera.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"
#include "../../collision/collisionmodel.h"
#include "../../collision/collisionchecker.h"

#include <algorithm>

std::list<TexturedModel*> MH_PathFlatSmall::models;
CollisionModel* MH_PathFlatSmall::cmOriginal;

MH_PathFlatSmall::MH_PathFlatSmall()
{

}

MH_PathFlatSmall::MH_PathFlatSmall(float x, float y, float z, float rotY)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	this->rotY = rotY;
	rotZ = 0;

	scale = 1;
	visible = true;
	
	updateTransformationMatrix();

	collideModelOriginal = MH_PathFlatSmall::cmOriginal;
	collideModelTransformed = collideModelOriginal->duplicateMe();

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void MH_PathFlatSmall::step()
{
	
}

std::list<TexturedModel*>* MH_PathFlatSmall::getModels()
{
	return &MH_PathFlatSmall::models;
}

void MH_PathFlatSmall::loadStaticModels()
{
	if (MH_PathFlatSmall::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading MH_PathFlatSmall static models...\n");
	#endif

	loadModel(&MH_PathFlatSmall::models, "res/Models/Levels/MetalHarbor/", "PathFlatSmall");

	if (MH_PathFlatSmall::cmOriginal == nullptr)
	{
		MH_PathFlatSmall::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "PathFlatSmallCollision");
	}
}

void MH_PathFlatSmall::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting MH_PathFlatSmall static models...\n");
	#endif

	Entity::deleteModels(&MH_PathFlatSmall::models);
	Entity::deleteCollisionModel(&MH_PathFlatSmall::cmOriginal);
}
