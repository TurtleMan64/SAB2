#ifndef FF_STAGEMANAGER_H
#define FF_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class FF_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    FF_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
