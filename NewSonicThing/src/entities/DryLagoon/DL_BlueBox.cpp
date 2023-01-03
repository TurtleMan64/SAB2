#include "dlbluebox.hpp"
#include "../entity.hpp"
#include "../camera.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"
#include "../../collision/collisionmodel.hpp"
#include "../../collision/collisionchecker.hpp"
#include "../controllableplayer.hpp"
#include "../../particles/particleresources.hpp"
#include "../../particles/particle.hpp"
#include "../../particles/particlemaster.hpp"

std::vector<TexturedModel*> DL_BlueBox::models;
CollisionModel* DL_BlueBox::cmOriginal = nullptr;

DL_BlueBox::DL_BlueBox(float x, float y, float z, float rot)
{
    position.set(x, y, z);
    rotX = 0;
    rotY = rot;
    rotZ = 0;

    scale = 1;
    visible = true;

    updateTransformationMatrix();

    collideModelOriginal = DL_BlueBox::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void DL_BlueBox::step()
{
    if (Global::gameMainPlayer->canBreakObjects())
    {
        collideModelTransformed->tangible = false;

        const float hitPad = 5.0f;

        Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;
        if (std::abs(diff.x) <= 10 + hitPad &&
            std::abs(diff.z) <= 10 + hitPad &&
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
                    position.x + spread*(Maths::random() - 0.5f),
                    position.y + spread*(Maths::random() - 0.5f) + height,
                    position.z + spread*(Maths::random() - 0.5f));

                ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &spd, 0, 1.55f, 3*Maths::random() + 8, 0, false, false, 1.0f, true);
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

std::vector<TexturedModel*>* DL_BlueBox::getModels()
{
    return &DL_BlueBox::models;
}

void DL_BlueBox::loadStaticModels()
{
    if (DL_BlueBox::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading DL_BlueBox static models...\n");
    #endif

    ModelLoader::loadModel(&DL_BlueBox::models, "res/Models/Levels/DryLagoon/Objects/", "Box");

    if (DL_BlueBox::cmOriginal == nullptr)
    {
        DL_BlueBox::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/DryLagoon/Objects/", "Box");
    }
}

void DL_BlueBox::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting DL_BlueBox static models...\n");
    #endif

    Entity::deleteModels(&DL_BlueBox::models);
    Entity::deleteCollisionModel(&DL_BlueBox::cmOriginal);
}
