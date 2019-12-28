#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "itemcapsule.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/playersonic.h"
#include "../toolbox/maths.h"
#include "../animation/body.h"
#include "../entities/camera.h"
#include "../audio/audioplayer.h"
#include "../particles/particleresources.h"
#include "../particles/particle.h"
#include "../particles/particlemaster.h"
#include "dummy.h"
#include "shieldmagnet.h"
#include "shieldgreen.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> ItemCapsule::modelsAnchoredBase;
std::list<TexturedModel*> ItemCapsule::modelsAnchoredGlass;
std::list<TexturedModel*> ItemCapsule::modelsFloatingBase;
std::list<TexturedModel*> ItemCapsule::modelsFloatingGlass;

std::list<TexturedModel*> ItemCapsule::modelsItem1Up;
std::list<TexturedModel*> ItemCapsule::modelsItemBomb;
std::list<TexturedModel*> ItemCapsule::modelsItemInvincible;
std::list<TexturedModel*> ItemCapsule::modelsItemBarrierG;
std::list<TexturedModel*> ItemCapsule::modelsItemRing5;
std::list<TexturedModel*> ItemCapsule::modelsItemRing10;
std::list<TexturedModel*> ItemCapsule::modelsItemRing20;
std::list<TexturedModel*> ItemCapsule::modelsItemSpeedUp;
std::list<TexturedModel*> ItemCapsule::modelsItemBarrierB;

ItemCapsule::ItemCapsule()
{

}

ItemCapsule::ItemCapsule(float x, float y, float z, float upX, float upY, float upZ, int itemType, int boxType, std::list<Entity*>* entityListToAdd)
{
    position.x = x;
    position.y = y;
    position.z = z;
    upDir.set(upX, upY, upZ);
    upDir.normalize();
    rotX = 0;
    Maths::sphereAnglesFromPosition(&upDir, &rotY, &rotZ);
    rotRoll = 0;
    scale = 1;
    this->itemType = itemType;
    this->boxType = boxType;
    visible = true;
    updateTransformationMatrix();

    if (boxType == 0)
    {
        entityGlass = new Dummy(&ItemCapsule::modelsAnchoredGlass); INCR_NEW("Entity")
    }
    else
    {
        entityGlass = new Dummy(&ItemCapsule::modelsFloatingGlass); INCR_NEW("Entity")
    }

    entityGlass->visible = true;
    entityGlass->renderOrder = 3;
    entityListToAdd->push_back(entityGlass);
    entityGlass->setPosition(&position);
    entityGlass->setRotY(rotY);
    entityGlass->setRotZ(rotZ);
    entityGlass->updateTransformationMatrix();

    if (boxType == 0)
    {
        centerPos = position + upDir.scaleCopy(8.0f);
    }
    else
    {
        centerPos = position + upDir.scaleCopy(14.0f);
    }

    switch (itemType)
    {
        case  0:  entityItem = new Dummy(&ItemCapsule::modelsItemSpeedUp);    INCR_NEW("Entity") break;
        case  1:  entityItem = new Dummy(&ItemCapsule::modelsItemRing5);      INCR_NEW("Entity") break;
        case  2:  entityItem = new Dummy(&ItemCapsule::modelsItem1Up);        INCR_NEW("Entity") break;
        case  3:  entityItem = new Dummy(&ItemCapsule::modelsItemRing10);     INCR_NEW("Entity") break;
        case  4:  entityItem = new Dummy(&ItemCapsule::modelsItemRing20);     INCR_NEW("Entity") break;
        case  5:  entityItem = new Dummy(&ItemCapsule::modelsItemBarrierG);   INCR_NEW("Entity") break;
        case  6:  entityItem = new Dummy(&ItemCapsule::modelsItemBomb);       INCR_NEW("Entity") break;
        case  7:  break;
        case  8:  entityItem = new Dummy(&ItemCapsule::modelsItemBarrierB);   INCR_NEW("Entity") break;
        case  9:  break;
        case 10:  entityItem = new Dummy(&ItemCapsule::modelsItemInvincible); INCR_NEW("Entity") break;
        default:  break;
    }
    if (entityItem == nullptr)
    {
        entityItem = new Dummy(&ItemCapsule::modelsItemRing5); INCR_NEW("Entity")
        this->itemType = 1;
    }

    entityItem->visible = true;
    entityListToAdd->push_back(entityItem);
    entityItem->setPosition(&centerPos);
    entityItem->setRotY(rotY);
    entityItem->setRotZ(rotZ);

    if (boxType == 0)
    {
        entityItem->scale = 1.f;
    }
    else
    {
        entityItem->scale = 1.75f;
    }
}

void ItemCapsule::step()
{
    extern float dt;

    entityItem->increaseRotation(300*dt, 0, 0);
    entityItem->updateTransformationMatrix();

    if (abs(position.x - Global::gameMainPlayer->position.x) < 50 && 
        abs(position.z - Global::gameMainPlayer->position.z) < 50 &&
        abs(position.y - Global::gameMainPlayer->position.y) < 50)
    {
        float distToPlayer = (Global::gameMainPlayer->getCenterPosition() - centerPos).lengthSquared();

        if (boxType == 0)
        {
            if (distToPlayer < (7.5f*7.5f + 6.0f*6.0f))
            {
                die();
            }
        }
        else
        {
            if (distToPlayer < (7.5f*7.5f + 11.0f*11.0f))
            {
                die();
            }
        }
    }
}

