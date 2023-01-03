#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "chaosemerald.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/split.hpp"
#include "camera.hpp"
#include "controllableplayer.hpp"
#include "../guis/guimanager.hpp"
#include "../audio/audioplayer.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> ChaosEmerald::models;

ChaosEmerald::ChaosEmerald()
{
    
}

ChaosEmerald::ChaosEmerald(float x, float y, float z, int id)
{
    position.set(x, y, z);
    visible = true;
    this->id = id;
    updateTransformationMatrix();

    std::string lookup = "ChaosEmerald_" + std::to_string(id);

    found = false;
    if (Global::gameSaveData.find(lookup) != Global::gameSaveData.end())
    {
        std::string isFound = Global::gameSaveData[lookup];
        if (isFound == "true")
        {
            found = true;
        }
    }

    switch (id)
    {
        case 0: baseColor.set(36,  211, 0);   break;
        case 1: baseColor.set(248, 0,   236); break;
        case 2: baseColor.set(20,  196, 249); break;
        case 3: baseColor.set(240, 0,   0);   break;
        case 4: baseColor.set(248, 204, 0);   break;
        case 5: baseColor.set(3,   0,   248); break;
        case 6: baseColor.set(185, 185, 185); break;
        default: break;
    }
    baseColor.scale(0.00392156862f); //divide by 255

    if (found)
    {
        baseAlpha = 0.2f;
        renderOrderOverride = 2;
    }

    if (Global::currentCharacterType != Global::PlayableCharacter::Sonic &&
        Global::currentCharacterType != Global::PlayableCharacter::Knuckles)
    {
        //Global::deleteEntity(this);
        //Global::deleteEntity(glasses);
        //return;
    }
}

void ChaosEmerald::step()
{
    Vector3f diff = Global::gameMainPlayer->position - position;

    if (std::abs(diff.x) < 50 &&
        std::abs(diff.z) < 50 &&
        std::abs(diff.y) < 50)
    {
        diff = Global::gameMainPlayer->getCenterPosition() - position;
        if (diff.lengthSquared() < 6.0f*6.0f + 4.4f*4.4f)
        {
            if (!found)
            {
                AudioPlayer::play(7, &position);
                found = true;

                std::string lookup = "ChaosEmerald_" + std::to_string(id);
                Global::gameSaveData[lookup] = "true";
                Global::saveSaveData();

                Global::deleteEntity(this);
                return;
            }
        }
    }

    extern float dt;
    rotY += 240*dt;
    bobTimer += dt;

    Vector3f posBob(&position);
    posBob.y += 0.75f*sinf((bobTimer*Maths::PI)*2.0f);
    Maths::createTransformationMatrix(&transformationMatrix, &posBob, 0, rotY, 0, 0, 2.5f);
}

std::vector<TexturedModel*>* ChaosEmerald::getModels()
{
    return &ChaosEmerald::models;
}

void ChaosEmerald::loadStaticModels()
{
    if (ChaosEmerald::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading ChaosEmerald static models...\n");
    #endif

    ModelLoader::loadModel(&ChaosEmerald::models, "res/Models/Objects/ChaosEmerald/", "ChaosEmerald");
}

void ChaosEmerald::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting ChaosEmerald static models...\n");
    #endif

    Entity::deleteModels(&ChaosEmerald::models);
}
