#ifndef BOSS_STAGEMANAGER_H
#define BOSS_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <vector>
#include "../entity.hpp"
#include "../../fontMeshCreator/guitext.hpp"

class Boss_StageManager : public Entity
{
private:
    static std::vector<TexturedModel*> modelsSkydome;
    static std::vector<TexturedModel*> modelsEarth;
    static std::vector<TexturedModel*> modelsEmerald;

    Dummy* earth = nullptr;
    Dummy* emerald1 = nullptr;
    Dummy* emerald2 = nullptr;
    Dummy* emerald3 = nullptr;
    Dummy* emerald4 = nullptr;
    Dummy* emerald5 = nullptr;
    Dummy* emerald6 = nullptr;
    Dummy* emerald7 = nullptr;
    GUIText* messageGUI1 = nullptr;
    GUIText* messageGUI2 = nullptr;
    float timer = 0.0f;

    void setMessages(std::string msg1, std::string msg2);

    Dummy* createEmerald(int i);

    void updateEmerald(Dummy* emerald, int i);

public:
    Boss_StageManager();
    ~Boss_StageManager();

    static bool foundAllEmeralds;

    void step();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
