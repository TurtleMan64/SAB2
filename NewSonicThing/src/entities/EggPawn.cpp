#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "eggpawn.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"
#include "../collision/collisionchecker.hpp"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::vector<std::list<TexturedModel*>> EggPawn::modelsWalk;
std::list<TexturedModel*> EggPawn::modelsStand;
std::vector<EggPawn*> EggPawn::pawns;

EggPawn::EggPawn()
{

}

EggPawn::EggPawn(float x, float y, float z, float dx, float dz)
{
    position.x = x;
    position.y = y;
    position.z = z;
    center = position;
    center.y += centerHeight;
    lookDir.set(dx, 0, dz);
    lookDir.normalize();
    rotY = Maths::toDegrees(atan2f(-dz, dx));
    isWalking = false;
    hitTimer = 0.0f;

    updateTransformationMatrix();
}

void EggPawn::step()
{
    extern float dt;
    hitTimer-=dt;

    isWalking = false;

    if (std::abs(position.x - Global::gameMainPlayer->position.x) < EggPawn::activationRange &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < EggPawn::activationRange &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < EggPawn::activationRange)
    {
        Vector3f toPlayerDiff = Global::gameMainPlayer->getCenterPosition() - center;

        if ((toPlayerDiff.lengthSquared()) < hitRadius*hitRadius)
        {
            if (Global::gameMainPlayer->isVulnerable())
            {
                Global::gameMainPlayer->takeDamage(&position);
                hitTimer = 3.0f;
            }
            else if (Global::gameMainPlayer->canDealDamage())
            {
                die();
                Global::gameMainPlayer->rebound(&center);
                Global::gameMainPlayer->increaseCombo();
                return;
            }
        }

        // move toward the player
        if (hitTimer <= 0.0f)
        {
            isWalking = true;

            //rotate toward player
            toPlayerDiff.y = 0;
            toPlayerDiff.normalize();
            lookDir = Maths::interpolateVector(&lookDir, &toPlayerDiff, EggPawn::lookSpeed*dt);
            lookDir.normalize();
            rotY = Maths::toDegrees(atan2f(-lookDir.z, lookDir.x));

            position = position + lookDir.scaleCopy(30*dt);
            center = position;
            center.y += centerHeight;
        }

        // get pushed by other pawns
        for (EggPawn* pawn : EggPawn::pawns)
        {
            Vector3f diff = position - pawn->position;
            if (diff.lengthSquared() < collisionRadiusPawn*collisionRadiusPawn)
            {
                diff.setLength(collisionRadiusPawn);
                position = pawn->position + diff;
                center = position;
                center.y += centerHeight;
            }
        }

        // check collision vertical
        if (CollisionChecker::checkCollision(position.x, position.y + centerHeight, position.z, position.x, position.y - centerHeight, position.z))
        {
            position.set(CollisionChecker::getCollidePosition());
        }
        else
        {
            position.y -= 30*dt;
        }
        center = position;
        center.y += centerHeight;

        // check cillision horizontal
        for (int i = 0; i < 5; i++)
        {
            Vector3f dir(collisionRadiusWall, 0, 0);
            dir = Maths::rotatePoint(&dir, &Y_AXIS, Maths::PI*2*(((float)i)/5.0f));
        
            if (CollisionChecker::checkCollision(position.x, position.y + centerHeight, position.z, position.x + dir.x, position.y + centerHeight, position.z + dir.z))
            {
                Vector3f colPosition(CollisionChecker::getCollidePosition());
                float dist = collisionRadiusWall - (center - colPosition).length();
                dir.setLength(dist);
                position = position - dir;
                center = position;
                center.y += centerHeight;
            }
        }

        updateTransformationMatrix();
    }
    //Maths::createTransformationMatrix(&transformationMatrix, &posBob, 0, rotY, 0, 0, 1);

    pawns.push_back(this);
}

void EggPawn::die()
{
    AudioPlayer::play(3, &position);

    Global::deleteChunkedEntity(this);

    const float height = 10.0f;
    const float spread = 20.0f;

    Vector3f vel(0, 0, 0);
    Vector3f toCamDiff = Global::gameCamera->eye - position;
    toCamDiff.setLength(20);
    
    for (int i = 7; i != 0; i--)
    {
        Vector3f pos(
            getX() + spread*(Maths::random() - 0.5f),
            getY() + spread*(Maths::random() - 0.5f) + height,
            getZ() + spread*(Maths::random() - 0.5f));
        pos = pos + toCamDiff; //so that these aren't behind the big explosion

        ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &vel, 0, 0.75f, 0, 3*Maths::random() + 6, 0, false, false, 0.5f, true);
    }
    
    Vector3f pos(
        getX(),
        getY() + height,
        getZ());
    
    ParticleMaster::createParticle(ParticleResources::textureExplosion2, &pos, &vel, 0, 0.916f, 0, 20, 0, false, false, 0.75f, true);
    
    Global::gameScore += 100;
}

std::list<TexturedModel*>* EggPawn::getModels()
{
    if (isWalking)
    {
        float time = fmodf(Global::gameClock*1.5f, 0.9999f);
        return &EggPawn::modelsWalk[(int)(time*31.0f)];
    }

    return &EggPawn::modelsStand;
}

