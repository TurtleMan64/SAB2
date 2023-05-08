#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "switch.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../entities/playersonic.hpp"
#include "../collision/collisionchecker.hpp"
#include "../collision/triangle3d.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particlemaster.hpp"
#include "shieldmagnet.hpp"
#include "ringmoving.hpp"
#include "../toolbox/matrix.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> Switch::models;

Switch::Switch()
{

}

Switch::Switch(float x, float y, float z, int idx)
{
    position.x = x;
    position.y = y;
    position.z = z;
    id = idx;

    baseColor = Vector3f(0.9f, 0.9f, 0.9f);

    updateTransformationMatrix();

    position.y += 5; //center of the switch ball
}

void Switch::step()
{
    if (std::abs(position.y - Global::gameMainPlayer->position.y) < 40 &&
        std::abs(position.z - Global::gameMainPlayer->position.z) < 40 &&
        std::abs(position.x - Global::gameMainPlayer->position.x) < 40 &&
        activated == false)
    {
        float toPlayerDistSquared = (Global::gameMainPlayer->getCenterPosition() - position).lengthSquared();
        if (toPlayerDistSquared < (11.0f*11.0f))
        {
            AudioPlayer::play(79, &position);

            activated = true;

            baseColor = Vector3f(1.2f, 1.2f, 1.2f);

            extern std::unordered_set<Entity*> gameEntities;

            for (Entity* e : gameEntities)
            {
                e->activateFromSwitch(id);
            }
        }
    }
}

std::vector<TexturedModel*>* Switch::getModels()
{
    return &Switch::models;
}

void Switch::loadStaticModels()
{
    if (Switch::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Switch static models...\n");
    #endif

    ModelLoader::loadModel(&Switch::models, "res/Models/Objects/Switch/", "Switch");
}

void Switch::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Switch static models...\n");
    #endif

    Entity::deleteModels(&Switch::models);
}
