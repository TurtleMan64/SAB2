#include <glad/glad.h>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "help.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/split.hpp"
#include "camera.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "controllableplayer.hpp"
#include "../guis/guimanager.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../audio/audioplayer.hpp"

#include <list>
#include <iostream>
#include <cstring>
#include <string>

std::vector<TexturedModel*> Help::models;
GUIText* Help::messageGUI1 = nullptr;
GUIText* Help::messageGUI2 = nullptr;

Help::Help()
{
    
}

Help::~Help()
{
    if (messageGUI1 != nullptr && (*messageGUI1->getTextString()) == myMessage1)
    {
        messageGUI1->deleteMe();
        delete messageGUI1; INCR_DEL("GUIText")
        messageGUI1 = nullptr;
    }
            
    if (messageGUI2 != nullptr && (*messageGUI2->getTextString()) == myMessage2)
    {
        messageGUI2->deleteMe();
        delete messageGUI2; INCR_DEL("GUIText")
        messageGUI2 = nullptr;
    }
}

Help::Help(float x, float y, float z, std::string message)
{
    position.set(x, y, z);
    visible = true;
    baseColor.set(Maths::nextUniform(), Maths::nextUniform(), Maths::nextUniform());
    updateTransformationMatrix();

    char msgBuf[512];
    memset(msgBuf, 0, 512);
    memcpy(msgBuf, message.c_str(), message.size());

    int splitLength = 0;
    char** msg = split(msgBuf, ';', &splitLength);

    myMessage1 = "";
    myMessage2 = "";

    if (splitLength == 1)
    {
        myMessage2 = msg[0];
    }
    else if (splitLength > 1)
    {
        myMessage1 = msg[0];
        myMessage2 = msg[1];
    }

    free(msg);
}

void Help::step()
{
    extern float dt;
    rotY += 360*dt;
    updateTransformationMatrix();

    Vector3f diff = Global::gameMainPlayer->position - position;
    float magSquared = diff.lengthSquared(); 

    if (magSquared < 20*20 && diff.y > -3 && diff.y < 12)
    {
        if (messageGUI1 == nullptr && myMessage1 != "")
        {
            float size = 3.0f/myMessage1.length();
            size = std::fminf(size, 0.09f);

            AudioPlayer::play(76, &position);

            messageGUI1 = new GUIText(myMessage1, size, Global::fontVipnagorgialla, 0.5f, 0.785f, 7, true); INCR_NEW("GUIText")
        }
            
        if (messageGUI2 == nullptr && myMessage2 != "")
        {
            float size = 3.0f/myMessage2.length();
            size = std::fminf(size, 0.09f);

            AudioPlayer::play(76, &position);

            messageGUI2 = new GUIText(myMessage2, size, Global::fontVipnagorgialla, 0.5f, 0.875f, 7, true); INCR_NEW("GUIText")
        }
    }
    else
    {
        if (messageGUI1 != nullptr && (*messageGUI1->getTextString()) == myMessage1)
        {
            messageGUI1->deleteMe();
            delete messageGUI1; INCR_DEL("GUIText")
            messageGUI1 = nullptr;
        }
            
        if (messageGUI2 != nullptr && (*messageGUI2->getTextString()) == myMessage2)
        {
            messageGUI2->deleteMe();
            delete messageGUI2; INCR_DEL("GUIText")
            messageGUI2 = nullptr;
        }
    }
}

std::vector<TexturedModel*>* Help::getModels()
{
    return &Help::models;
}

void Help::loadStaticModels()
{
    if (Help::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading Help static models...\n");
    #endif

    ModelLoader::loadModel(&Help::models, "res/Models/Objects/Help/", "Help");
}

void Help::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting Help static models...\n");
    #endif

    Entity::deleteModels(&Help::models);

    if (messageGUI1 != nullptr)
    {
        messageGUI1->deleteMe();
        delete messageGUI1; INCR_DEL("GUIText")
        messageGUI1 = nullptr;
    }

    if (messageGUI2 != nullptr)
    {
        messageGUI2->deleteMe();
        delete messageGUI2; INCR_DEL("GUIText")
        messageGUI2 = nullptr;
    }
}
