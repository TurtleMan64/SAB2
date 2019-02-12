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

	float aspectRatio; // Current aspect ratio from GuiManager

	Button** menuButtons;

	int buttonCount;

	void unloadResources();

	void buttonClick();

public:
	MainMenu();

	~MainMenu();

	FontType* font;

	void init();

	int step();

	void loadResources();

	void selectMenuRoot(int newSelection);
};

#endif