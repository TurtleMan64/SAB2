#ifndef IC_STAGEMANAGER_H
#define IC_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class IC_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;
    float snowSpawnTimer = 0;
    const float snowSparseness = 0.001f;
    const float snowRadius = 200.0f;
    bool firstFrame;

public:
    IC_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
