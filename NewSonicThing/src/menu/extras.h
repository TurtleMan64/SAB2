#ifndef EXTRAS_H
#define EXTRAS_H

class GUIText;

#include <glad/glad.h>
#include "menu.h"

class Extras : public Menu
{
private:
	GUIText* timeText;
	GUIText* jebaited;
public:
	Extras();
	~Extras();
	Menu* step();
};

#endif
