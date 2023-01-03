#ifndef DP_STAGEMANAGER_H
#define DP_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class DP_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkyPass1;
    static std::vector<TexturedModel*> modelsSkyPass2;
    static std::vector<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2 = nullptr;
    Dummy* skyTransparent = nullptr;

public:
    DP_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
