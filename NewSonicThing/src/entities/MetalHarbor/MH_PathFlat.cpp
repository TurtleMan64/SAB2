#include "mhpathflat.h"
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

std::list<TexturedModel*> MH_PathFlat::models;
CollisionModel* MH_PathFlat::cmOriginal;

MH_PathFlat::MH_PathFlat()
{

}

MH_PathFlat::MH_PathFlat(float x, float y, float z, float rotY)
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

	collideModelOriginal = MH_PathFlat::cmOriginal;
	collideModelTransformed = collideModelOriginal->duplicateMe();

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void MH_PathFlat::step()
{
	
}

std::list<TexturedModel*>* MH_PathFlat::getModels()
{
	return &MH_PathFlat::models;
}

void MH_PathFlat::loadStaticModels()
{
	if (MH_PathFlat::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading MH_PathFlat static models...\n");
	#endif

	loadModel(&MH_PathFlat::models, "res/Models/Levels/MetalHarbor/", "PathFlat");

	if (MH_PathFlat::cmOriginal == nullptr)
	{
		MH_PathFlat::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "PathFlatCollision");
	}
}

void MH_PathFlat::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting MH_PathFlat static models...\n");
	#endif

	Entity::deleteModels(&MH_PathFlat::models);
	Entity::deleteCollisionModel(&MH_PathFlat::cmOriginal);
}
