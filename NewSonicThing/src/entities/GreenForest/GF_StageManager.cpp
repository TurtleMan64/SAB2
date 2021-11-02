#include <glad/glad.h>

#include "../entity.hpp"
#include "gfstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"
#include "../../toolbox/maths.hpp"
#include "../../particles/particle.hpp"
#include "../../particles/particleresources.hpp"
#include "../../particles/particlemaster.hpp"
#include "gfparticle.hpp"

std::list<TexturedModel*> GF_StageManager::modelsSkydome;

GF_StageManager::GF_StageManager()
{
    visible = true;
    firstFrame = true;
    if (Global::renderParticles)
    {
        int numToSpawn = 4000 - GF_Particle::currentPopulation;
        for (int i = 0; i < numToSpawn; i++)
        {
            new GF_Particle; INCR_NEW("GF_Particle")
        }
    }
}

void GF_StageManager::step()
{
    //setting the global water heights
    if (Global::gameMainPlayer->position.x < 25 &&
        Global::gameMainPlayer->position.z > 1800)
    {
        Global::waterHeight = -10000.0f;
    }
    else if (Global::gameMainPlayer->position.x < 143 &&
             Global::gameMainPlayer->position.z > 2500 &&
             Global::gameMainPlayer->position.y < -1041)
    {
        Global::waterHeight = -10000.0f;
    }
    else if (Global::gameMainPlayer->position.x < 447 &&
             Global::gameMainPlayer->position.z > 2849)
    {
        Global::waterHeight = -10000.0f;
    }
    else if (Global::gameMainPlayer->position.x < 1058 &&
             Global::gameMainPlayer->position.z > 2912)
    {
        Global::waterHeight = -10000.0f;
    }
    else if (Global::gameMainPlayer->position.z > 5240.0f)
    {
        Global::waterHeight = -2325.0f;
    }
    else
    {
        Global::waterHeight = -980.0f;
    }

    //set the position of the background sky dome
    position.y = Global::waterHeight+1000.0f;
    position.x = Global::gameMainPlayer->getX();
    position.z = Global::gameMainPlayer->getZ();
    updateTransformationMatrix();

    //kill sonic
    if (Global::gameMainPlayer->position.y < Global::waterHeight-150)
    {
        Global::gameMainPlayer->die();
    }

    //if first frame, spawn black fadein
    if (firstFrame)
    {
        firstFrame = false;
        Vector3f partVel(0, 0, 0);
        ParticleMaster::createParticle(ParticleResources::textureBlackFade, Global::gameCamera->getFadePosition1(), &partVel, 0, 1.0f, 0, 900, 0, true, false, 2.0f, false);
    }

    //set pollen particles center point
    Vector3f center = Global::gameMainPlayer->position;
    center = center + Global::gameMainPlayer->vel.scaleCopy(0.4f);
    float basex = center.x - GF_Particle::pollenRadius;
    float basey = center.y - GF_Particle::pollenRadius;
    float basez = center.z - GF_Particle::pollenRadius;
    GF_Particle::centerSpawnPos.set(basex, basey, basez);
}

std::list<TexturedModel*>* GF_StageManager::getModels()
{
    return &GF_StageManager::modelsSkydome;
}

void GF_StageManager::loadStaticModels()
{
    if (GF_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading GF_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&GF_StageManager::modelsSkydome,  "res/Models/Levels/GreenForest/", "Skydome");
}

void GF_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting GF_StageManager static models...\n");
    #endif

    Entity::deleteModels(&GF_StageManager::modelsSkydome);
}
