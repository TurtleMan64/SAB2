#include <glad/glad.h>

#include "../entity.h"
#include "mhstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"

std::list<TexturedModel*> MH_StageManager::modelsSkydome;

MH_StageManager::MH_StageManager()
{
	visible = true;
    scale = 2;
}

void MH_StageManager::step()
{
	//set the position of the background sky dome
	position.y = Global::waterHeight;
	position.x = Global::gameMainPlayer->getX();
	position.z = Global::gameMainPlayer->getZ();
	updateTransformationMatrix();
}

std::list<TexturedModel*>* MH_StageManager::getModels()
{
	return &MH_StageManager::modelsSkydome;
}

void MH_StageManager::loadStaticModels()
{
	if (MH_StageManager::modelsSkydome.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading MH_StageManager static models...\n");
	#endif

	loadModel(&MH_StageManager::modelsSkydome,  "res/Models/Levels/MetalHarbor/Sky/", "SkySpheroid");
}

void MH_StageManager::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting MH_StageManager static models...\n");
	#endif

	Entity::deleteModels(&MH_StageManager::modelsSkydome);
}
