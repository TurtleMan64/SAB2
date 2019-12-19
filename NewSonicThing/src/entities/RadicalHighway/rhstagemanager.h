#ifndef RH_STAGEMANAGER_H
#define RH_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class RH_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    RH_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
