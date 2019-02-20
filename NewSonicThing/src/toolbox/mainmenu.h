#ifndef MAINMENU_H
#define MAINMENU_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include "button.h"
#include "menu.h"

class FontType;
class GUIText;
class GuiTexture;

class MainMenu : public Menu
{
private:
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

	void setVisible(bool);

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

	FontType* font;

	void init();

	Menu* step();

	void loadResources();

	void draw();
};

#endif