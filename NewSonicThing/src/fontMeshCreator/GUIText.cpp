#include "guitext.h"
#include "../renderEngine/renderEngine.h"
#include "../fontRendering/textmaster.h"

GUIText::GUIText(std::string text, float fontSize, FontType* font, float x, float y, float maxLineLength,
	bool centered, bool rightAligned, bool visible)
{
	this->colour.set(1, 1, 1);
	this->textString.assign(text);
	this->fontSize = fontSize;
	this->font = font;
	this->position.set(x, y);
	this->lineMaxSize = maxLineLength;
	this->centerText = centered;
	this->rightAlign = rightAligned;
	this->visible = visible;
	TextMaster::loadText(this);
}

void GUIText::deleteMe()
{
	TextMaster::removeText(this);
	for (int vboID : textMeshVbos)
	{
		Loader::deleteVBO(vboID);
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

int GUIText::getNumberOfLines()
{
	return numberOfLines;
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

void GUIText::setNumberOfLines(int number)
{
	numberOfLines = number;
}

bool GUIText::isCentered()
{
	return centerText;
}

bool GUIText::isRightAligned()
{
	return rightAlign;
}

float GUIText::getMaxLineSize()
{
	return lineMaxSize;
}

std::string* GUIText::getTextString()
{
	return &textString;
}
