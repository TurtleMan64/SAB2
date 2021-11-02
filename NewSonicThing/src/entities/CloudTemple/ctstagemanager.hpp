#ifndef CT_STAGEMANAGER_H
#define CT_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class CT_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    CT_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
