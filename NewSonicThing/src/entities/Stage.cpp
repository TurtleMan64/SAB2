#include "../engineTester/main.hpp"
#include "entity.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "stage.hpp"
#include "../loading/modelloader.hpp"
#include "dummy.hpp"
#include "controllableplayer.hpp"
#include "camera.hpp"

#include <list>
#include <vector>
#include <iostream>
#include <string>

std::vector<std::vector<TexturedModel*>> Stage::chunkModels;
std::vector<Dummy*> Stage::chunkDummies;
std::vector<std::vector<Vector3f>> Stage::chunkMin;
std::vector<std::vector<Vector3f>> Stage::chunkMax;

Stage::Stage()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    visible = false;
}

void Stage::respawnChunks()
{
    Stage::chunkDummies.clear();
    for (int i = 0; i < (int)Stage::chunkModels.size(); i++)
    {
        Dummy* chunk = new Dummy(&Stage::chunkModels[i]); INCR_NEW("Entity");
        chunk->visible = (true);
        chunk->setPosition(0, 0, 0);
        chunk->updateTransformationMatrix();
        Global::addEntity(chunk);
        Stage::chunkDummies.push_back(chunk);
    }
}

void Stage::updateVisibleChunks()
{
    //go through and test bounds
    for (int i = 0; i < (int)Stage::chunkDummies.size(); i++)
    {
        std::vector<Vector3f> mins = Stage::chunkMin[i];
        std::vector<Vector3f> maxs = Stage::chunkMax[i];

        Vector3f t(0, 0, 0);
        if (Global::gameMainPlayer != nullptr)
        {
            t.set(&Global::gameCamera->eye);
        }

        bool vis = false;

        for (int c = 0; c < (int)mins.size(); c++)
        {
            Vector3f min = mins[c];
            Vector3f max = maxs[c];

            if (t.x > min.x && t.y > min.y && t.z > min.z &&
                t.x < max.x && t.y < max.y && t.z < max.z)
            {
                vis = true;
            }
        }

        Stage::chunkDummies[i]->visible = (vis);
    }
}

void Stage::step()
{
    
}

std::vector<TexturedModel*>* Stage::getModels()
{
    return nullptr;
}

void Stage::loadModels(
        const char* folder, 
        std::vector<std::string>* fnames, 
        std::vector<std::vector<Vector3f>>* mins,  
        std::vector<std::vector<Vector3f>>* maxs)
{
    if (Stage::chunkModels.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading stage models...\n");
    #endif

    std::string path = "res/Models/";
    path = (path + folder) + "/";

    for (int i = 0; i < (int)fnames->size(); i++)
    {
        std::vector<TexturedModel*> model;
        Stage::chunkModels.push_back(model);

        ModelLoader::loadModel(&Stage::chunkModels[i], path, fnames->at(i));
        Stage::chunkMin.push_back(mins->at(i));
        Stage::chunkMax.push_back(maxs->at(i));
    }
}

void Stage::deleteModels()
{
    #ifdef DEV_MODE
    printf("Deleting stage models...\n");
    #endif

    for (int i = 0; i < (int)Stage::chunkModels.size(); i++)
    {
        std::vector<TexturedModel*>* models = &Stage::chunkModels[i];
        Entity::deleteModels(models);
    }

    Stage::chunkModels.clear();
    Stage::chunkDummies.clear();
    Stage::chunkMin.clear();
    Stage::chunkMax.clear();
}

std::string Stage::getName()
{
    return "stage";
}
