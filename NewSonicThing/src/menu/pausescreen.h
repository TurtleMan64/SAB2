#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

class GUIText;
class HUD;

#include "menu.h"

class PauseScreen : public Menu
{
private:
    int menuSelection;
    int menuSelectionMAX;
    int menuDisplayID;

    int moveYPrevious;
    bool selectInputPrevious;
    bool backInputPrevious;

    GUIText* textCursor;
    GUIText* textResume;
    GUIText* textCamera;
    GUIText* textRestart;
    GUIText* textQuit;

    bool pausedSounds[14];

    float size;
    void selectButton();
    void setVisible(bool visible);

    HUD* gameHud;

public:
    PauseScreen(HUD* gameHud);

    ~PauseScreen();

    Menu* step();
};

#endif
