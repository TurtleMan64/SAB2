#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <stack>
#include "menu.h"

class MenuManager
{
private:
	std::stack<Menu*> menuStack;

	void pop();

public:
	MenuManager();
	void push(Menu*);
	void step();
};

#endif
