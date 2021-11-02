#include "textmeshcreator.hpp"
#include "metafile.hpp"
#include "textmeshdata.hpp"
#include "guitext.hpp"

#include "../engineTester/main.hpp"


float TextMeshCreator::LINE_HEIGHT = 1.0f;
#define FONT_SIZE text->getFontSize()

TextMeshCreator::TextMeshCreator(std::string metaFilename)
{
    metaData = new MetaFile(metaFilename); INCR_NEW("MetaFile");
}

TextMeshData* TextMeshCreator::createTextMesh(GUIText* text)
{
    std::vector<Line> lines = createStructure(text);
    text->lineWidths.clear();
    text->maxLineWidth = 0.0f;
    for (int i = 0; i < (int)lines.size(); i++)
    {
        float newLength = lines[i].getLineLength();
        text->lineWidths.push_back(lines[i].getLineLength());
        if (newLength > text->maxLineWidth)
        {
            text->maxLineWidth = newLength;
        }
    }
    TextMeshData* data = createQuadVertices(text, &lines);
    return data;
}

std::vector<Line> TextMeshCreator::createStructure(GUIText* text)
{
    const char* chars = text->getTextString()->c_str();
    int charsLength = (int)(text->getTextString()->size());
    std::vector<Line> lines;
    Line currentLine(metaData->getSpaceWidth(), FONT_SIZE, text->getMaxLineSizeConstraint());
    Word currentWord(FONT_SIZE);
    for (int i = 0; i < charsLength; i++)
    {
        int ascii = (int)chars[i];
        if (ascii == SPACE_ASCII)
        {
            bool added = currentLine.attemptToAddWord(&currentWord);
            if (!added)
            {
                lines.push_back(currentLine);
                currentLine = Line(metaData->getSpaceWidth(), FONT_SIZE, text->getMaxLineSizeConstraint());
                currentLine.attemptToAddWord(&currentWord);
            }
            currentWord = Word(FONT_SIZE);
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
        currentLine = Line(metaData->getSpaceWidth(), FONT_SIZE, text->getMaxLineSizeConstraint());
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
    float curserX = 0.0f;
    float curserY = 0.0f;
    std::vector<float> vertices;
    std::vector<float> textureCoords;

    if (text->getAlignment() == 3 ||
        text->getAlignment() == 4 ||
        text->getAlignment() == 5)
    {
        curserY = -((LINE_HEIGHT * FONT_SIZE)/2);
    }
    else if (text->getAlignment() == 6 ||
             text->getAlignment() == 7 ||
             text->getAlignment() == 8)
    {
        curserY = -(LINE_HEIGHT * FONT_SIZE);
    }

    for (Line line : (*lines))
    {
        if (text->getAlignment() == 1 ||
            text->getAlignment() == 4 ||
            text->getAlignment() == 7)
        {
            curserX = -(line.getLineLength()/2);
        }
        else if (text->getAlignment() == 2 ||
                 text->getAlignment() == 5 ||
                 text->getAlignment() == 8)
        {
            curserX = -line.getLineLength();
        }

        for (Word word : (*line.getWords()))
        {
            for (Character letter : (*word.getCharacters()))
            {
                addVerticesForCharacter(curserX, curserY, letter, FONT_SIZE, &vertices);
                addTexCoords(&textureCoords, letter.getxTextureCoord(), letter.getyTextureCoord(),
                    letter.getXMaxTextureCoord(), letter.getYMaxTextureCoord());
                curserX += letter.getxAdvance() * FONT_SIZE;
            }
            curserX += metaData->getSpaceWidth() * FONT_SIZE;
        }

        curserX = 0;
        curserY += LINE_HEIGHT * FONT_SIZE;
    }

    INCR_NEW("TextMeshData");
    return new TextMeshData(&vertices, &textureCoords);
}

void TextMeshCreator::addVerticesForCharacter(float curserX, float curserY, Character character, float fontSize,
    std::vector<float>* vertices)
{
    float x = curserX + (character.getxOffset() * fontSize);
    float y = curserY + (character.getyOffset() * fontSize);
    float maxX = x + (character.getSizeX() * fontSize);
    float maxY = y + (character.getSizeY() * fontSize);
    float properX = (2 * x) - 1;
    float properY = (-2 * y) + 1;
    float properMaxX = (2 * maxX) - 1;
    float properMaxY = (-2 * maxY) + 1;
    addVertices(vertices, properX, properY, properMaxX, properMaxY);
}

void TextMeshCreator::addVertices(std::vector<float>* vertices, float x, float y, float maxX, float maxY)
{
    vertices->push_back(x);
    vertices->push_back(y);
    vertices->push_back(x);
    vertices->push_back(maxY);
    vertices->push_back(maxX);
    vertices->push_back(maxY);
    vertices->push_back(maxX);
    vertices->push_back(maxY);
    vertices->push_back(maxX);
    vertices->push_back(y);
    vertices->push_back(x);
    vertices->push_back(y);
}

void TextMeshCreator::addTexCoords(std::vector<float>* texCoords, float x, float y, float maxX, float maxY)
{
    texCoords->push_back(x);
    texCoords->push_back(y);
    texCoords->push_back(x);
    texCoords->push_back(maxY);
    texCoords->push_back(maxX);
    texCoords->push_back(maxY);
    texCoords->push_back(maxX);
    texCoords->push_back(maxY);
    texCoords->push_back(maxX);
    texCoords->push_back(y);
    texCoords->push_back(x);
    texCoords->push_back(y);
}
