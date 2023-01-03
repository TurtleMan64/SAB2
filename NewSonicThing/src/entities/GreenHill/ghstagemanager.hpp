#ifndef GH_STAGEMANAGER_H
#define GH_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class GH_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkyCylinder;
    static std::vector<TexturedModel*> modelsSkyClouds;
    static std::vector<TexturedModel*> modelsWaterFlat;
    static std::vector<TexturedModel*> modelsWaterVertical;

    Dummy* skyClouds = nullptr;
    Dummy* waterFlat = nullptr;
    Dummy* waterVertical = nullptr;

public:
    GH_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
