#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

class GUIText;
class HUD;

#include "menu.h"

class PauseScreen : public Menu
{
private:
    int menuSelection = 0;
    int menuSelectionMAX = 3;

    float size = 0.075f;

    int moveYPrevious = 0;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

    GUIText* textCursor = nullptr;
    GUIText* textResume = nullptr;
    GUIText* textCamera = nullptr;
    GUIText* textRestart = nullptr;
    GUIText* textQuit = nullptr;

    bool pausedSounds[14];

    void selectButton();
    void setVisible(bool visible);

public:
    PauseScreen();

    ~PauseScreen();

    Menu* step();
};

#endif
