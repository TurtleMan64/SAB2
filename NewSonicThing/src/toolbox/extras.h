#ifndef EXTRAS_H
#define EXTRAS_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/guinumber.h"
#include "menu.h"

class Extras : public Menu
{
private:
	GUIText* timeText;
	GUIText* jebaited;
public:
	Extras();
	~Extras();
	int step();
};

#endif