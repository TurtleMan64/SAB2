#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "itemballoon.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../entities/camera.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"
#include "shieldmagnet.hpp"
#include "shieldgreen.hpp"
#include "../menu/hud.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> ItemBalloon::models;

ItemBalloon::ItemBalloon()
{

}

ItemBalloon::ItemBalloon(float x, float y, float z, int itemType)
{
    position.x = x;
    position.y = y;
    position.z = z;
    this->itemType = itemType;
    visible = true;
    updateTransformationMatrix();

    switch (this->itemType)
    {
        case  1: Global::gameDebugRingCount+= 5; break;
        case  3: Global::gameDebugRingCount+=10; break;
        case  4: Global::gameDebugRingCount+=20; break;
        case  0:
        case  2:
        case  5:
        case  6:
        case  8:
        case 10:
        default: break;
    }
}

void ItemBalloon::step()
{
    if (std::abs(position.x - Global::gameMainPlayer->position.x) < 50 && 
        std::abs(position.z - Global::gameMainPlayer->position.z) < 50 &&
        std::abs(position.y - Global::gameMainPlayer->position.y) < 50)
    {
        float distToPlayer = (Global::gameMainPlayer->getCenterPosition() - position).lengthSquared();

        if (distToPlayer < (4*4 + 23*23))
        {
            die();
        }
    }
}

void ItemBalloon::die()
{
    AudioPlayer::play(3, &position, 0.75f);

    Global::deleteChunkedEntity(this);

    float height = 6.0f;
    float spread = 10.0f;
    Vector3f vel(0,0,0);
    for (int i = 4; i != 0; i--)
    {
        Vector3f pos(
            getX() + spread*(Maths::random() - 0.5f),
            getY() + spread*(Maths::random() - 0.5f) + height,
            getZ() + spread*(Maths::random() - 0.5f));

        ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &vel, 0, 0.75f, 3*Maths::random() + 6, 0, false, false, 0.5f, true);
    }
    
    Vector3f storedPlayerPos = Global::gameMainPlayer->position;
    Global::gameMainPlayer->rebound(&position);
    Global::gameMainPlayer->position = storedPlayerPos;

    switch (itemType)
    {
        case 0:
            //speed shoes
            Global::gameScore += 100;
            Global::gameMainPlayer->setSpeedshoesTimer(25.0f);
            break;

        case 1:
            //increase rings by 5
            Global::increaseRingCount(5);
            Global::gameScore += 50;
            break;

        case 2:
            //1 up
            Global::gameScore += 300;
            Global::gameLives++;
            AudioPlayer::play(35, &position);
            break;

        case 3:
            //increase rings by 10
            Global::increaseRingCount(10);
            Global::gameScore += 100;
            break;

        case 4:
            //increase rings by 20
            Global::increaseRingCount(20);
            Global::gameScore += 200;
            break;

        case 5:
        {
            //green shield
            Global::gameScore += 100;
            if (Global::gameMainPlayer->getShieldGreen()  == nullptr &&
                Global::gameMainPlayer->getShieldMagnet() != nullptr)
            {
                ShieldGreen* shield = new ShieldGreen; INCR_NEW("Entity");
                Global::addEntity(shield);
                Global::gameMainPlayer->setShieldGreen(shield);
            }
            break;
        }

        case 6:
        {
            //bomb
            Global::gameScore += 100;

            extern std::unordered_set<Entity*> gameEntities;
            for (auto e : gameEntities)
            {
                if (e->isEnemy())
                {
                    Vector3f diff = e->position - position;
                    if (diff.lengthSquared() < BOMB_RADIUS*BOMB_RADIUS)
                    {
                        e->die();
                    }
                }
            }

            std::list<std::unordered_set<Entity*>*> nearbyChunkedEntities;
            Global::getNearbyEntities(position.x, position.z, &nearbyChunkedEntities, BOMB_RADIUS);
            for (auto set : nearbyChunkedEntities)
            {
                for (auto e : (*set))
                {
                    if (e->isEnemy())
                    {
                        Vector3f diff = e->position - position;
                        if (diff.lengthSquared() < BOMB_RADIUS*BOMB_RADIUS)
                        {
                            e->die();
                        }
                    }
                }
            }

            break;
        }

        case 8:
        {
            //electric shield
            Global::gameScore += 100;
            if (Global::gameMainPlayer->getShieldMagnet() == nullptr)
            {
                ShieldMagnet* shield = new ShieldMagnet; INCR_NEW("Entity");
                Global::addEntity(shield);
                Global::gameMainPlayer->setShieldMagnet(shield);
            }
            break;
        }

        case 10:
            //invincible
            Global::gameScore += 100;
            Global::gameMainPlayer->setInvincibleTimer(25.0f);
            break;

        default:
            break;
    }

    HUD::displayItem(itemType);
}

std::list<TexturedModel*>* ItemBalloon::getModels()
{
    return &ItemBalloon::models;
}

void ItemBalloon::loadStaticModels()
{
    if (ItemBalloon::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading ItemBalloon static models...\n");
    #endif

    ModelLoader::loadModel(&ItemBalloon::models,   "res/Models/Objects/ItemBalloon/", "ItemBalloon");
}

void ItemBalloon::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting ItemBalloon static models...\n");
    #endif

    Entity::deleteModels(&ItemBalloon::models);
}

bool ItemBalloon::canHomingAttackOn() const
{
    return true;
}

Vector3f ItemBalloon::getHomingCenter() const
{
    return position;
}
