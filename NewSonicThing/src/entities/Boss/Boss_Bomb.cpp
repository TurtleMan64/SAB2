#include "bossbomb.hpp"
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
#include "../../menu/timer.hpp"

std::vector<TexturedModel*> Boss_Bomb::models;
int Boss_Bomb::bombsRemaining = 0;

Boss_Bomb::Boss_Bomb(float x, float y, float z, float rotY)
{
    Boss_Bomb::bombsRemaining++;

    position.set(x, y, z);
    rotX = 0;
    this->rotY = rotY;
    rotZ = 0;

    this->rotY = 360*Maths::random();

    scale = 1;
    visible = true;

    updateTransformationMatrix();

    c1p1 = Vector3f( 198.632f,   53.73f, -59.3377f);
    c1p2 = Vector3f(-198.632f,   53.73f, -59.3377f);
    c2p1 = Vector3f( 198.632f,   53.73f,  59.3377f);
    c2p2 = Vector3f(-198.632f,   53.73f,  59.3377f);
    c3p1 = Vector3f( 198.632f, 155.637f,   0.0000f);
    c3p2 = Vector3f(-198.632f, 155.637f,   0.0000f);

    c1p1 = Maths::rotatePoint(&c1p1, &Y_AXIS, Maths::toRadians(this->rotY));
    c1p2 = Maths::rotatePoint(&c1p2, &Y_AXIS, Maths::toRadians(this->rotY));
    c2p1 = Maths::rotatePoint(&c2p1, &Y_AXIS, Maths::toRadians(this->rotY));
    c2p2 = Maths::rotatePoint(&c2p2, &Y_AXIS, Maths::toRadians(this->rotY));
    c3p1 = Maths::rotatePoint(&c3p1, &Y_AXIS, Maths::toRadians(this->rotY));
    c3p2 = Maths::rotatePoint(&c3p2, &Y_AXIS, Maths::toRadians(this->rotY));

    c1p1 = c1p1 + position;
    c1p2 = c1p2 + position;
    c2p1 = c2p1 + position;
    c2p2 = c2p2 + position;
    c3p1 = c3p1 + position;
    c3p2 = c3p2 + position;
}

Boss_Bomb::~Boss_Bomb()
{
    Boss_Bomb::bombsRemaining--;
}

void Boss_Bomb::step()
{
    float s = 2.0f + sinf(Global::gameClock*5);
    baseColor = Vector3f(s, s, s);

    if (Maths::pointIsInCylinder(&Global::gameMainPlayer->position, &c1p1, &c1p2, 60.0f) ||
        Maths::pointIsInCylinder(&Global::gameMainPlayer->position, &c2p1, &c2p2, 60.0f) ||
        Maths::pointIsInCylinder(&Global::gameMainPlayer->position, &c3p1, &c3p2, 60.0f))
    {
        //explode and die
        AudioPlayer::play(73, &position);
        
        const float height = 240.0f;
        const float spread = 280.0f;
        
        Vector3f spd(0, 0, 0);
        
        for (int i = 90; i != 0; i--)
        {
            Vector3f pos(
                position.x + spread*(Maths::random() - 0.5f),
                position.y + spread*(Maths::random() - 0.5f) + height,
                position.z + spread*(Maths::random() - 0.5f));
        
            ParticleMaster::createParticle(ParticleResources::textureExplosion1, &pos, &spd, 0, 1.55f, 32, 0, false, false, 1.0f, true);
        }

        if (Boss_Bomb::bombsRemaining <= 1)
        {
            Global::finishStageTimer = 0;
            if (Global::mainHudTimer != nullptr)
            {
                Global::mainHudTimer->freeze(true);
            }
        }
        
        Global::deleteEntity(this);
        visible = false;
    }
}

std::vector<TexturedModel*>* Boss_Bomb::getModels()
{
    return &Boss_Bomb::models;
}

void Boss_Bomb::loadStaticModels()
{
    if (Boss_Bomb::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Boss_Bomb static models...\n");
    #endif

    ModelLoader::loadModel(&Boss_Bomb::models, "res/Models/Objects/Dynamite/", "Dynamite");
}

void Boss_Bomb::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Boss_Bomb static models...\n");
    #endif

    Entity::deleteModels(&Boss_Bomb::models);
}
