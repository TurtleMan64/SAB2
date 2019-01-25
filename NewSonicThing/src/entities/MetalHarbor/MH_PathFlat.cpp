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

MH_PathFlat::MH_PathFlat(float x, float y, float z)
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

	collideModelOriginal = MH_PathFlat::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/Levels/MetalHarbor/", "PathFlatRescaled");

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void MH_PathFlat::step()
{
	if (abs(getX() - Global::gameCamera->eye.x) > ENTITY_RENDER_DIST)
	{
		setVisible(false);
	}
	else
	{
		if (abs(getZ() - Global::gameCamera->eye.z) > ENTITY_RENDER_DIST)
		{
			setVisible(false);
		}
		else
		{
			setVisible(true);
		}
	}
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

	loadModel(&MH_PathFlat::models, "res/Models/Levels/MetalHarbor/", "PathFlatRescaled");

	if (MH_PathFlat::cmOriginal == nullptr)
	{
		MH_PathFlat::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "PathFlatCollisionRescaled");
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
