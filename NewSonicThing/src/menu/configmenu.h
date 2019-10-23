#ifndef CONFIGMENU_H
#define CONFIGMENU_H

class Button;

#include <glad/glad.h>
#include <vector>
#include <string>
#include "menu.h"

class ConfigMenu : public Menu
{
private:
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

	float holdDownTimer;
	float holdUpTimer;
    float holdLeftTimer;
	float holdRightTimer;

    int currentButtonIndex = 0;
    std::vector<Button*> buttonsNames;
    std::vector<Button*> buttonsValues;

	bool visible;

	// Animation
	float offsetTarget;
	float offsetCurr;

private:
	void draw();
	void setVisible(bool visibleStatus);

    static std::string boolToString(bool value);
    static std::string floatToString(float value);

public:
	ConfigMenu();
	~ConfigMenu();
	void init();
	void loadResources();
	void unloadResources();
	Menu* step();
	
	float fontSize;
};

#endif
