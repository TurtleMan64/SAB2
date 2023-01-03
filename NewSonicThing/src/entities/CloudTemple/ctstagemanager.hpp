#ifndef CT_STAGEMANAGER_H
#define CT_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class CT_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    CT_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
