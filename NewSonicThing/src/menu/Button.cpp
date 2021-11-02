#include <string>
#include "button.hpp"
#include "../engineTester/main.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guitexture.hpp"
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
    this->texture = GuiTexture(texture, posX, posY, scaleX, scaleY, 0);
    this->textureHighlight = GuiTexture(highlight, posX, posY, scaleX, scaleY, 0);
    this->textIsLeftAnchored = false;
    this->scaleX = scaleX;

    Button::setVisible(visible);
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

    texture = GuiTexture(textureId, posX, posY, scaleX, scaleY, 0);
    textureHighlight = GuiTexture(highlight, posX, posY, scaleX, scaleY, 0);

    Button::setVisible(visible);
}

Button::~Button()
{
    this->text->deleteMe(); delete this->text; INCR_DEL("GUIText");
}

void Button::generateText(std::string newText)
{
    float textScale = text->getFontSize();
    FontType* textFont = text->getFont();
    float textPosX = text->getPosition()->getX();
    float textPosY = text->getPosition()->getY();

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
        text->colour.set(0.5f, 0.5f, 0.5f);
    }
}

// Changes the button position. Should be preceded by GuiManager::clearGuisToRender.
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

    this->texture.setX(xPos);
    this->texture.setY(yPos);
    this->textureHighlight.setX(xPos);
    this->textureHighlight.setY(yPos);
}

void Button::setVisible(bool makeVisible)
{
    this->text->setVisibility(makeVisible);
    if (!makeVisible && this->visible)
    {
        GuiManager::removeGui(&this->texture);
        GuiManager::removeGui(&this->textureHighlight);

        this->visible = makeVisible;
        this->text->setVisibility(false);
        this->visibleHighlight = makeVisible;
    }
    else if (makeVisible)
    {
        GuiManager::addGuiToRender(&this->texture);
        this->text->setVisibility(true);
        this->visible = makeVisible;
    }
}

void Button::setHighlight(bool makeVisible)
{
    if (this->visibleHighlight && !makeVisible)
    {
        GuiManager::removeGui(&this->textureHighlight);
        this->visibleHighlight = false;
    }
    else if (!this->visibleHighlight && makeVisible)
    {
        GuiManager::addGuiToRender(&this->textureHighlight);
        this->visibleHighlight = true;
    }
}

GUIText* Button::getText()
{
    return this->text;
}
