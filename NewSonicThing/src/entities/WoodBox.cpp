#include "woodbox.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "../engineTester/main.hpp"
#include "../models/models.hpp"
#include "../loading/modelloader.hpp"
#include "../toolbox/maths.hpp"
#include "../collision/collisionmodel.hpp"
#include "../collision/collisionchecker.hpp"
#include "controllableplayer.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particle.hpp"
#include "../particles/particlemaster.hpp"

std::list<TexturedModel*> WoodBox::models;
CollisionModel* WoodBox::cmOriginal = nullptr;

WoodBox::WoodBox(float x, float y, float z, float /*rot*/)
{
    position.set(x, y, z);
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    scale = 1;
    visible = true;

    updateTransformationMatrix();

    collideModelOriginal = WoodBox::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void WoodBox::step()
{
    if (Global::gameMainPlayer->canBreakObjects())
    {
        collideModelTransformed->tangible = false;

        const float hitPad = 5.0f;

        Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;
        if (fabsf(diff.x) <= 10 + hitPad &&
            fabsf(diff.z) <= 10 + hitPad &&
                  diff.y  <= 20 + hitPad &&
                  diff.y  >=  0 - hitPad)
        {
            //explode and die

            const float height = 10.0f;
            const float spread = 20.0f;

            Vector3f spd(0, 0, 0);

            for (int i = 12; i != 0; i--)
            {
                Vector3f pos(
                    getX() + spread*(Maths::random() - 0.5f),
                    getY() + spread*(Maths::random() - 0.5f) + height,
                    getZ() + spread*(Maths::random() - 0.5f));

                ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &spd, 0, 1.55f, 0, 3*Maths::random() + 8, 0, false, false, 1.0f, true);
            }

            CollisionChecker::deleteCollideModel(collideModelTransformed);
            Global::deleteChunkedEntity(this);
            collideModelTransformed = nullptr;
            visible = false;
            Global::gameMainPlayer->vel.scale(0.8f);
        }
    }
    else
    {
        collideModelTransformed->tangible = true;
    }
}

std::list<TexturedModel*>* WoodBox::getModels()
{
    return &WoodBox::models;
}

void WoodBox::loadStaticModels()
{
    if (WoodBox::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading WoodBox static models...\n");
    #endif

    ModelLoader::loadModel(&WoodBox::models, "res/Models/Objects/WoodBox/", "WoodBox");

    if (WoodBox::cmOriginal == nullptr)
    {
        WoodBox::cmOriginal = ModelLoader::loadCollisionModel("Models/Objects/WoodBox/", "WoodBox");
    }
}

void WoodBox::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting WoodBox static models...\n");
    #endif

    Entity::deleteModels(&WoodBox::models);
    Entity::deleteCollisionModel(&WoodBox::cmOriginal);
}
