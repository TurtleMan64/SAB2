#include <glad/glad.h>

#include "../entity.hpp"
#include "bossstagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"
#include "../../toolbox/maths.hpp"
#include "../../toolbox/vector.hpp"
#include "../../fontMeshCreator/guitext.hpp"
#include "../../audio/audioplayer.hpp"
#include "../playersupersonic.hpp"

std::vector<TexturedModel*> Boss_StageManager::modelsSkydome;
std::vector<TexturedModel*> Boss_StageManager::modelsEarth;
std::vector<TexturedModel*> Boss_StageManager::modelsEmerald;
bool Boss_StageManager::foundAllEmeralds = false;

Boss_StageManager::Boss_StageManager()
{
    visible = true;
    scale = 7.0f;

    earth = new Dummy(&Boss_StageManager::modelsEarth); INCR_NEW("Entity");
    earth->visible = true;
    earth->scale = 5.0f;
    earth->position = Vector3f(0, 9000, 0);
    earth->updateTransformationMatrix();
    Global::addEntity(earth);

    int total = 0;
    for (int i = 0; i < 7; i++)
    {
        if (Global::arcadeModeEmeraldFound[i])
        {
            total += 1;
        }
    }

    //total = 7;

    if (total == 7)
    {
        Boss_StageManager::foundAllEmeralds = true;

        emerald1 = createEmerald(0);
        emerald2 = createEmerald(1);
        emerald3 = createEmerald(2);
        emerald4 = createEmerald(3);
        emerald5 = createEmerald(4);
        emerald6 = createEmerald(5);
        emerald7 = createEmerald(6);
    }
    else
    {
        Boss_StageManager::foundAllEmeralds = false;
        Global::gameLives = 0;
    }
}

Boss_StageManager::~Boss_StageManager()
{
    if (messageGUI1 != nullptr)
    {
        messageGUI1->deleteMe();
        delete messageGUI1; INCR_DEL("GUIText");
        messageGUI1 = nullptr;
    }
            
    if (messageGUI2 != nullptr)
    {
        messageGUI2->deleteMe();
        delete messageGUI2; INCR_DEL("GUIText");
        messageGUI2 = nullptr;
    }
}

void Boss_StageManager::step()
{
    extern float dt;
    float timerBefore = timer;
    timer += dt;

    //set the position of the background sky dome
    position.set(&Global::gameMainPlayer->position);
    position.y = 0;
    updateTransformationMatrix();

    earth->position.y = earth->position.y - dt*10;
    earth->updateTransformationMatrix();

    if (timerBefore < 0.9f && timer >= 0.9f)
    {
        setMessages("The ARK is on a collision course with the Earth...", "Use the Chaos Emeralds to escape and stop it!");
    }

    if (Boss_StageManager::foundAllEmeralds)
    {
        if (timerBefore < 12.0f && timer >= 12.0f)
        {
            if (messageGUI1 != nullptr)
            {
                messageGUI1->deleteMe();
                delete messageGUI1; INCR_DEL("GUIText");
                messageGUI1 = nullptr;
            }
            
            if (messageGUI2 != nullptr)
            {
                messageGUI2->deleteMe();
                delete messageGUI2; INCR_DEL("GUIText");
                messageGUI2 = nullptr;
            }
        }

        if (timer > 12.0f && timer < 20.0f)
        {
            updateEmerald(emerald1, 0);
            updateEmerald(emerald2, 1);
            updateEmerald(emerald3, 2);
            updateEmerald(emerald4, 3);
            updateEmerald(emerald5, 4);
            updateEmerald(emerald6, 5);
            updateEmerald(emerald7, 6);
        }

        if (timerBefore < 20.0f && timer >= 20.0f)
        {
            emerald1->visible = false;
            emerald2->visible = false;
            emerald3->visible = false;
            emerald4->visible = false;
            emerald5->visible = false;
            emerald6->visible = false;
            emerald7->visible = false;

            Vector3f pPos = Global::gameMainPlayer->position;
            Vector3f pCamDir = Global::gameMainPlayer->camDir;
            Vector3f pCamDirSmooth = Global::gameMainPlayer->camDirSmooth;

            Global::gameMainPlayer->position.y = -100000.0f;
            Global::gameMainPlayer->animate();
            delete Global::gameMainPlayer; INCR_DEL("Entity");

            Global::gameMainPlayer = new PlayerSuperSonic(pPos.x, pPos.y, pPos.z);
            Global::gameMainPlayer->camDir = pCamDir;
            Global::gameMainPlayer->camDirSmooth = pCamDirSmooth;
        }
    }
    else
    {
        if (timerBefore < 12.0f && timer >= 12.0f)
        {
            setMessages("What?", "You didn't find all the Chaos Emeralds???");

            AudioPlayer::stopBGM();
        }

        if (timerBefore < 20.0f && timer >= 20.0f)
        {
            Global::gameMainPlayer->takeDamage(&position);
        }
    }

    if (timer > 20.0f)
    {
        if (messageGUI1 != nullptr)
        {
            messageGUI1->deleteMe();
            delete messageGUI1; INCR_DEL("GUIText");
            messageGUI1 = nullptr;
        }
            
        if (messageGUI2 != nullptr)
        {
            messageGUI2->deleteMe();
            delete messageGUI2; INCR_DEL("GUIText");
            messageGUI2 = nullptr;
        }
    }
}

