#include <string>
#include "button.hpp"
#include "../engineTester/main.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guiimage.hpp"
#include "../renderEngine/display.hpp"

/*
   Basic button with normal/highlight texture and text
 * label: text to display on button
 * posX, posY: coordinates to center the button, where
  (0,0) is top-left, and (1,1) is bottom-right
 * scaleX, scaleY: scale for the button, where
   1.0f is the whole screen
 * visible: draw the button after creating it
*/
Button::Button(std::string label, FontType* font, GLuint texture, GLuint highlight, float posX, float posY, float scaleX, float scaleY, bool visible)
{
    this->text = new GUIText(label, scaleY, font, posX, posY, 4, true); INCR_NEW("GUIText");
    this->texture = new GuiImage(texture, posX, posY, scaleX, scaleY, 0); INCR_NEW("GuiImage");
    this->textureHighlight = new GuiImage(highlight, posX, posY, scaleX, scaleY, 0); INCR_NEW("GuiImage");
    this->textIsLeftAnchored = false;
    this->scaleX = scaleX;

    setVisible(visible);
}

Button::Button(std::string label, FontType* font, GLuint textureId, GLuint highlight, float posX, float posY, float scaleX, float scaleY, bool visible, bool leftAnchored)
{
    float aspectRatio = Display::ASPECT_RATIO;
    anchorOffset = 0.02f*aspectRatio;

    if (!leftAnchored)
    {
        text = new GUIText(label, scaleY, font, posX, posY, 4, true); INCR_NEW("GUIText");
    }
    else
    {
        text = new GUIText(label, scaleY, font, posX - scaleX/2 + anchorOffset, posY, 3, true); INCR_NEW("GUIText");
    }

    this->scaleX = scaleX;
    textIsLeftAnchored = leftAnchored;

    texture = new GuiImage(textureId, posX, posY, scaleX, scaleY, 0); INCR_NEW("GuiImage");
    textureHighlight = new GuiImage(highlight, posX, posY, scaleX, scaleY, 0); INCR_NEW("GuiImage");

    setVisible(visible);
}

Button::~Button()
{
    this->text->deleteMe(); delete this->text; INCR_DEL("GUIText");
    delete texture; INCR_DEL("GuiImage");
    delete textureHighlight; INCR_DEL("GuiImage");
}

void Button::generateText(std::string newText)
{
    float textScale = text->getFontSize();
    FontType* textFont = text->getFont();
    float textPosX = text->position.x;
    float textPosY = text->position.y;

    text->deleteMe(); delete text; INCR_DEL("GUIText");
    text = nullptr;
    if (textIsLeftAnchored)
    {
        text = new GUIText(newText, textScale, textFont, textPosX, textPosY, 3, true); INCR_NEW("GUIText");
    }
    else
    {
        text = new GUIText(newText, textScale, textFont, textPosX, textPosY, 4, true); INCR_NEW("GUIText");
    }
}

void Button::generateText(std::string newText, bool darkText)
{
    generateText(newText);
    if (darkText)
    {
        text->color.set(0.5f, 0.5f, 0.5f);
    }
}

void Button::setPos(float xPos, float yPos)
{
    if (textIsLeftAnchored)
    {
        text->getPosition()->x = xPos - scaleX/2 + anchorOffset;
        text->getPosition()->y = yPos;
    }
    else
    {
        text->getPosition()->x = xPos;
        text->getPosition()->y = yPos;
    }

    texture->setX(xPos);
    texture->setY(yPos);
    textureHighlight->setX(xPos);
    textureHighlight->setY(yPos);
}

void Button::setVisible(bool isVisible)
{
    visible = isVisible;
    text->visible = isVisible;
    if (!isVisible)
    {
        GuiManager::removeImageToRender(texture);
        visibleHighlight = false;
    }
    else
    {
        GuiManager::addImageToRender(texture);
    }
}

void Button::setHighlight(bool isHighlight)
{
    if (isHighlight)
    {
        visibleHighlight = true;
        texture->alpha = 1.0f;
    }
    else
    {
        visibleHighlight = false;
        texture->alpha = 0.35f;
    }
}

GUIText* Button::getText()
{
    return text;
}
