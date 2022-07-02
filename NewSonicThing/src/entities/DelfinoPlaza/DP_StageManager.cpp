#include <glad/glad.h>

#include "../entity.hpp"
#include "dpstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../particles/particlemaster.hpp"
#include "../../particles/particleresources.hpp"

std::list<TexturedModel*> DP_StageManager::modelsSkyPass1;
std::list<TexturedModel*> DP_StageManager::modelsSkyPass2;
std::list<TexturedModel*> DP_StageManager::modelsSkyTransparent;

DP_StageManager::DP_StageManager()
{
    skyPass2       = new Dummy(&DP_StageManager::modelsSkyPass2);       INCR_NEW("Entity");
    skyTransparent = new Dummy(&DP_StageManager::modelsSkyTransparent); INCR_NEW("Entity");

    visible = true;
    skyPass2->visible = true;
    skyTransparent->visible = false;

    scale = 3.0f;
    skyPass2->scale = 3.0f;
    skyTransparent->scale = 1.8f;

    Global::addEntity(skyPass2);
    Global::addEntity(skyTransparent);
}

void DP_StageManager::step()
{
    //set the position of the background sky dome
    position.set(&Global::gameMainPlayer->position);
    position.y = 0;
    updateTransformationMatrix();

    skyPass2->setPosition(&position);
    skyTransparent->setPosition(&position);

    skyPass2->updateTransformationMatrix();
    skyTransparent->updateTransformationMatrix();

    if (Global::levelId == LVL_DELFINO_PLAZA)
    {
        extern float dt;

        float chanceToSpawn = dt*60.0f;

        if (Maths::nextUniform() < chanceToSpawn) //on higher than 60fps, dont spawn every frame
        {
            Vector3f origin1(-220.489f,  3.1f, -192.629f);
            Vector3f origin2(-147.044f,  3.1f, -199.254f);
            Vector3f origin3( -95.8652f, 3.1f, -242.41f);
            Vector3f vel1(0, 3, 1);
            Vector3f vel2(1, 4, 2);
            Vector3f vel3(2, 4, 1);

            vel1.setLength(120);
            vel2.setLength(120);
            vel3.setLength(120);

            Vector3f randVel(Maths::nextUniform() - 0.5f, Maths::nextUniform() - 0.5f, Maths::nextUniform() - 0.5f);
            randVel.scale(10.0f);

            vel1 = vel1 + randVel;
            vel2 = vel2 + randVel;
            vel3 = vel3 + randVel;

            ParticleMaster::createParticle(ParticleResources::textureBubble, &origin1, &vel1, 100.0f, 3.0f, 0.0f, 3.0f, 16.0f, false, false, 0.6f, true);
            ParticleMaster::createParticle(ParticleResources::textureBubble, &origin2, &vel2, 100.0f, 3.0f, 0.0f, 3.0f, 16.0f, false, false, 0.6f, true);
            ParticleMaster::createParticle(ParticleResources::textureBubble, &origin3, &vel3, 100.0f, 3.0f, 0.0f, 3.0f, 16.0f, false, false, 0.6f, true);
        }
    }
}

std::list<TexturedModel*>* DP_StageManager::getModels()
{
    return &DP_StageManager::modelsSkyPass1;
}

void DP_StageManager::loadStaticModels()
{
    if (DP_StageManager::modelsSkyPass1.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DP_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&DP_StageManager::modelsSkyPass1,       "res/Models/Levels/DelfinoPlaza/Sky/", "Blue");
    ModelLoader::loadModel(&DP_StageManager::modelsSkyPass2,       "res/Models/Levels/DelfinoPlaza/Sky/", "Pass2");
    ModelLoader::loadModel(&DP_StageManager::modelsSkyTransparent, "res/Models/Levels/DelfinoPlaza/Sky/", "Trans");

    setModelsRenderOrder(&DP_StageManager::modelsSkyPass2,       1);
    setModelsRenderOrder(&DP_StageManager::modelsSkyTransparent, 3);
}

void DP_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DP_StageManager static models...\n");
    #endif

    Entity::deleteModels(&DP_StageManager::modelsSkyPass1);
    Entity::deleteModels(&DP_StageManager::modelsSkyPass2);
    Entity::deleteModels(&DP_StageManager::modelsSkyTransparent);
}
