#include "rhblimp.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../dummy.hpp"
#include "../../toolbox/maths.hpp"

std::list<TexturedModel*> RH_Blimp::modelsBlimp;
std::list<TexturedModel*> RH_Blimp::modelsSpotlight;

RH_Blimp::RH_Blimp()
{

}

RH_Blimp::RH_Blimp(float x, float y, float z, float newRotY)
{
    position.set(x, y, z);
    rotY = newRotY;
    updateTransformationMatrix();

    Vector3f spotlightPos(-173.67f, -70.358f, 0.0f);
    Vector3f yAxis(0, 1, 0);
    spotlightPos = Maths::rotatePoint(&spotlightPos, &yAxis, Maths::toRadians(rotY));

    spotlightRotAxis.set(-92, -254, 0);
    spotlightRotAxis.normalize();
    spotlightRotAxis = Maths::rotatePoint(&spotlightRotAxis, &yAxis, Maths::toRadians(rotY));

    spotlight = new Dummy(&RH_Blimp::modelsSpotlight); INCR_NEW("Entity");
    spotlight->position = position + spotlightPos;
    Global::addEntity(spotlight);

    Vector3f perpen(0, 0, 1);
    perpen = Maths::rotatePoint(&perpen, &yAxis, Maths::toRadians(rotY));

    spotlightDirectionTarget = Maths::rotatePoint(&spotlightRotAxis, &perpen, Maths::toRadians(30.0f));
    spotlightDirectionCurrent = spotlightDirectionTarget;
}

void RH_Blimp::step()
{
    extern float dt;

    timer += dt;
    
    if (timer > 2.0f)
    {
        timer -= 2.0f + Maths::random();
        //printf("new thingy\n");
        spotlightDirectionTarget = Maths::rotatePoint(&spotlightDirectionTarget, &spotlightRotAxis, 2*Maths::PI*(Maths::random()));
    }
    else
    {
        spotlightDirectionCurrent = Maths::interpolateVector(&spotlightDirectionCurrent, &spotlightDirectionTarget, dt);
    }

    Maths::sphereAnglesFromPosition(&spotlightDirectionCurrent, &spotlight->rotY, &spotlight->rotZ);
    //printf("   spotlight->rotY = %f\n", spotlight->rotY);
    spotlight->updateTransformationMatrix();
}

std::list<TexturedModel*>* RH_Blimp::getModels()
{
    return &RH_Blimp::modelsBlimp;
}

void RH_Blimp::loadStaticModels()
{
    if (RH_Blimp::modelsBlimp.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading RH_Blimp static models...\n");
    #endif

    ModelLoader::loadModel(&RH_Blimp::modelsBlimp,     "res/Models/Levels/RadicalHighway/Objects/", "Blimp");
    ModelLoader::loadModel(&RH_Blimp::modelsSpotlight, "res/Models/Levels/RadicalHighway/Objects/", "Spotlight");
}

void RH_Blimp::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting RH_Blimp static models...\n");
    #endif

    Entity::deleteModels(&RH_Blimp::modelsBlimp);
    Entity::deleteModels(&RH_Blimp::modelsSpotlight);
}
