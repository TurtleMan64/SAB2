#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "bullet.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../collision/collisionchecker.h"
#include "../particles/particlemaster.h"
#include "../particles/particleresources.h"
#include "../collision/triangle3d.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> Bullet::models;

Bullet::Bullet()
{

}

Bullet::Bullet(Vector3f* pos, Vector3f* velocity, float timeToLive)
{
    position.set(pos);
    vel.set(velocity);
    timeUntilDeath = timeToLive;

    rotY = Maths::toDegrees(atan2f(-vel.z, vel.x));
    rotZ = Maths::toDegrees(atan2f(vel.y, sqrtf(vel.x*vel.x + vel.z*vel.z)));

    renderOrder = 3;
    updateTransformationMatrix();
}

void Bullet::step()
{
    extern float dt;

    timeUntilDeath -= dt;

    if (timeUntilDeath <= 0.0f)
    {
        Main_deleteEntity(this);
        visible = false;
        return;
    }
    
    Vector3f nextPos = position + vel.scaleCopy(dt);

    if (CollisionChecker::checkCollision(&position, &nextPos))
    {
        Vector3f hit(CollisionChecker::getCollidePosition());
        Vector3f bounce = Maths::bounceVector(&vel, &CollisionChecker::getCollideTriangle()->normal, 0.8f);

        float spread = 4;
        for (int i = 7; i != 0; i--)
        {
            Vector3f pos(
                hit.x + spread*(Maths::random() - 0.5f),
                hit.y + spread*(Maths::random() - 0.5f),
                hit.z + spread*(Maths::random() - 0.5f));

            ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &bounce, 0, 0.75f, 0, 1*Maths::random() + 2, 0, false, false, 0.5f, true);
        }

        Main_deleteEntity(this);
        visible = false;
        return;
    }

    position = position + vel.scaleCopy(dt);
    rotX += dt*1800;
    updateTransformationMatrix();

    Vector3f diff = position - Global::gameMainPlayer->position;
    float dist = diff.lengthSquared();
    if (dist < 7*7)
    {
        if (Global::gameMainPlayer->canDealDamage())
        {
            Global::gameMainPlayer->takeDamage(&position);
            Main_deleteEntity(this);
            visible = false;
        }
    }
}

std::list<TexturedModel*>* Bullet::getModels()
{
    return &Bullet::models;
}

void Bullet::loadStaticModels()
{
    if (Bullet::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading Bullet static models...\n");
    #endif

    loadModel(&Bullet::models, "res/Models/Objects/Bullet/", "Bullet");
}

void Bullet::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting Bullet static models...\n");
    #endif

    Entity::deleteModels(&Bullet::models);
}
