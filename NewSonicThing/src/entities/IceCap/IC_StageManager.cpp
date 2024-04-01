#include <glad/glad.h>

#include "../entity.hpp"
#include "icstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"
#include "../../toolbox/maths.hpp"
#include "../../particles/particle.hpp"
#include "../../particles/particleresources.hpp"
#include "../../particles/particlemaster.hpp"
#include "icparticle.hpp"

std::vector<TexturedModel*> IC_StageManager::modelsSkydome;

IC_StageManager::IC_StageManager()
{
    visible = false;
    firstFrame = true;
    if (Global::renderParticles)
    {
        int numToSpawn = 4000 - IC_Particle::currentPopulation;
        for (int i = 0; i < numToSpawn; i++)
        {
            new IC_Particle; INCR_NEW("IC_Particle")
        }
    }
}

void IC_StageManager::step()
{
    //set pollen particles center point
    Vector3f center = Global::gameMainPlayer->position;
    center = center + Global::gameMainPlayer->vel.scaleCopy(0.4f);
    float basex = center.x - IC_Particle::snowRadius;
    float basey = center.y - IC_Particle::snowRadius;
    float basez = center.z - IC_Particle::snowRadius;
    IC_Particle::centerSpawnPos.set(basex, basey, basez);
}

std::vector<TexturedModel*>* IC_StageManager::getModels()
{
    return &IC_StageManager::modelsSkydome;
}

void IC_StageManager::loadStaticModels()
{
    //if (IC_StageManager::modelsSkydome.size() > 0)
    //{
    //    return;
    //}
    //
    //#ifdef DEV_MODE
    //printf("Loading IC_StageManager static models...\n");
    //#endif

    //ModelLoader::loadModel(&IC_StageManager::modelsSkydome,  "res/Models/Levels/GreenForest/", "Skydome");
}

void IC_StageManager::deleteStaticModels()
{
    //#ifdef DEV_MODE
    //printf("Deleting IC_StageManager static models...\n");
    //#endif

    //Entity::deleteModels(&IC_StageManager::modelsSkydome);
}
