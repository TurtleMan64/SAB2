#ifndef MH_STAGEMANAGER_H
#define MH_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class MH_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;
    float timer;
    bool playedBGM;

public:
    MH_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
