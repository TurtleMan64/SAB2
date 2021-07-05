#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "dpbigshine.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> DP_BigShine::models;

DP_BigShine::DP_BigShine()
{
    
}

DP_BigShine::DP_BigShine(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 0.55f;
    visible = true;
    updateTransformationMatrix();
}

void DP_BigShine::step()
{
    extern float dt;

    rotY += dt*60;

    updateTransformationMatrix();
}

std::list<TexturedModel*>* DP_BigShine::getModels()
{
    return &DP_BigShine::models;
}

void DP_BigShine::loadStaticModels()
{
    if (DP_BigShine::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading DP_BigShine static models...\n");
    #endif

    ObjLoader::loadModel(&DP_BigShine::models, "res/Models/Levels/DelfinoPlaza/BigShine/", "BigShine");
}

void DP_BigShine::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting DP_BigShine static models...\n");
    #endif

    Entity::deleteModels(&DP_BigShine::models);
}
