#include <glad/glad.h>

#include "../loading/modelloader.hpp"
#include "entity.hpp"
#include "../models/models.hpp"
#include "stagepass2.hpp"
#include "../engineTester/main.hpp"

#include <list>

std::vector<TexturedModel*> StagePass2::models;

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
    visible = true;
    
    if (StagePass2::models.size() == 0)
    {
        #ifdef DEV_MODE
        printf("Loading StagePass2 static models...\n");
        #endif

        ModelLoader::loadModel(&StagePass2::models, objFolder, objFilename);

        setModelsRenderOrder(&StagePass2::models, 1);
    }
    
    updateTransformationMatrix();
}

void StagePass2::step()
{

}

std::vector<TexturedModel*>* StagePass2::getModels()
{
    return &StagePass2::models;
}

void StagePass2::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting StagePass2 static models...\n");
    #endif

    Entity::deleteModels(&StagePass2::models);
}
