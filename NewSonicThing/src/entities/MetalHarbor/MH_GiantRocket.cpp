#include "mhgiantrocket.h"
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

std::list<TexturedModel*> MH_GiantRocket::models;
CollisionModel* MH_GiantRocket::cmOriginal;

MH_GiantRocket::MH_GiantRocket()
{

}

MH_GiantRocket::MH_GiantRocket(float x, float y, float z)
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

	collideModelOriginal = MH_GiantRocket::cmOriginal;
	collideModelTransformed = loadCollisionModel("Models/Levels/MetalHarbor/", "GiantRocketRescaled");

	CollisionChecker::addCollideModel(collideModelTransformed);

	updateCollisionModel();
}

void MH_GiantRocket::step()
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

std::list<TexturedModel*>* MH_GiantRocket::getModels()
{
	return &MH_GiantRocket::models;
}

void MH_GiantRocket::loadStaticModels()
{
	if (MH_GiantRocket::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading MH_GiantRocket static models...\n");
	#endif

	loadModel(&MH_GiantRocket::models, "res/Models/Levels/MetalHarbor/", "GiantRocketRescaled");

	if (MH_GiantRocket::cmOriginal == nullptr)
	{
		MH_GiantRocket::cmOriginal = loadCollisionModel("Models/Levels/MetalHarbor/", "GiantRocketCollisionRescaled");
	}
}

void MH_GiantRocket::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting MH_GiantRocket static models...\n");
	#endif

	Entity::deleteModels(&MH_GiantRocket::models);
	Entity::deleteCollisionModel(&MH_GiantRocket::cmOriginal);
}
