#ifndef DR_STAGEMANAGER_H
#define DR_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class DR_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    DR_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
