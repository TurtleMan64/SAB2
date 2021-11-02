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

    float fontSize;
    bool selectInputPrevious;
    bool backInputPrevious;

    GUIText* textArcadeModeClear = nullptr;
    GUIText* textClearTime = nullptr;

public:
    ResultsScreen();
    ~ResultsScreen();
    Menu* step();
};

#endif
