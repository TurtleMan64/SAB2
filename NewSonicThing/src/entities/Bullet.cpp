#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "bullet.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../collision/collisionchecker.hpp"
#include "../particles/particlemaster.hpp"
#include "../particles/particleresources.hpp"
#include "../collision/triangle3d.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> Bullet::models;

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

    updateTransformationMatrix();
}

void Bullet::step()
{
    extern float dt;

    timeUntilDeath -= dt;

    if (timeUntilDeath <= 0.0f)
    {
        Global::deleteEntity(this);
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

            ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &bounce, 0, 0.75f, 1*Maths::random() + 2, 0, false, false, 0.5f, true);
        }

        Global::deleteEntity(this);
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
        //if (Global::gameMainPlayer->canDealDamage())
        {
            Global::gameMainPlayer->takeDamage(&position);
            Global::deleteEntity(this);
            visible = false;
        }
    }
}

std::vector<TexturedModel*>* Bullet::getModels()
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
    printf("Loading Bullet static models...\n");
    #endif

    ModelLoader::loadModel(&Bullet::models, "res/Models/Objects/Bullet/", "Bullet");

    setModelsRenderOrder(&Bullet::models, 3);
}

void Bullet::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Bullet static models...\n");
    #endif

    Entity::deleteModels(&Bullet::models);
}
