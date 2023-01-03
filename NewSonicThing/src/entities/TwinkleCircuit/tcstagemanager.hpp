#ifndef TC_STAGEMANAGER_H
#define TC_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.hpp"

class TC_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;

public:
    TC_StageManager();

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
