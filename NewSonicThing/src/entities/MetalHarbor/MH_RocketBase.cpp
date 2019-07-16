#include "mhrocketbase.h"
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

std::list<TexturedModel*> MH_RocketBase::models;
CollisionModel* MH_RocketBase::cmOriginal;

MH_RocketBase::MH_RocketBase()
{

}

MH_RocketBase::MH_RocketBase(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	rotY = 0;
	rotZ = 0;

	scale = 1;
	visible = true;
	
	updateTransformationMatrix();

	collideModelOriginal = MH_RocketBase::cmOriginal;
	collideModelTransformed = collideModelOriginal->duplicateMe();

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void MH_RocketBase::step()
{
	
}

std::list<TexturedModel*>* MH_RocketBase::getModels()
{
	return &MH_RocketBase::models;
}

void MH_RocketBase::loadStaticModels()
{
	if (MH_RocketBase::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading MH_RocketBase static models...\n");
	#endif

	loadModel(&MH_RocketBase::models, "res/Models/Levels/MetalHarbor/", "RocketBase");

	if (MH_RocketBase::cmOriginal == nullptr)
	{
		MH_RocketBase::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "RocketBase");
	}
}

void MH_RocketBase::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting MH_RocketBase static models...\n");
	#endif

	Entity::deleteModels(&MH_RocketBase::models);
	Entity::deleteCollisionModel(&MH_RocketBase::cmOriginal);
}
