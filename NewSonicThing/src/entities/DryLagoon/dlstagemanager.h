#ifndef DL_STAGEMANAGER_H
#define DL_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class DL_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    DL_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
