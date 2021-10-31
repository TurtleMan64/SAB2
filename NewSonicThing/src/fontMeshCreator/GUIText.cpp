#include "guitext.h"
#include "../renderEngine/renderEngine.h"
#include "../fontRendering/textmaster.h"
#include "fonttype.h"

GUIText::GUIText(std::string text, float fontSize, FontType* font, float x, float y, float maxLineLength,
    bool centered, bool rightAligned, bool visible)
{
    this->colour.set(1, 1, 1);
    this->textString.assign(text);
    this->fontSize = fontSize;
    this->font = font;
    this->position.set(x, y);
    alignment = 0;
    this->lineMaxSizeConstraint = maxLineLength;
    if (centered)
    {
        alignment = 1;
        position.x += maxLineLength/2;
    }
    else if (rightAligned)
    {
        alignment = 2;
        position.x += maxLineLength;
    }
    this->visible = visible;
    TextMaster::loadText(this);
}

GUIText::GUIText(std::string text, float fontSize, FontType* font, float x, float y, int alignment, bool visible)
{
    this->colour.set(1, 1, 1);
    this->textString.assign(text);
    this->fontSize = fontSize;
    this->font = font;
    this->position.set(x, y);
    this->alignment = alignment;
    this->visible = visible;
    this->lineMaxSizeConstraint = 1000000.0f;
    TextMaster::loadText(this);
}

void GUIText::deleteMe()
{
    TextMaster::removeText(this);
    for (int vboId : textMeshVbos)
    {
        Loader::deleteVBO(vboId);
    }
    Loader::deleteVAO(textMeshVao);
}

FontType* GUIText::getFont()
{
    return font;
}

void GUIText::setColour(float r, float g, float b)
{
    colour.set(r, g, b);
}

Vector3f* GUIText::getColour()
{
    return &colour;
}

float GUIText::getMaxLineSizeConstraint()
{
    return lineMaxSizeConstraint;
}

Vector2f* GUIText::getPosition()
{
    return &position;
}

void GUIText::setPosition(Vector2f* newPos)
{
    position.set(newPos);
}

void GUIText::setVisibility(bool newVisible)
{
    visible = newVisible;
}

bool GUIText::isVisible()
{
    return visible;
}

int GUIText::getMesh()
{
    return textMeshVao;
}

void GUIText::setMeshInfo(int vao, std::vector<int>* vbos, int verticesCount)
{
    this->textMeshVao = vao;
    for (int vbo : (*vbos))
    {
        this->textMeshVbos.push_back(vbo);
    }
    this->vertexCount = verticesCount;
}

int GUIText::getVertexCount()
{
    return vertexCount;
}

float GUIText::getFontSize()
{
    return fontSize;
}

int GUIText::getAlignment()
{
    return alignment;
}

std::string* GUIText::getTextString()
{
    return &textString;
}
