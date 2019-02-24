#include "pulley.h"
#include "../entity.h"
#include "../camera.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../toolbox/maths.h"

#include <algorithm>

std::list<TexturedModel*> Pulley::models;

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

    ropeYAxisScale = 100;
	
	updateTransformationMatrix(1, ropeYAxisScale, 1);
}

void Pulley::step() 
{
    
}

std::list<TexturedModel*>* Pulley::getModels()
{
	return &Pulley::models;
}

void Pulley::loadStaticModels()
{
	if (Pulley::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Pulley static models...\n");
	#endif

	loadModel(&Pulley::models, "res/Models/Objects/Pulley/", "Pulley");
}

void Pulley::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Pulley static models...\n");
	#endif

	Entity::deleteModels(&Pulley::models);
}
