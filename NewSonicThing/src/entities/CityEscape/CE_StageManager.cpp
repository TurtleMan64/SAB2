#include <glad/glad.h>

#include "../entity.h"
#include "cestagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../camera.h"

std::list<TexturedModel*> CE_StageManager::modelsSkydome;

CE_StageManager::CE_StageManager()
{
    scale = 1.5f;
	visible = true;
}

void CE_StageManager::step()
{
	position.set(&Global::gameMainPlayer->position);
	updateTransformationMatrix();
}

std::list<TexturedModel*>* CE_StageManager::getModels()
{
	return &CE_StageManager::modelsSkydome;
}

void CE_StageManager::loadStaticModels()
{
	if (CE_StageManager::modelsSkydome.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading CE_StageManager static models...\n");
	#endif

	loadModel(&CE_StageManager::modelsSkydome,  "res/Models/Levels/CityEscape/Sky/", "Sky");
}

void CE_StageManager::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting CE_StageManager static models...\n");
	#endif

	Entity::deleteModels(&CE_StageManager::modelsSkydome);
}
