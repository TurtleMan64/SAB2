#ifndef MISSIONMENU_H
#define MISSIONMENU_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include "../guis/guitexture.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/fonttype.h"
#include "button.h"
#include "menu.h"

class MissionMenu : public Menu
{
private:
	int index;
	GLuint textureParallelogram;
	GLuint textureParallelogramBackdrop;
	GLuint textureParallelogramHalf;
	GLuint textureParallelogramHalfBackdrop;
	GLuint textureParallelogramHalf2;
	GLuint textureParallelogramHalf2Backdrop;

	int moveXPrevious;
	int moveYPrevious;
	bool selectInputPrevious;
	bool backInputPrevious;

	float holdUpTimer;
	float holdDownTimer;

	Button** levelButton;
	int counter;

	bool visible;

	// Animation
	int oldSelection;
	float animationTime;
	bool animating;
	int animationDirection;

private:
	FontType* font;

	void draw();
	void setVisible(bool);

public:
	MissionMenu();
	~MissionMenu();
	void init();
	void loadResources();
	void unloadResources();
	Menu* step();
	
	float fontSize;
};

#endif