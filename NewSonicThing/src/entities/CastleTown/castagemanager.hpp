#ifndef CA_STAGEMANAGER_H
#define CA_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class CA_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;
    float rainSpawnTimer = 0;
    const float rainSparseness = 0.001f;
    const float rainRadius = 200.0f;
    bool firstFrame;

public:
    CA_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
