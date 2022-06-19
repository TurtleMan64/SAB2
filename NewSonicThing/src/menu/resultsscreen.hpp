#ifndef RESULTSSCREEN_H
#define RESULTSSCREEN_H

class Button;
class GUIText;

#include <glad/glad.h>
#include <vector>
#include <string>
#include "menu.hpp"

class ResultsScreen : public Menu
{
private:
    void loadResources();
    void unloadResources();

    float fontSize = 1.0f;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

    GUIText* textArcadeModeClear = nullptr;
    GUIText* textClearTime = nullptr;

public:
    ResultsScreen();
    ~ResultsScreen();
    Menu* step();
    void draw() {}
    void setVisible(bool) {}
};

#endif
