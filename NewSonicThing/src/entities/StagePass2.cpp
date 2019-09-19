#include <glad/glad.h>

#include "../objLoader/objLoader.h"
#include "entity.h"
#include "../models/models.h"
#include "stagepass2.h"
#include "../renderEngine/renderEngine.h"
#include "../engineTester/main.h"

#include <list>

std::list<TexturedModel*> StagePass2::models;

StagePass2::StagePass2()
{
	
}

StagePass2::StagePass2(const char* objFolder, const char* objFilename)
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	scale = 1;
    renderOrder = 1;
	visible = true;
	
	if (StagePass2::models.size() == 0)
	{
		#ifdef DEV_MODE
		std::fprintf(stdout, "Loading StagePass2 static models...\n");
		#endif

		loadModel(&StagePass2::models, objFolder, objFilename);
	}
	
	updateTransformationMatrix();
}

void StagePass2::step()
{

}

std::list<TexturedModel*>* StagePass2::getModels()
{
	return &StagePass2::models;
}

void StagePass2::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting StagePass2 static models...\n");
	#endif

	Entity::deleteModels(&StagePass2::models);
}
