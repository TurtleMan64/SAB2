#include <glad/glad.h>

#include "../entity.hpp"
#include "castagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"
#include "../../toolbox/maths.hpp"
#include "../../particles/particle.hpp"
#include "../../particles/particleresources.hpp"
#include "../../particles/particlemaster.hpp"
#include "caparticle.hpp"

std::vector<TexturedModel*> CA_StageManager::modelsSkydome;

CA_StageManager::CA_StageManager()
{
    visible = false;
    firstFrame = true;
    if (Global::renderParticles)
    {
        int numToSpawn = 4000 - CA_Particle::currentPopulation;
        for (int i = 0; i < numToSpawn; i++)
        {
            new CA_Particle; INCR_NEW("CA_Particle")
        }
    }
}

void CA_StageManager::step()
{
    //set pollen particles center point
    Vector3f center = Global::gameMainPlayer->position;
    center = center + Global::gameMainPlayer->vel.scaleCopy(0.4f);
    float basex = center.x - CA_Particle::rainRadius;
    float basey = center.y - CA_Particle::rainRadius;
    float basez = center.z - CA_Particle::rainRadius;
    CA_Particle::centerSpawnPos.set(basex, basey, basez);
}

std::vector<TexturedModel*>* CA_StageManager::getModels()
{
    return &CA_StageManager::modelsSkydome;
}

void CA_StageManager::loadStaticModels()
{
    //if (CC_StageManager::modelsSkydome.size() > 0)
    //{
    //    return;
    //}
    //
    //#ifdef DEV_MODE
    //printf("Loading CC_StageManager static models...\n");
    //#endif

    //ModelLoader::loadModel(&CC_StageManager::modelsSkydome,  "res/Models/Levels/GreenForest/", "Skydome");
}

void CA_StageManager::deleteStaticModels()
{
    //#ifdef DEV_MODE
    //printf("Deleting CC_StageManager static models...\n");
    //#endif

    //Entity::deleteModels(&CC_StageManager::modelsSkydome);
}
