#include <glad/glad.h>
#include <string>

#include "../../engineTester/main.h"
#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "RR_backgroundstars.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../car.h"
#include "../dummy.h"

#include <list>
#include <iostream>
#include <string>

std::list<TexturedModel*> RR_BackgroundStars::modelsPass1;
std::list<TexturedModel*> RR_BackgroundStars::modelsPass2;

RR_BackgroundStars::RR_BackgroundStars()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
	rotY = 0;
	visible = true;

	pass2 = new Dummy(&RR_BackgroundStars::modelsPass2); INCR_NEW
	pass2->setVisible(true);
	Main_addEntityPass2(pass2);
}

void RR_BackgroundStars::step()
{
	if (Global::gameMainVehicle != nullptr) 
	{
		setPosition(Global::gameMainVehicle->getPosition());
		pass2->setPosition(Global::gameMainVehicle->getPosition());
	}
	updateTransformationMatrix();
	pass2->updateTransformationMatrix();
}

std::list<TexturedModel*>* RR_BackgroundStars::getModels()
{
	return &RR_BackgroundStars::modelsPass1;
}

void RR_BackgroundStars::loadModels()
{
	if (RR_BackgroundStars::modelsPass1.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading RR_BackgroundStars models...\n");
	#endif

	loadModel(&RR_BackgroundStars::modelsPass1,  "res/Models/Tracks/RainbowRoadDS/SkySphere/", "Pass2");
	loadModel(&RR_BackgroundStars::modelsPass2,  "res/Models/Tracks/RainbowRoadDS/SkySphere/", "Pass3");
}

void RR_BackgroundStars::deleteModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting RR_BackgroundStars models...\n");
	#endif

	Entity::deleteModels(&RR_BackgroundStars::modelsPass1);
	Entity::deleteModels(&RR_BackgroundStars::modelsPass2);
}