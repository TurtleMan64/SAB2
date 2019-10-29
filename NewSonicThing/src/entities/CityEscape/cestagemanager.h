#ifndef CE_STAGEMANAGER_H
#define CE_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class CE_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;
    bool playedBGM;

public:
    CE_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
