#include "mhmissle.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../controllableplayer.hpp"

extern float dt;

std::list<TexturedModel*> MH_Missle::models;

MH_Missle::MH_Missle() {}

MH_Missle::MH_Missle(float x, float y, float z)
{
    position.set(x, y, z);
    updateTransformationMatrix();
}

void MH_Missle::step() 
{
    if (timer == 0.0f)
    {
        Vector3f handlePos(position.x+88, position.y+532, position.z);
        Vector3f roughDiff = handlePos - Global::gameMainPlayer->position;
        //printf("roughDiff = %f %f %f\n", roughDiff.x, roughDiff.y, roughDiff.z);
        if (roughDiff.lengthSquared() < 20*20)
        {
            Vector3f end1(position.x + 87.7854f, position.y + 532.0f, position.z - 10.0f);
            Vector3f end2(position.x + 87.7854f, position.y + 532.0f, position.z + 10.0f);
            if (Maths::pointIsInCylinder(&Global::gameMainPlayer->position, &end1, &end2, 5.0f))
            {
                timer += dt;
                Global::gameMainPlayer->startGrabbing();
                Global::gameMainPlayer->vel.set(-500.0f, 0, 0);
                Global::gameMainPlayer->camDir.set(-1, 0, 0);
                Global::gameMainPlayer->canMoveTimer = 200.0f;
            }
        }
    }
    else if (timer < 1.0f)
    {
        timer += dt;
        putPlayerAtHandle();

        Global::gameMainPlayer->camDir.set(-1, 0, 0);
        Global::gameMainPlayer->camDir.normalize();
    }
    else if (timer < 4.0f)
    {
        position.y += 700*dt;
        updateTransformationMatrix();

        putPlayerAtHandle();

        timer += dt;

        Global::gameMainPlayer->camDir.set(-1, 5, 0);
        Global::gameMainPlayer->camDir.normalize();
    }
    else if (timer < 7.0f)
    {
        position.y += 700*dt;
        updateTransformationMatrix();

        putPlayerAtHandle();

        Global::gameMainPlayer->camDir.set(0, -8, -1);
        Global::gameMainPlayer->camDir.normalize();

        timer += dt;

        if (timer >= 7.0f)
        {
            Global::gameMainPlayer->canMoveTimer = 5.0f;
            Global::gameMainPlayer->stopGrabbing();
            Global::gameMainPlayer->vel.set(400.0f, 0, 0);
            Global::gameMainPlayer->camDir.set(1, -10, 0);
            Global::gameMainPlayer->camDir.normalize();
        }
    }
    else
    {
        position.y += 700*dt;
        updateTransformationMatrix();
    }
}

void MH_Missle::putPlayerAtHandle()
{
    Global::gameMainPlayer->position.set(position.x + 87.7854f, position.y + 532.0f, position.z);
    Global::gameMainPlayer->vel.set(-500.0f, 0, 0);
    Global::gameMainPlayer->animate();
}

std::list<TexturedModel*>* MH_Missle::getModels()
{
    return &MH_Missle::models;
}

void MH_Missle::loadStaticModels()
{
    if (MH_Missle::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading MH_Missle static models...\n");
    #endif

    ModelLoader::loadModel(&MH_Missle::models, "res/Models/Levels/MetalHarbor/Objects/", "Missle");
}

void MH_Missle::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting MH_Missle static models...\n");
    #endif

    Entity::deleteModels(&MH_Missle::models);
}
