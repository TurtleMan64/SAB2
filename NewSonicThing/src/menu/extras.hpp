#ifndef EXTRAS_H
#define EXTRAS_H

class GUIText;

#include <glad/glad.h>
#include "menu.hpp"

class Extras : public Menu
{
private:
    GUIText* textTime = nullptr;
    GUIText* textRingsAccum = nullptr;
    GUIText* textRingsComp = nullptr;
    GUIText* textNPC = nullptr;
    GUIText* textEmeralds = nullptr;
    GUIText* textRank = nullptr;
    GUIText* textArcadeClear = nullptr;
    GUIText* textCompletion = nullptr;
    GUIText* textCharSelect = nullptr;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

    int getRingsComp(int lvlId);
    int getNPCComp(int lvlId);
    int getTotalEmeralds();
    int getTotalRankA();

public:
    Extras();
    ~Extras();
    Menu* step();
    void draw() {}
    void setVisible(bool) {}
};

#endif
