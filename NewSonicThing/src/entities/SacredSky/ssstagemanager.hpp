#ifndef SS_STAGEMANAGER_H
#define SS_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class SS_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSkydome;

public:
    SS_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
