#ifndef RESULTSSCREEN_H
#define RESULTSSCREEN_H

class Button;
class GUIText;

#include <glad/glad.h>
#include <vector>
#include <string>
#include "menu.h"

class ResultsScreen : public Menu
{
private:
    void loadResources();
    void unloadResources();

    float fontSize;
    bool selectInputPrevious;
    bool backInputPrevious;
    GUIText* textArcadeModeClear = nullptr;

public:
    ResultsScreen();
    ~ResultsScreen();
    Menu* step();
};

#endif
