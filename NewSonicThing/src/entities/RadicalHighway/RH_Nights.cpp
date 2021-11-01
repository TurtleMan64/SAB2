#include "rhnights.h"
#include "../entity.h"
#include "../../engineTester/main.h"
#include "../../models/models.h"
#include "../../objLoader/objLoader.h"

std::list<TexturedModel*> RH_Nights::models;

RH_Nights::RH_Nights()
{

}

RH_Nights::RH_Nights(float x, float y, float z)
{
    position.set(x, y, z);
    updateTransformationMatrixYXZ();
}

void RH_Nights::step()
{
    extern float dt;
    rotY += 40*dt;
    updateTransformationMatrixYXZ();

    timer += dt;

    if (timer >= 4.0f)
    {
        timer -= 4.0f;
    }

    baseColour.set(1, 1, 1);
    baseColour.scale(1.0f + timer*5);
}

std::list<TexturedModel*>* RH_Nights::getModels()
{
    return &RH_Nights::models;
}

void RH_Nights::loadStaticModels()
{
    if (RH_Nights::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading RH_Nights static models...\n");
    #endif

    ObjLoader::loadModel(&RH_Nights::models, "res/Models/Levels/RadicalHighway/Objects/", "Nights");
}

void RH_Nights::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting RH_Nights static models...\n");
    #endif

    Entity::deleteModels(&RH_Nights::models);
}
