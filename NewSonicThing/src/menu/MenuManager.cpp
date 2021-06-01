#include "menumanager.h"
#include "menu.h"
#include <stack>
#include <iostream>
#include "../toolbox/input.h"
#include "hud.h"
#include "mainmenu.h"
#include "../guis/guimanager.h"
#include "../engineTester/main.h"
#include "resultsscreen.h"

bool MenuManager::arcadeModeIsDone = false;
bool MenuManager::playerFailedArcadeMode = false;

MenuManager::MenuManager()
{
    //std::cout << "Initializing menu stack\n";
    this->menuStack = std::stack<Menu*>();
    this->gameStack = std::stack<Menu*>();
    this->currentStack = &this->menuStack;
    //std::cout << "Menu stack initialized\n";
}

// Push the parameter menu to the stack
void MenuManager::push(Menu* menu)
{
    //std::cout << "Pushing menu...\n";
    this->currentStack->push(menu);
    //std::cout << "Menu pushed\n";
}

// Pop the top menu from the stack
void MenuManager::pop()
{
    //std::cout << "Popping top menu...\n";
    delete this->currentStack->top(); INCR_DEL("Menu");
    this->currentStack->pop();
    //std::cout << "Top menu popped\n";
}

// Clear stack until empty
void MenuManager::clearStack()
{
    //std::cout << "Clearing the current menu stack\n";
    while (!this->currentStack->empty())
    {
        this->pop();
    }
    //std::cout << "Menu stack cleared\n";
}

// Switch current working stack
void MenuManager::switchStack()
{
    if (this->currentStack == &this->menuStack)
    {
        //std::cout << "Switching from MenuStack to GameMenuStack\n";
        this->currentStack = &this->gameStack;
        if (this->currentStack->empty())
        {
            //printf("creating HUD object, adding to stack\n");
            this->currentStack->push(new HUD); INCR_NEW("Menu");
        }
    }
    else
    {
        //std::cout << "Switching from GameMenuStack to MenuStack\n";
        this->currentStack = &this->menuStack;
        if (this->currentStack->empty())
        {
            this->currentStack->push(new MainMenu); INCR_NEW("Menu");
        }
    }
}

void MenuManager::step()
{
    //printf("MenuManager::step()\n");
    //printf("playerFailedArcadeMode = %d\n", (int)MenuManager::playerFailedArcadeMode);
    if (MenuManager::playerFailedArcadeMode) //end of arcade mode, return to main menu
    {
        //printf("end of arcade mode, return to main menu\n");
        MenuManager::playerFailedArcadeMode = false;
        clearMenuStack();
        clearGameStack();
        currentStack = &menuStack;
        currentStack->push(new MainMenu); INCR_NEW("Menu");
    }
    else if (Global::finishStageTimer >= 9.05f)
    {
        if (!Global::gameIsArcadeMode) //end of a mission, return to mission select
        {
            //printf("end of a mission, return to mission select\n");
            clearGameStack();
            currentStack = &menuStack;
        }
        else if (MenuManager::arcadeModeIsDone) //end of arcade mode, return to main menu and show results screen
        {
            //printf("end of arcade mode, return to main menu and show results screen\n");
            MenuManager::arcadeModeIsDone = false;
            clearMenuStack();
            clearGameStack();
            currentStack = &menuStack;
            currentStack->push(new MainMenu); INCR_NEW("Menu");
            currentStack->push(new ResultsScreen); INCR_NEW("Menu");
        }
    }

    // Only run if there's a menu in the working stack
    if (!this->currentStack->empty())
    {
        //printf("MenuManager about to run currentStack.top.step()\n");
        Menu* retVal = this->currentStack->top()->step();

        // If return is null, do nothing
        if (retVal != nullptr)
        {
            // Singleton menus to return stack commands
            if (retVal == PopMenu::get())
            {
                //printf("currentStack returned popped\n");
                this->pop();
            }
            else if (retVal == SwitchStack::get())
            {
                //printf("currentStack returned switched\n");
                this->switchStack();
            }
            else if (retVal == ClearStack::get())
            {
                //printf("currentStack returned clear\n");
                this->clearStack();
                this->switchStack();
            }

            // If a menu is returned, push the menu
            else if (retVal != nullptr)
            {
                //printf("currentStack returned a menu\n");
                this->push(retVal);
            }
        }
    }
}

void MenuManager::clearMenuStack()
{
    std::stack<Menu*>* prevStack = currentStack;
    currentStack = &menuStack;
    clearStack();
    currentStack = prevStack;
}

void MenuManager::clearGameStack()
{
    std::stack<Menu*>* prevStack = currentStack;
    currentStack = &gameStack;
    clearStack();
    currentStack = prevStack;
}
