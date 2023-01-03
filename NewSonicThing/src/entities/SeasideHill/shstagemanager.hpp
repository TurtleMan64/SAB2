#ifndef SH_STAGEMANAGER_H
#define SH_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class SH_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkyPass1;
    static std::vector<TexturedModel*> modelsSkyPass2;

    Dummy* skyPass2 = nullptr;

public:
    SH_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
