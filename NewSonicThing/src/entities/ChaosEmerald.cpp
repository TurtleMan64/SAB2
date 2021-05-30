#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "chaosemerald.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../toolbox/maths.h"
#include "../toolbox/split.h"
#include "camera.h"
#include "controllableplayer.h"
#include "../guis/guimanager.h"
#include "../audio/audioplayer.h"

#include <list>
#include <iostream>

std::list<TexturedModel*> ChaosEmerald::models;

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
        case 0: baseColour.set(36,  211, 0);   break;
        case 1: baseColour.set(248, 0,   236); break;
        case 2: baseColour.set(20,  196, 249); break;
        case 3: baseColour.set(240, 0,   0);   break;
        case 4: baseColour.set(248, 204, 0);   break;
        case 5: baseColour.set(3,   0,   248); break;
        case 6: baseColour.set(185, 185, 185); break;
        default: break;
    }
    baseColour.scale(0.00392156862f); //divide by 255

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

    if (fabsf(diff.x) < 50 &&
        fabsf(diff.z) < 50 &&
        fabsf(diff.y) < 50)
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

std::list<TexturedModel*>* ChaosEmerald::getModels()
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
    std::fprintf(stdout, "Loading ChaosEmerald static models...\n");
    #endif

    ObjLoader::loadModel(&ChaosEmerald::models, "res/Models/Objects/ChaosEmerald/", "ChaosEmerald");
}

void ChaosEmerald::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting ChaosEmerald static models...\n");
    #endif

    Entity::deleteModels(&ChaosEmerald::models);
}
