#include "menumanager.h"
#include "menu.h"
#include <stack>
#include <iostream>

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
		if (this->menuStack.top()->step()) // If step returns 1, pop.
			this->pop();
	}
}