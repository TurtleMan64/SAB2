#include "bosslaser.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../controllableplayer.hpp"
#include "../../particles/particleresources.hpp"
#include "../../particles/particle.hpp"
#include "../../particles/particlemaster.hpp"
#include "../../audio/audioplayer.hpp"
#include "../../menu/timer.hpp"

std::vector<TexturedModel*> Boss_Laser::models;

Boss_Laser::Boss_Laser(float x, float y, float z)
{
    position.set(x, y, z);
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    dir = Maths::randomPointOnSphere();
    normal = Maths::randomPointOnSphere();
    dir = Maths::projectOntoPlane(&dir, &normal);
    dir.normalize();

    scale = 1;
    visible = true;

    updateTransformationMatrix();
}

void Boss_Laser::step()
{
    if (Global::mainHudTimer != nullptr && Global::mainHudTimer->totalTime > 20.0f)
    {
        extern float dt;

        // Rotate around the player
        normal = normal + dir.scaleCopy(1.5f*dt);
        normal = normal + Vector3f(Maths::nextGaussian(), Maths::nextGaussian(), Maths::nextGaussian()).scaleCopy(0.1f*dt);
        normal.normalize();
        dir = Maths::projectOntoPlane(&dir, &normal);
        dir.normalize();

        Vector3f end = Global::gameMainPlayer->position + normal.scaleCopy(200.0f);

        Vector3f diff = end - position;
        if (diff.lengthSquared() > 3000.0f*3000.0f)
        {
            diff.setLength(3000.0f);
            end = position + diff;
        }
        else if (diff.lengthSquared() < 900.0f*900.0f)
        {
            diff.setLength(900.0f);
            end = position + diff;
        }
        Maths::sphereAnglesFromPosition(&diff, &rotY, &rotZ);
        updateTransformationMatrix(diff.length(), 50, 50);

        if (Global::gameMainPlayer->isVulnerable() &&
            Maths::pointIsInCylinder(&Global::gameMainPlayer->position, &position, &end, 50.0f))
        {
            Global::gameMainPlayer->takeDamage(&end);
        }
    }
}

std::vector<TexturedModel*>* Boss_Laser::getModels()
{
    return &Boss_Laser::models;
}

void Boss_Laser::loadStaticModels()
{
    if (Boss_Laser::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Boss_Laser static models...\n");
    #endif

    ModelLoader::loadModel(&Boss_Laser::models, "res/Models/Levels/Boss/Laser/", "Laser");
}

void Boss_Laser::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Boss_Laser static models...\n");
    #endif

    Entity::deleteModels(&Boss_Laser::models);
}
