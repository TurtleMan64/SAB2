#ifndef EXTRAS_H
#define EXTRAS_H

class GUIText;

#include <string>

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
    GUIText* textArcadeBadClear = nullptr;
    GUIText* textArcadeGoodClear = nullptr;
    GUIText* textCompletion = nullptr;
    GUIText* textCharSelect = nullptr;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

    int getRingsComp(int lvlId);
    int getNPCComp(int lvlId);
    int getTotalEmeralds();
    int getTotalRankA();
    std::string timeToString(float time);

public:
    Extras();
    ~Extras();
    Menu* step();
    void draw() {}
    void setVisible(bool) {}
};

#endif
