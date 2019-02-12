#ifndef MENU_H
#define MENU_H

class Menu
{
public:
	virtual int step() = 0;
	virtual ~Menu()
	{
	}
};

#endif