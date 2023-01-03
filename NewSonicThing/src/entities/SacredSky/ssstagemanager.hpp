#ifndef SS_STAGEMANAGER_H
#define SS_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class SS_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    SS_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
