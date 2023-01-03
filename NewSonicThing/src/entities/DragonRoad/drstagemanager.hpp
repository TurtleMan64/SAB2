#ifndef DR_STAGEMANAGER_H
#define DR_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class DR_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    DR_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
