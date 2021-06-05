#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "ghflower.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../dummy.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> GH_Flower::modelsBase;
std::list<TexturedModel*> GH_Flower::modelsTop;

GH_Flower::GH_Flower()
{
	
}

GH_Flower::GH_Flower(float x, float y, float z, float rotY, float scale, std::list<Entity*>* entityListToAdd)
{
	position.x = x;
	position.y = y;
	position.z = z;
	rotX = 0;
	this->rotY = rotY;
	rotZ = 0;
	this->scale = scale;
	visible = true;
	updateTransformationMatrix();

    petals = new Dummy(&GH_Flower::modelsTop); INCR_NEW("Entity");
    petals->setVisible(true);
    petals->setPosition(&position);
    petals->setRotY(rotY);
    petals->updateTransformationMatrix();
    entityListToAdd->push_back(petals);
}

void GH_Flower::step()
{
    extern float dt;

    float valPrev = fmod(timer, 5.66666666667f/2);

    timer += dt;

    float val = fmod(timer, 5.66666666667f/2);

	if (valPrev > val)
	{
		petals->setY(position.y);
		petals->updateTransformationMatrix();
	}
	else if ((val >= 2.0f/2 && valPrev < 2.0f/2) || 
             (val >= 5.0f/2 && valPrev < 5.0f/2))
	{
		petals->setY(position.y + 1.0f);
		petals->updateTransformationMatrix();
	}
	else if (val >= 2.66666666667f/2 && valPrev < 2.66666666667f/2)
	{
		petals->setY(position.y + 2.0f);
		petals->updateTransformationMatrix();
	}
}

std::list<TexturedModel*>* GH_Flower::getModels()
{
	return &GH_Flower::modelsBase;
}

void GH_Flower::loadStaticModels()
{
	if (GH_Flower::modelsBase.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading GH_Flower static models...\n");
	#endif

	ObjLoader::loadModel(&GH_Flower::modelsBase, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillPFlowerBase");
	ObjLoader::loadModel(&GH_Flower::modelsTop,  "res/Models/Levels/GreenHillZone/Objects/", "GreenHillPFlowerTop");
}

void GH_Flower::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting GH_Flower static models...\n");
	#endif

	Entity::deleteModels(&GH_Flower::modelsBase);
	Entity::deleteModels(&GH_Flower::modelsTop);
}
