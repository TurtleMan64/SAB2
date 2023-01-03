#ifndef RH_STAGEMANAGER_H
#define RH_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class RH_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    RH_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
