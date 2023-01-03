#ifndef T_STAGEMANAGER_H
#define T_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class T_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkyPass1;
    static std::vector<TexturedModel*> modelsSkyPass2;
    static std::vector<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2 = nullptr;
    Dummy* skyTransparent = nullptr;

public:
    T_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
