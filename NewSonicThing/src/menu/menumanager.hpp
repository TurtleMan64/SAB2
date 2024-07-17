#ifndef MENUMANAGER_H
#define MENUMANAGER_H

class Menu;

#include <stack>

class MenuManager
{
private:
    std::stack<Menu*> menuStack; // Stack for menus preceeding gameplay (main menu/config/mission menu)
    std::stack<Menu*> gameStack; // Stack for menus after gameplay (hud/pause menu/config)

    std::stack<Menu*>* currentStack;

    void pop();
    void clearStack();
    void switchStack();

    void clearMenuStack();
    void clearGameStack();

public:
    static bool arcadeModeIsDoneBadEnding;
    static bool arcadeModeIsDoneGoodEnding;
    static bool playerFailedArcadeMode;
    static bool justFinishedArcadeMode;
    MenuManager(); // Constructor initializes
    void push(Menu* menu); // Push a menu to the current stack
    void step(); // Run the step function for the menu at the top of the current stack
};

#endif
