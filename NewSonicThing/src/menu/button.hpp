#ifndef BUTTON_H
#define BUTTON_H

class GUIText;
class FontType;

#include <string>
#include "../guis/guitexture.hpp"
#include <glad/glad.h>

class Button
{
private:
    bool visible;
    bool visibleHighlight;

    GUIText* text = nullptr;
    bool textIsLeftAnchored;
    float scaleX;
    float anchorOffset = 0;

    GuiTexture texture;
    GuiTexture textureHighlight; // For Arcade

public:
    /*
       Basic button with normal/highlight texture and text
     * label: text to display on button
     * posX, posY: coordinates to center the button, where
      (0,0) is top-left, and (1,1) is bottom-right
     * scaleX, scaleY: scale for the button, where
       1.0f is the whole screen
     * visible: draw the button after creating it
    */
    Button(std::string label, FontType* font, GLuint texture, GLuint highlight, float posX, float posY, float scaleX, float scaleY, bool visible);

    //If you want text to be anchored on the left side
    Button(std::string label, FontType* font, GLuint texture, GLuint highlight, float posX, float posY, float scaleX, float scaleY, bool visible, bool leftAnchored);

    ~Button();

    void setVisible(bool makeVisible);
    void setHighlight(bool makeVisible); // Toggles textureSet display

    void setPos(float xPos, float yPos);

    void generateText(std::string newText);
    void generateText(std::string newText, bool darkText);

    GUIText* getText();
};
#endif
