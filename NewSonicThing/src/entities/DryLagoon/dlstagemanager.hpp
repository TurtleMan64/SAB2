#ifndef DL_STAGEMANAGER_H
#define DL_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class DL_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    DL_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
