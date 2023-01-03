#ifndef PC_STAGEMANAGER_H
#define PC_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class PC_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    PC_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
