#ifndef SM_STAGEMANAGER_H
#define SM_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.hpp"

class SM_StageManager : public Entity
{
private:
    static std::list<TexturedModel*> modelsSky0;
    static std::list<TexturedModel*> modelsSky1;
    static std::list<TexturedModel*> modelsSky2;
    static std::list<TexturedModel*> modelsSky3;
    static std::list<TexturedModel*> modelsSky4;
    static std::list<TexturedModel*> modelsSky5;
    static std::list<TexturedModel*> modelsSky6;

    Dummy* sky1 = nullptr;
    Dummy* sky2 = nullptr;
    Dummy* sky3 = nullptr;
    Dummy* sky4 = nullptr;
    Dummy* sky5 = nullptr;
    Dummy* sky6 = nullptr;

public:
    SM_StageManager();

    void step();

    std::list<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
