#ifndef GF_STAGEMANAGER_H
#define GF_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class GF_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;
    float pollenSpawnTimer = 0;
    const float pollenSparseness = 0.001f;
    const float pollenRadius = 200.0f;
    bool firstFrame;

public:
    GF_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