void EggPawn::loadStaticModels()
{
    if (EggPawn::modelsStand.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading EggPawn static models...\n");
    #endif

    ModelLoader::loadModel(&EggPawn::modelsStand, "res/Models/Objects/EggPawn/", "EggPawn");

    for (int i = 0; i < 31; i++)
    {
        std::list<TexturedModel*> models;
        EggPawn::modelsWalk.push_back(models);
    }

    ModelLoader::loadModel(&EggPawn::modelsWalk[ 0], "res/Models/Objects/EggPawn/", "EggPawnWalk_000000");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 1], "res/Models/Objects/EggPawn/", "EggPawnWalk_000001");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 2], "res/Models/Objects/EggPawn/", "EggPawnWalk_000002");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 3], "res/Models/Objects/EggPawn/", "EggPawnWalk_000003");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 4], "res/Models/Objects/EggPawn/", "EggPawnWalk_000004");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 5], "res/Models/Objects/EggPawn/", "EggPawnWalk_000005");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 6], "res/Models/Objects/EggPawn/", "EggPawnWalk_000006");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 7], "res/Models/Objects/EggPawn/", "EggPawnWalk_000007");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 8], "res/Models/Objects/EggPawn/", "EggPawnWalk_000008");
    ModelLoader::loadModel(&EggPawn::modelsWalk[ 9], "res/Models/Objects/EggPawn/", "EggPawnWalk_000009");
    ModelLoader::loadModel(&EggPawn::modelsWalk[10], "res/Models/Objects/EggPawn/", "EggPawnWalk_000010");
    ModelLoader::loadModel(&EggPawn::modelsWalk[11], "res/Models/Objects/EggPawn/", "EggPawnWalk_000011");
    ModelLoader::loadModel(&EggPawn::modelsWalk[12], "res/Models/Objects/EggPawn/", "EggPawnWalk_000012");
    ModelLoader::loadModel(&EggPawn::modelsWalk[13], "res/Models/Objects/EggPawn/", "EggPawnWalk_000013");
    ModelLoader::loadModel(&EggPawn::modelsWalk[14], "res/Models/Objects/EggPawn/", "EggPawnWalk_000014");
    ModelLoader::loadModel(&EggPawn::modelsWalk[15], "res/Models/Objects/EggPawn/", "EggPawnWalk_000015");
    ModelLoader::loadModel(&EggPawn::modelsWalk[16], "res/Models/Objects/EggPawn/", "EggPawnWalk_000016");
    ModelLoader::loadModel(&EggPawn::modelsWalk[17], "res/Models/Objects/EggPawn/", "EggPawnWalk_000017");
    ModelLoader::loadModel(&EggPawn::modelsWalk[18], "res/Models/Objects/EggPawn/", "EggPawnWalk_000018");
    ModelLoader::loadModel(&EggPawn::modelsWalk[19], "res/Models/Objects/EggPawn/", "EggPawnWalk_000019");
    ModelLoader::loadModel(&EggPawn::modelsWalk[20], "res/Models/Objects/EggPawn/", "EggPawnWalk_000020");
    ModelLoader::loadModel(&EggPawn::modelsWalk[21], "res/Models/Objects/EggPawn/", "EggPawnWalk_000021");
    ModelLoader::loadModel(&EggPawn::modelsWalk[22], "res/Models/Objects/EggPawn/", "EggPawnWalk_000022");
    ModelLoader::loadModel(&EggPawn::modelsWalk[23], "res/Models/Objects/EggPawn/", "EggPawnWalk_000023");
    ModelLoader::loadModel(&EggPawn::modelsWalk[24], "res/Models/Objects/EggPawn/", "EggPawnWalk_000024");
    ModelLoader::loadModel(&EggPawn::modelsWalk[25], "res/Models/Objects/EggPawn/", "EggPawnWalk_000025");
    ModelLoader::loadModel(&EggPawn::modelsWalk[26], "res/Models/Objects/EggPawn/", "EggPawnWalk_000026");
    ModelLoader::loadModel(&EggPawn::modelsWalk[27], "res/Models/Objects/EggPawn/", "EggPawnWalk_000027");
    ModelLoader::loadModel(&EggPawn::modelsWalk[28], "res/Models/Objects/EggPawn/", "EggPawnWalk_000028");
    ModelLoader::loadModel(&EggPawn::modelsWalk[29], "res/Models/Objects/EggPawn/", "EggPawnWalk_000029");
    ModelLoader::loadModel(&EggPawn::modelsWalk[30], "res/Models/Objects/EggPawn/", "EggPawnWalk_000030");
}

void EggPawn::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting EggPawn static models...\n");
    #endif

    Entity::deleteModels(&EggPawn::modelsStand);
    
    for (int i = 0; i < (int)EggPawn::modelsWalk.size(); i++)
    {
        Entity::deleteModels(&EggPawn::modelsWalk[i]);
    }
    EggPawn::modelsWalk.clear();
}

bool EggPawn::canHomingAttackOn() const
{
    return true;
}

Vector3f EggPawn::getHomingCenter() const
{
    return center;
}

bool EggPawn::isEnemy() const
{
    return true;
}
