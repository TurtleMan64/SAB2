#ifndef STAGE_H
#define STAGE_H

class TexturedModel;

#include <list>
#include <vector>
#include "entity.h"
#include "dummy.h"
#include "../toolbox/vector.h"
#include <string>

class Stage : public Entity
{
private:
    static std::vector<std::list<TexturedModel*>> chunkModels;
    static std::vector<Dummy*> chunkDummies;
    static std::vector<std::vector<Vector3f>> chunkMin;
    static std::vector<std::vector<Vector3f>> chunkMax;

public:
    Vector3f finishPlayerPosition;
    float finishPlayerRotY;
    float finishCameraPitch;

    Stage();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadModels(
        const char* folder, 
        std::vector<std::string>* fnames, 
        std::vector<std::vector<Vector3f>>* mins,  
        std::vector<std::vector<Vector3f>>* maxs);

    static void respawnChunks();

    static void updateVisibleChunks();

    static void deleteModels();

    std::string getName();
};
#endif
