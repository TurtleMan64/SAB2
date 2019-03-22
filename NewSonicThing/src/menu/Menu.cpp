#include "menu.h"
#include "../engineTester/main.h"

Menu* PopMenu::retVal = nullptr;
Menu* ClearStack::retVal = nullptr;
Menu* SwitchStack::retVal = nullptr;

Menu* PopMenu::get()
{
	if (PopMenu::retVal == nullptr)
	{
		PopMenu::retVal = new PopMenu; INCR_NEW("Menu");
	}
	return PopMenu::retVal;
}


Menu* ClearStack::get()
{
	if (ClearStack::retVal == nullptr)
	{
		ClearStack::retVal = new ClearStack; INCR_NEW("Menu");
	}
	return ClearStack::retVal;
}

Menu* SwitchStack::get()
{
	if (SwitchStack::retVal == nullptr)
	{
		SwitchStack::retVal = new SwitchStack; INCR_NEW("Menu");
	}
	return SwitchStack::retVal;
}
