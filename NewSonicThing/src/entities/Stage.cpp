#include <glad/glad.h>
#include <string>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "stage.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "dummy.h"
#include "car.h"

#include <list>
#include <vector>
#include <iostream>
#include <string>

std::vector<std::list<TexturedModel*>> Stage::chunkModels;
std::vector<Dummy*> Stage::chunkDummies;
std::vector<std::vector<Vector3f>> Stage::chunkMin;
std::vector<std::vector<Vector3f>> Stage::chunkMax;

Stage::Stage()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
	visible = false;
}

void Stage::respawnChunks()
{
	Stage::chunkDummies.clear();
	for (int i = 0; i < Stage::chunkModels.size(); i++)
	{
		Dummy* chunk = new Dummy(&Stage::chunkModels[i]);
		chunk->setVisible(true);
		chunk->setPosition(0, 0, 0);
		chunk->updateTransformationMatrix();
		Main_addEntity(chunk);
		Stage::chunkDummies.push_back(chunk);
	}
}

void Stage::updateVisibleChunks()
{
	//go through and test bounds
	for (int i = 0; i < Stage::chunkDummies.size(); i++)
	{
		std::vector<Vector3f> mins = Stage::chunkMin[i];
		std::vector<Vector3f> maxs = Stage::chunkMax[i];

		Vector3f t(0, 0, 0);
		if (Global::gameMainVehicle != nullptr)
		{
			t.set(Global::gameMainVehicle->getPosition());
		}

		bool vis = false;

		for (int c = 0; c < mins.size(); c++)
		{
			Vector3f min = mins[c];
			Vector3f max = maxs[c];

			if (t.x > min.x && t.y > min.y && t.z > min.z &&
				t.x < max.x && t.y < max.y && t.z < max.z)
			{
				vis = true;
			}
		}

		Stage::chunkDummies[i]->setVisible(vis);
	}
}

void Stage::step()
{
	
}

std::list<TexturedModel*>* Stage::getModels()
{
	return nullptr;
}

void Stage::loadModels(
		const char* folder, 
		std::vector<std::string>* fnames, 
		std::vector<std::vector<Vector3f>>* mins,  
		std::vector<std::vector<Vector3f>>* maxs)
{
	if (Stage::chunkModels.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading stage models...\n");
	#endif

	std::string path = "res/Models/";
	path = (path + folder) + "/";

	for (int i = 0; i < fnames->size(); i++)
	{
		std::list<TexturedModel*> model;
		Stage::chunkModels.push_back(model);

		loadModel(&Stage::chunkModels[i], path, (*fnames)[i]);
		Stage::chunkMin.push_back((*mins)[i]);
		Stage::chunkMax.push_back((*maxs)[i]);
	}
}

void Stage::deleteModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting stage models...\n");
	#endif

	for (int i = 0; i < Stage::chunkModels.size(); i++)
	{
		std::list<TexturedModel*>* models = &Stage::chunkModels[i];
		Entity::deleteModels(models);
	}

	Stage::chunkModels.clear();
	Stage::chunkDummies.clear();
	Stage::chunkMin.clear();
	Stage::chunkMax.clear();
}

std::string Stage::getName()
{
	return "stage";
}
