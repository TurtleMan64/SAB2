#ifndef GH_STAGEMANAGER_H
#define GH_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"

class GH_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkyCylinder;
    static std::list<TexturedModel*> modelsSkyClouds;
    static std::list<TexturedModel*> modelsWaterFlat;
    static std::list<TexturedModel*> modelsWaterVertical;

    Dummy* skyClouds = nullptr;
    Dummy* waterFlat = nullptr;
    Dummy* waterVertical = nullptr;

public:
    GH_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
