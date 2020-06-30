#ifndef TC_STAGEMANAGER_H
#define TC_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class TC_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    TC_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
