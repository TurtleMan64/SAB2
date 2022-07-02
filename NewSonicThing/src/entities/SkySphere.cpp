#include <glad/glad.h>
#include <string>

#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "skysphere.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../renderEngine/skymanager.hpp"

#include <list>
#include <iostream>
#include <string>

std::list<TexturedModel*> SkySphere::models;

SkySphere::SkySphere()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotY = 0;
    followsY = false;
    visible = true;
}

void SkySphere::step()
{
    //setRotY(SkyManager::getTimeOfDay());
    //if (Global::gamePlayer != nullptr) 
    {
        //game player was somtimes deallocated but not null? now i just set it in the player
        //position.x = Global::gamePlayer->getX();
        //position.z = Global::gamePlayer->getZ();
    }
    updateTransformationMatrix();
}

void SkySphere::setFollowsY(bool newFollowsY)
{
    followsY = newFollowsY;
}

bool SkySphere::getFollowsY()
{
    return followsY;
}

std::list<TexturedModel*>* SkySphere::getModels()
{
    return &SkySphere::models;
}

void SkySphere::loadModels(char* folder, char* objname, char* mtlname)
{
    if (SkySphere::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading sky sphere models...\n");
    #endif

    std::string path = "res/Models/";
    path = (path + folder) + "/";

    std::string objfilename = objname;
    objfilename = objfilename + ".obj";

    std::string mtlfilename = mtlname;
    mtlfilename = mtlfilename + ".mtl";

    if (ModelLoader::loadBinaryModelWithMTL(&SkySphere::models, path.c_str(), objfilename.c_str(), mtlfilename.c_str()) != 0)
    {
        ModelLoader::loadObjModelWithMTL(&SkySphere::models, path.c_str(), objfilename.c_str(), mtlfilename.c_str());
    }
}

void SkySphere::deleteModels()
{
    #ifdef DEV_MODE
    printf("Deleting sky sphere models...\n");
    #endif

    Entity::deleteModels(&SkySphere::models);
}