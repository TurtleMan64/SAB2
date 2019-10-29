#ifndef SR_STAGEMANAGER_H
#define SR_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"

class SR_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkyPass1;
    static std::list<TexturedModel*> modelsSkyPass2;
    static std::list<TexturedModel*> modelsSkyPass3;
    static std::list<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2;
    Dummy* skyPass3;
    Dummy* skyTransparent;

public:
    SR_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
