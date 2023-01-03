#ifndef CE_STAGEMANAGER_H
#define CE_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class CE_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;
    bool playedBGM;

public:
    CE_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
