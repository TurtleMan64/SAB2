#include "textmeshcreator.h"
#include "metafile.h"
#include "textmeshdata.h"
#include "guitext.h"

#include "../engineTester/main.h"


double TextMeshCreator::LINE_HEIGHT = 1;


TextMeshCreator::TextMeshCreator(std::string metaFilename)
{
	metaData = new MetaFile(metaFilename);
	INCR_NEW
}

TextMeshData* TextMeshCreator::createTextMesh(GUIText* text)
{
	std::vector<Line> lines = createStructure(text);
	TextMeshData* data = createQuadVertices(text, &lines);
	return data;
}

std::vector<Line> TextMeshCreator::createStructure(GUIText* text)
{
	const char* chars = text->getTextString()->c_str();
	int charsLength = (int)(text->getTextString()->size());
	std::vector<Line> lines;
	Line currentLine(metaData->getSpaceWidth(), text->getFontSize(), text->getMaxLineSize());
	Word currentWord(text->getFontSize());
	for (int i = 0; i < charsLength; i++)
	{
		int ascii = (int)chars[i];
		if (ascii == SPACE_ASCII)
		{
			bool added = currentLine.attemptToAddWord(&currentWord);
			if (!added)
			{
				lines.push_back(currentLine);
				currentLine = Line(metaData->getSpaceWidth(), text->getFontSize(), text->getMaxLineSize());
				currentLine.attemptToAddWord(&currentWord);
			}
			currentWord = Word(text->getFontSize());
			continue;
		}
		Character character = metaData->getCharacter(ascii);
		currentWord.addCharacter(&character);
	}
	//completeStructure(lines, currentLine, currentWord, text);

	bool added = currentLine.attemptToAddWord(&currentWord);
	if (!added)
	{
		lines.push_back(currentLine);
		currentLine = Line(metaData->getSpaceWidth(), text->getFontSize(), text->getMaxLineSize());
		currentLine.attemptToAddWord(&currentWord);
	}
	lines.push_back(currentLine);


	return lines;
}
/*
void TextMeshCreator::completeStructure(std::vector<Line> lines, Line currentLine, Word currentWord, GUIText* text)
{
	bool added = currentLine.attemptToAddWord(currentWord);
	if (!added)
	{
		lines.add(currentLine);
		currentLine = Line(metaData->getSpaceWidth(), text.getFontSize(), text.getMaxLineSize());
		currentLine.attemptToAddWord(currentWord);
	}
	lines.add(currentLine);
}
*/

TextMeshData* TextMeshCreator::createQuadVertices(GUIText* text, std::vector<Line>* lines)
{
	text->setNumberOfLines((int)(lines->size()));
	double curserX = 0.0;
	double curserY = 0.0;
	std::vector<float> vertices;
	std::vector<float> textureCoords;
	for (Line line : (*lines))
	{
		if (text->isCentered())
		{
			curserX = (line.getMaxLength() - line.getLineLength()) / 2;
		}
		else if (text->isRightAligned())
		{
			curserX = (line.getMaxLength() - line.getLineLength());
		}
		for (Word word : (*line.getWords()))
		{
			for (Character letter : (*word.getCharacters()))
			{
				addVerticesForCharacter(curserX, curserY, letter, text->getFontSize(), &vertices);
				addTexCoords(&textureCoords, letter.getxTextureCoord(), letter.getyTextureCoord(),
					letter.getXMaxTextureCoord(), letter.getYMaxTextureCoord());
				curserX += letter.getxAdvance() * text->getFontSize();
			}
			curserX += metaData->getSpaceWidth() * text->getFontSize();
		}
		curserX = 0;
		curserY += LINE_HEIGHT * text->getFontSize();
	}
	INCR_NEW
	return new TextMeshData(&vertices, &textureCoords);
}

void TextMeshCreator::addVerticesForCharacter(double curserX, double curserY, Character character, double fontSize,
	std::vector<float>* vertices)
{
	double x = curserX + (character.getxOffset() * fontSize);
	double y = curserY + (character.getyOffset() * fontSize);
	double maxX = x + (character.getSizeX() * fontSize);
	double maxY = y + (character.getSizeY() * fontSize);
	double properX = (2 * x) - 1;
	double properY = (-2 * y) + 1;
	double properMaxX = (2 * maxX) - 1;
	double properMaxY = (-2 * maxY) + 1;
	addVertices(vertices, properX, properY, properMaxX, properMaxY);
}

void TextMeshCreator::addVertices(std::vector<float>* vertices, double x, double y, double maxX, double maxY)
{
	vertices->push_back((float)x);
	vertices->push_back((float)y);
	vertices->push_back((float)x);
	vertices->push_back((float)maxY);
	vertices->push_back((float)maxX);
	vertices->push_back((float)maxY);
	vertices->push_back((float)maxX);
	vertices->push_back((float)maxY);
	vertices->push_back((float)maxX);
	vertices->push_back((float)y);
	vertices->push_back((float)x);
	vertices->push_back((float)y);
}

void TextMeshCreator::addTexCoords(std::vector<float>* texCoords, double x, double y, double maxX, double maxY)
{
	texCoords->push_back((float)x);
	texCoords->push_back((float)y);
	texCoords->push_back((float)x);
	texCoords->push_back((float)maxY);
	texCoords->push_back((float)maxX);
	texCoords->push_back((float)maxY);
	texCoords->push_back((float)maxX);
	texCoords->push_back((float)maxY);
	texCoords->push_back((float)maxX);
	texCoords->push_back((float)y);
	texCoords->push_back((float)x);
	texCoords->push_back((float)y);
}