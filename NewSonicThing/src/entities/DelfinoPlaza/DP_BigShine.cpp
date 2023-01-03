#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "dpbigshine.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> DP_BigShine::models;

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

std::vector<TexturedModel*>* DP_BigShine::getModels()
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
    printf("Loading DP_BigShine static models...\n");
    #endif

    ModelLoader::loadModel(&DP_BigShine::models, "res/Models/Levels/DelfinoPlaza/BigShine/", "BigShine");
}

void DP_BigShine::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DP_BigShine static models...\n");
    #endif

    Entity::deleteModels(&DP_BigShine::models);
}