void Boss_StageManager::setMessages(std::string msg1, std::string msg2)
{
    if (messageGUI1 != nullptr)
    {
        messageGUI1->deleteMe();
        delete messageGUI1; INCR_DEL("GUIText");
        messageGUI1 = nullptr;
    }
            
    if (messageGUI2 != nullptr)
    {
        messageGUI2->deleteMe();
        delete messageGUI2; INCR_DEL("GUIText");
        messageGUI2 = nullptr;
    }

    float size = 3.0f/msg1.length();
    size = std::fminf(size, 0.1f);
    messageGUI1 = new GUIText(msg1, size, Global::fontVipnagorgialla, 0.5f, 0.75f, 4, true); INCR_NEW("GUIText");

    size = 3.0f/msg2.length();
    size = std::fminf(size, 0.1f);
    messageGUI2 = new GUIText(msg2, size, Global::fontVipnagorgialla, 0.5f, 0.85f, 4, true); INCR_NEW("GUIText");
}

Dummy* Boss_StageManager::createEmerald(int i)
{
    Dummy* emerald = new Dummy(&Boss_StageManager::modelsEmerald); INCR_NEW("Entity");
    emerald->visible = false;
    emerald->position = Vector3f(0, 0, 0);
    emerald->updateTransformationMatrix();
    Global::addEntity(emerald);

    switch (i)
    {
        case 0: emerald->baseColor.set( 36, 211,   0); break;
        case 1: emerald->baseColor.set(248,   0, 236); break;
        case 2: emerald->baseColor.set( 20, 196, 249); break;
        case 3: emerald->baseColor.set(240,   0,   0); break;
        case 4: emerald->baseColor.set(248, 204,   0); break;
        case 5: emerald->baseColor.set(  3,   0, 248); break;
        case 6: emerald->baseColor.set(185, 185, 185); break;
        default: break;
    }
    emerald->baseColor.scale(0.00392156862f); //divide by 255

    return emerald;
}

void Boss_StageManager::updateEmerald(Dummy* emerald, int i)
{
    Vector3f off(30*(20 - timer), 7, 0);
    off = Maths::rotatePoint(&off, &Y_AXIS, timer + 2*Maths::PI*(i/7.0f));

    emerald->visible = true;
    emerald->position = Global::gameMainPlayer->position + off;
    emerald->updateTransformationMatrix();
}

std::vector<TexturedModel*>* Boss_StageManager::getModels()
{
    return &Boss_StageManager::modelsSkydome;
}

void Boss_StageManager::loadStaticModels()
{
    if (Boss_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Boss_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&Boss_StageManager::modelsSkydome, "res/Models/Levels/Boss/Sky/", "Sky");
    ModelLoader::loadModel(&Boss_StageManager::modelsEarth,   "res/Models/Levels/Boss/Sky/", "Earth");
    ModelLoader::loadModel(&Boss_StageManager::modelsEmerald, "res/Models/Objects/ChaosEmerald/", "ChaosEmerald");

}

void Boss_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Boss_StageManager static models...\n");
    #endif

    Entity::deleteModels(&Boss_StageManager::modelsSkydome);
    Entity::deleteModels(&Boss_StageManager::modelsEarth);
    Entity::deleteModels(&Boss_StageManager::modelsEmerald);
}
