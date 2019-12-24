#ifndef PC_STAGEMANAGER_H
#define PC_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class PC_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    PC_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
