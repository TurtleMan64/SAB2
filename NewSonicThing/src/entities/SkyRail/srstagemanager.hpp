#ifndef SR_STAGEMANAGER_H
#define SR_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class SR_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkyPass1;
    static std::vector<TexturedModel*> modelsSkyPass2;
    static std::vector<TexturedModel*> modelsSkyPass3;
    static std::vector<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2;
    Dummy* skyPass3;
    Dummy* skyTransparent;

public:
    SR_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