void ItemCapsule::die()
{
    AudioPlayer::play(3, getPosition());

    Main_deleteChunkedEntity(this);
    Main_deleteChunkedEntity(entityItem);
    Main_deleteChunkedEntity(entityGlass);

    float height = 6.0f;
    float spread = 10.0f;
    Vector3f vel(0,0,0);
    for (int i = 4; i != 0; i--)
    {
        Vector3f pos(
            getX() + spread*(Maths::random() - 0.5f),
            getY() + spread*(Maths::random() - 0.5f) + height,
            getZ() + spread*(Maths::random() - 0.5f));

        ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &vel, 0, 0.75f, 0, 3*Maths::random() + 6, 0, false, false, 0.5f, true);
    }
    
    Global::gameMainPlayer->rebound(&centerPos);

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
            AudioPlayer::play(35, getPosition());
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
            ShieldGreen* shield = new ShieldGreen; INCR_NEW("Entity")
            Main_addEntity(shield);
            Global::gameMainPlayer->setShieldGreen(shield);
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
                    if (diff.lengthSquared() < 300*300)
                    {
                        e->die();
                    }
                }
            }

            std::list<std::unordered_set<Entity*>*> nearbyChunkedEntities;
            Global::getNearbyEntities(position.x, position.z, &nearbyChunkedEntities, 300);
            for (auto set : nearbyChunkedEntities)
            {
                for (auto e : (*set))
                {
                    if (e->isEnemy())
                    {
                        Vector3f diff = e->position - position;
                        if (diff.lengthSquared() < 300*300)
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
            ShieldMagnet* shield = new ShieldMagnet; INCR_NEW("Entity")
            Main_addEntity(shield);
            Global::gameMainPlayer->setShieldMagnet(shield);
            break;
        }

        case 10:
            //invincible
            Global::gameScore += 100;
            Global::gameMainPlayer->setInvincibleTimer(25.0f);
            break;


        default:
        {
            break;
        }
    }
}

std::list<TexturedModel*>* ItemCapsule::getModels()
{
    switch (boxType)
    {
        case 0:  return &ItemCapsule::modelsAnchoredBase;
        default: break;
    }

    return &ItemCapsule::modelsFloatingBase;
}

void ItemCapsule::loadStaticModels()
{
    if (ItemCapsule::modelsAnchoredBase.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading ItemCapsule static models...\n");
    #endif

    loadModel(&ItemCapsule::modelsAnchoredBase,   "res/Models/Objects/ItemBox/", "ItemBoxBase");
    loadModel(&ItemCapsule::modelsAnchoredGlass,  "res/Models/Objects/ItemBox/", "ItemBoxShell");
    loadModel(&ItemCapsule::modelsFloatingBase,   "res/Models/Objects/ItemBox/", "ItemBoxAirBase");
    loadModel(&ItemCapsule::modelsFloatingGlass,  "res/Models/Objects/ItemBox/", "ItemBoxAirShell");

    loadModel(&ItemCapsule::modelsItem1Up,        "res/Models/Objects/ItemBox/Items/", "Item1Up");
    loadModel(&ItemCapsule::modelsItemBomb,       "res/Models/Objects/ItemBox/Items/", "ItemBomb");
    loadModel(&ItemCapsule::modelsItemInvincible, "res/Models/Objects/ItemBox/Items/", "ItemInvincible");
    loadModel(&ItemCapsule::modelsItemBarrierG,   "res/Models/Objects/ItemBox/Items/", "ItemBarrierG");
    loadModel(&ItemCapsule::modelsItemRing5,      "res/Models/Objects/ItemBox/Items/", "ItemRing5");
    loadModel(&ItemCapsule::modelsItemRing10,     "res/Models/Objects/ItemBox/Items/", "ItemRing10");
    loadModel(&ItemCapsule::modelsItemRing20,     "res/Models/Objects/ItemBox/Items/", "ItemRing20");
    loadModel(&ItemCapsule::modelsItemSpeedUp,    "res/Models/Objects/ItemBox/Items/", "ItemSpeedUp");
    loadModel(&ItemCapsule::modelsItemBarrierB,   "res/Models/Objects/ItemBox/Items/", "ItemBarrierB");
}

void ItemCapsule::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting ItemCapsule static models...\n");
    #endif

    Entity::deleteModels(&ItemCapsule::modelsAnchoredBase);
    Entity::deleteModels(&ItemCapsule::modelsAnchoredGlass);
    Entity::deleteModels(&ItemCapsule::modelsFloatingBase);
    Entity::deleteModels(&ItemCapsule::modelsFloatingGlass);

    Entity::deleteModels(&ItemCapsule::modelsItem1Up);
    Entity::deleteModels(&ItemCapsule::modelsItemBomb);
    Entity::deleteModels(&ItemCapsule::modelsItemInvincible);
    Entity::deleteModels(&ItemCapsule::modelsItemBarrierG);
    Entity::deleteModels(&ItemCapsule::modelsItemRing5);
    Entity::deleteModels(&ItemCapsule::modelsItemRing10);
    Entity::deleteModels(&ItemCapsule::modelsItemRing20);
    Entity::deleteModels(&ItemCapsule::modelsItemSpeedUp);
    Entity::deleteModels(&ItemCapsule::modelsItemBarrierB);
}

const bool ItemCapsule::canHomingAttackOn()
{
    return true;
}

const Vector3f ItemCapsule::getHomingCenter()
{
    return centerPos;
}
