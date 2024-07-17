#include "bosscapsule.hpp"
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
#include "../../audio/audioplayer.hpp"

std::vector<TexturedModel*> Boss_Capsule::models;
CollisionModel* Boss_Capsule::cmOriginal = nullptr;

Boss_Capsule::Boss_Capsule(float x, float y, float z)
{
    position.set(x, y, z);
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    scale = 1;
    visible = true;

    updateTransformationMatrix();

    collideModelOriginal = Boss_Capsule::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void Boss_Capsule::step()
{
    extern float dt;

    timer += dt;

    if (timer > 20.0f)
    {
        //explode and die
        AudioPlayer::play(73, &position);

        const float height = 18.0f;
        const float spread = 46.0f;

        Vector3f spd(0, 0, 0);

        for (int i = 60; i != 0; i--)
        {
            Vector3f pos(
                position.x + spread*(Maths::random() - 0.5f),
                position.y + spread*(Maths::random() - 0.5f) + height,
                position.z + spread*(Maths::random() - 0.5f));

            ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &spd, 0, 1.55f, 3*Maths::random() + 8, 0, false, false, 1.0f, true);
        }

        CollisionChecker::deleteCollideModel(collideModelTransformed);
        Global::deleteEntity(this);
        collideModelTransformed = nullptr;
        visible = false;
    }
}

std::vector<TexturedModel*>* Boss_Capsule::getModels()
{
    return &Boss_Capsule::models;
}

void Boss_Capsule::loadStaticModels()
{
    if (Boss_Capsule::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Boss_Capsule static models...\n");
    #endif

    ModelLoader::loadModel(&Boss_Capsule::models, "res/Models/Levels/Boss/Capsule/", "Capsule");

    if (Boss_Capsule::cmOriginal == nullptr)
    {
        Boss_Capsule::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/Boss/Capsule/", "Collision");
    }
}

void Boss_Capsule::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Boss_Capsule static models...\n");
    #endif

    Entity::deleteModels(&Boss_Capsule::models);
    Entity::deleteCollisionModel(&Boss_Capsule::cmOriginal);
}
