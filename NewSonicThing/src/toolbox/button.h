#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "../fontMeshCreator/fonttype.h"
#include "../fontMeshCreator/guitext.h"
#include "../guis/guitexture.h"
#include <glad/glad.h>

class Button
{
private:
	bool visible;
	bool visibleHighlight;

	GUIText* text;

	GuiTexture texture;
	GuiTexture textureHighlight; // For Arcade
public:
	Button(std::string, FontType*, GLuint, GLuint, float, float, float, float, bool);
	~Button();

	void setVisible(bool);
	void setHighlight(bool); // Toggles textureSet display

	void setPos(float, float);

	GUIText* getText();
};
#endif
