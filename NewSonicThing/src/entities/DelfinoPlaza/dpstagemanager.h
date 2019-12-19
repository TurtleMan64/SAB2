#ifndef DP_STAGEMANAGER_H
#define DP_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"

class DP_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkyPass1;
    static std::list<TexturedModel*> modelsSkyPass2;
    static std::list<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2;
    Dummy* skyTransparent;

public:
    DP_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
