#include <glad/glad.h>
#include <string>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "stage.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"

#include <list>
#include <iostream>
#include <string>

std::list<TexturedModel*> Stage::models;

Stage::Stage()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
	visible = true;
	updateTransformationMatrix();
}

void Stage::step()
{

}

std::list<TexturedModel*>* Stage::getModels()
{
	return &Stage::models;
}

void Stage::loadModels(char* folder, char* name)
{
	if (Stage::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading stage models...\n");
	#endif

	std::string path = "res/Models/";
	path = (path + folder) + "/";

	loadModel(&Stage::models, path, name);
}

void Stage::deleteModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting stage models...\n");
	#endif

	Entity::deleteModels(&Stage::models);
}

std::string Stage::getName()
{
	return "stage";
}