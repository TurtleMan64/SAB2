#ifndef BUTTON_H
#define BUTTON_H

class GUIText;
class FontType;

#include <string>
#include "../guis/guitexture.h"
#include <glad/glad.h>

class Button
{
private:
	bool visible;
	bool visibleHighlight;

	GUIText* text = nullptr;

	GuiTexture texture;
	GuiTexture textureHighlight; // For Arcade

public:
	Button(std::string label, FontType* font, GLuint texture, GLuint highlight, float posX, float posY, float scaleX, float scaleY, bool visible);
	~Button();

	void setVisible(bool makeVisible);
	void setHighlight(bool makeVisible); // Toggles textureSet display

	void setPos(float xPos, float yPos);

    void generateText(std::string newText);

	GUIText* getText();
};
#endif
