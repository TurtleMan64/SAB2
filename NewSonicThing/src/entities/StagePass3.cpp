#include <glad/glad.h>

#include "../loading/modelloader.hpp"
#include "entity.hpp"
#include "../models/models.hpp"
#include "stagepass3.hpp"
#include "../engineTester/main.hpp"

#include <list>

std::list<TexturedModel*> StagePass3::models;

StagePass3::StagePass3()
{
    
}

StagePass3::StagePass3(const char* objFolder, const char* objFilename)
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 1;
    visible = true;
    
    if (StagePass3::models.size() == 0)
    {
        #ifdef DEV_MODE
        printf("Loading StagePass3 static models...\n");
        #endif

        ModelLoader::loadModel(&StagePass3::models, objFolder, objFilename);

        setModelsRenderOrder(&StagePass3::models, 2);
    }
    
    updateTransformationMatrix();
}

void StagePass3::step()
{

}

std::list<TexturedModel*>* StagePass3::getModels()
{
    return &StagePass3::models;
}

void StagePass3::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting StagePass3 static models...\n");
    #endif

    Entity::deleteModels(&StagePass3::models);
}
