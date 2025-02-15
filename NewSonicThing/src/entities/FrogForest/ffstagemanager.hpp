#ifndef FF_STAGEMANAGER_H
#define FF_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class FF_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    FF_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
