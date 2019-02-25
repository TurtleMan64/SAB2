#ifndef MAINMENU_H
#define MAINMENU_H

class GUIText;
class Button;

#include <glad/glad.h>
#include <vector>
#include "../guis/guitexture.h"
#include "menu.h"

class MainMenu : public Menu
{
private:
	enum ButtonID { mission, config, exit };

	int menuSelectionID;	// Current menu selection highlighted

	// Title card text graphics
	GLuint textureParallelogram; // Texture for button
	GLuint textureParallelogramBackdrop; // Texture for button highlight
	GLuint textureLogo; // Texture for logo

	GuiTexture logo;

	Button** menuButtons;

	int buttonCount;

	float aspectRatio;

	void unloadResources();

	void setVisible(bool visibleStatus);

	bool visible;

	// Animation
	int oldSelection;
	float animationTime;
	bool animating;
	int animationDirection;
	void resetAnimation();

public:
	MainMenu();

	~MainMenu();

	void init();

	Menu* step();

	void loadResources();

	void draw();
};

#endif
