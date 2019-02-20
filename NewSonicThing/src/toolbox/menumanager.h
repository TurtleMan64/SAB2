#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <stack>
#include "menu.h"

class MenuManager
{
private:
	std::stack<Menu*> menuStack; // Stack for menus preceeding gameplay (main menu/config/mission menu)
	std::stack<Menu*> gameMenuStack; // Stack for menus after gameplay (pause menu/config)

	std::stack<Menu*>* currentStack;

	void pop();
	void clearStack();
	void switchStack();

public:
	MenuManager(); // Constructor initializes
	void push(Menu*); // Push a menu to the current stack
	void step(); // Run the step function for the menu at the top of the current stack
};

#endif
