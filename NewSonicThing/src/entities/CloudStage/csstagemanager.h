#ifndef CS_STAGEMANAGER_H
#define CS_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class CS_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    CS_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
