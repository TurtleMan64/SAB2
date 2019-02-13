#include "menumanager.h"
#include "menu.h"
#include <stack>
#include <iostream>
#include "input.h"
#include "pausescreen.h"
#include "missionmenu.h"

MenuManager::MenuManager()
{
	std::cout << "Initializing menu stack\n";
	this->menuStack = std::stack<Menu*>();
	std::cout << "Menu stack initialized\n";
}

void MenuManager::push(Menu* menu)
{
	std::cout << "Pushing menu...\n";
	this->menuStack.push(menu);
	std::cout << "Menu pushed\n";
}

void MenuManager::pop()
{
	std::cout << "Popping top menu...\n";
	delete this->menuStack.top();
	this->menuStack.pop();
	std::cout << "Top menu popped\n";
}

void MenuManager::step()
{
	if (!this->menuStack.empty())
	{
		int retVal = this->menuStack.top()->step();
		if (retVal) // If step returns 1, pop.
		{
			this->pop();
			if (retVal == 2)
			{
				this->push(new MissionMenu);
			}
		}
	}
	else
	{
		if (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START)
		{
			this->push(new PauseScreen());
		}
	}
}