#include "shstoneblock.hpp"
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
#include "../dummy.hpp"
#include "../../audio/audioplayer.hpp"

std::list<TexturedModel*> SH_StoneBlock::models;
std::list<TexturedModel*> SH_StoneBlock::modelsP1;
std::list<TexturedModel*> SH_StoneBlock::modelsP2;
std::list<TexturedModel*> SH_StoneBlock::modelsP3;
std::list<TexturedModel*> SH_StoneBlock::modelsP4;
std::list<TexturedModel*> SH_StoneBlock::modelsP5;
std::list<TexturedModel*> SH_StoneBlock::modelsP6;
std::list<TexturedModel*> SH_StoneBlock::modelsP7;
std::list<TexturedModel*> SH_StoneBlock::modelsP8;
std::list<TexturedModel*> SH_StoneBlock::modelsP9;
std::list<TexturedModel*> SH_StoneBlock::modelsP10;
std::list<TexturedModel*> SH_StoneBlock::modelsP11;
std::list<TexturedModel*> SH_StoneBlock::modelsP12;

CollisionModel* SH_StoneBlock::cmOriginal = nullptr;

SH_StoneBlock::SH_StoneBlock(float x, float y, float z, float /*rot*/)
{
    position.set(x, y, z);
    rotX = 0;
    rotY = 0;
    rotZ = 0;

    scale = 1;
    visible = true;

    updateTransformationMatrix();

    collideModelOriginal = SH_StoneBlock::cmOriginal;
    collideModelTransformed = collideModelOriginal->duplicateMe();

    CollisionChecker::addCollideModel(collideModelTransformed);

    updateCollisionModel();
}

void SH_StoneBlock::step()
{
    if (pieces.size() == 0)
    {
        if (Global::gameMainPlayer->canBreakObjects())
        {
            collideModelTransformed->tangible = false;

            const float hitPad = 5.0f;

            Vector3f diff = Global::gameMainPlayer->getCenterPosition() - position;
            if (std::abs(diff.x) <= 25 + hitPad &&
                std::abs(diff.z) <= 25 + hitPad &&
                      diff.y  <= 50 + hitPad &&
                      diff.y  >=  0 - hitPad)
            {
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP1));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP2));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP3));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP4));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP5));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP6));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP7));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP8));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP9));  INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP10)); INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP11)); INCR_NEW("Entity");
                pieces.push_back(new Dummy(&SH_StoneBlock::modelsP12)); INCR_NEW("Entity");

                for (Dummy* d : pieces)
                {
                    Global::addEntity(d);

                    Vector3f pos = position;
                    pos.x += 15.0f*(2*Maths::random() - 1.0f);
                    pos.z += 15.0f*(2*Maths::random() - 1.0f);
                    pos.y += 40.0f*Maths::random();

                    d->position = pos;
                    d->renderOrderOverride = 5;
                    d->updateTransformationMatrix();
                    piecesVel.push_back(Vector3f(280*Maths::random()-140, 100*Maths::random()+120, 280*Maths::random()-140));
                }

                AudioPlayer::play(0, &position);

                collideModelTransformed->tangible = false;
                visible = false;
                Global::gameMainPlayer->vel.scale(0.7f);
            }
        }
        else
        {
            collideModelTransformed->tangible = true;
        }
    }
    else
    {
        extern float dt;
        breakTimer -= dt;

        if (breakTimer <= 0.0f)
        {
            Global::deleteChunkedEntity(this);
            CollisionChecker::deleteCollideModel(collideModelTransformed);
            collideModelTransformed = nullptr;
            for (Dummy* d : pieces)
            {
                Global::deleteEntity(d);
            }
            pieces.clear();
        }
        else
        {
            for (int i = 0; i < (int)pieces.size(); i++)
            {
                Dummy* piece = pieces[i];
                piece->baseAlpha = breakTimer/0.75f;
                Vector3f vel = piecesVel[i];
                vel.y -= 600*dt;

                piece->position = piece->position + vel.scaleCopy(dt);
                piece->increaseRotation(150*dt, 120*dt, 100*dt);
                piece->updateTransformationMatrix();

                piecesVel[i] = vel;
            }
        }
    }
}

std::list<TexturedModel*>* SH_StoneBlock::getModels()
{
    return &SH_StoneBlock::models;
}

void SH_StoneBlock::loadStaticModels()
{
    if (SH_StoneBlock::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SH_StoneBlock static models...\n");
    #endif

    ModelLoader::loadModel(&SH_StoneBlock::models,    "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Block");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP1,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece1");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP2,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece2");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP3,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece3");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP4,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece4");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP5,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece5");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP6,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece6");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP7,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece7");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP8,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece8");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP9,  "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece9");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP10, "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece10");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP11, "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece11");
    ModelLoader::loadModel(&SH_StoneBlock::modelsP12, "res/Models/Levels/SeasideHill/Objects/BreakableBlock/", "Piece12");

    if (SH_StoneBlock::cmOriginal == nullptr)
    {
        SH_StoneBlock::cmOriginal = ModelLoader::loadCollisionModel("Models/Levels/SeasideHill/Objects/BreakableBlock/", "BlockCollision");
    }
}

void SH_StoneBlock::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SH_StoneBlock static models...\n");
    #endif

    Entity::deleteModels(&SH_StoneBlock::models);
    Entity::deleteModels(&SH_StoneBlock::modelsP1);
    Entity::deleteModels(&SH_StoneBlock::modelsP2);
    Entity::deleteModels(&SH_StoneBlock::modelsP3);
    Entity::deleteModels(&SH_StoneBlock::modelsP4);
    Entity::deleteModels(&SH_StoneBlock::modelsP5);
    Entity::deleteModels(&SH_StoneBlock::modelsP6);
    Entity::deleteModels(&SH_StoneBlock::modelsP7);
    Entity::deleteModels(&SH_StoneBlock::modelsP8);
    Entity::deleteModels(&SH_StoneBlock::modelsP9);
    Entity::deleteModels(&SH_StoneBlock::modelsP10);
    Entity::deleteModels(&SH_StoneBlock::modelsP11);
    Entity::deleteModels(&SH_StoneBlock::modelsP12);
    Entity::deleteCollisionModel(&SH_StoneBlock::cmOriginal);
}
