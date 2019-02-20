#include "menumanager.h"
#include "menu.h"
#include <stack>
#include <iostream>
#include "input.h"
#include "hud.h"
#include "mainmenu.h"
#include "../guis/guimanager.h"

MenuManager::MenuManager()
{
	std::cout << "Initializing menu stack\n";
	this->menuStack = std::stack<Menu*>();
	this->gameMenuStack = std::stack<Menu*>();
	this->currentStack = &this->menuStack;
	std::cout << "Menu stack initialized\n";
}

// Push the parameter menu to the stack
void MenuManager::push(Menu* menu)
{
	std::cout << "Pushing menu...\n";
	this->currentStack->push(menu);
	std::cout << "Menu pushed\n";
}

// Pop the top menu from the stack
void MenuManager::pop()
{
	std::cout << "Popping top menu...\n";
	delete this->currentStack->top();
	this->currentStack->pop();
	std::cout << "Top menu popped\n";
}

// Clear stack until empty
void MenuManager::clearStack()
{
	std::cout << "Clearing the current menu stack\n";
	while (!this->currentStack->empty())
	{
		this->pop();
	}
	std::cout << "Menu stack cleared\n";
}

// Switch current working stack
void MenuManager::switchStack()
{
	if (this->currentStack == &this->menuStack)
	{
		std::cout << "Switching from MenuStack to GameMenuStack\n";
		this->currentStack = &this->gameMenuStack;
		if (this->currentStack->empty())
		{
			this->currentStack->push(new HUD());
		}
	}
	else
	{
		std::cout << "Switching from GameMenuStack to MenuStack\n";
		this->currentStack = &this->menuStack;
		if (this->currentStack->empty())
		{
			this->currentStack->push(new MainMenu());
		}
	}
}

void MenuManager::step()
{
	// Only run if there's a menu in the working stack
	if (!this->currentStack->empty())
	{
		Menu* retVal = this->currentStack->top()->step();

		// If return is null, do nothing
		if (retVal != nullptr)
		{
			// Singleton menus to return stack commands
			if (retVal == PopMenu::get())
			{
				this->pop();
			}
			else if (retVal == SwitchStack::get())
			{
				this->switchStack();
			}
			else if (retVal == ClearStack::get())
			{
				this->clearStack();
				this->switchStack();
			}

			// If a menu is returned, push the menu
			else if (retVal != nullptr)
			{
				this->push(retVal);
			}
		}
	}
}