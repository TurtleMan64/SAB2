#include "metafile.hpp"
#include "../toolbox/split.hpp"
#include "../toolbox/getline.hpp"
#include "textmeshcreator.hpp"

#include <iostream>
#include <string>
#include <cstring>

#include "../engineTester/main.hpp"
#include "../renderEngine/display.hpp"

MetaFile::MetaFile(std::string filename)
{
    aspectRatio = Display::ASPECT_RATIO;
    openFile(filename);
    loadPaddingData();
    loadLineSizes();
    int imageWidth = getValueOfVariable("scaleW");
    loadCharacterData(imageWidth);
    close();
}

float MetaFile::getSpaceWidth()
{
    return spaceWidth;
}

Character MetaFile::getCharacter(int ascii)
{
    return metaData[ascii];
}

bool MetaFile::processNextLine()
{
    std::string line;

    if (reader->eof())
    {
        return false;
    }
    values.clear();

    getlineSafe((*reader), line);
    //std::fprintf(stdout, "line = '%s'\n", line.c_str());

    char lineBuf[1024];
    memcpy(lineBuf, line.c_str(), line.size()+1);

    int splitLength1 = 0;
    char** lineSplit1 = split(lineBuf, SPLITTER, &splitLength1);

    for (int i = 0; i < splitLength1; i++)
    {
        char* part = lineSplit1[i];

        int splitLength2 = 0;
        char** lineSplit2 = split(part, '=', &splitLength2);

        if (splitLength2 == 2)
        {
            std::string vp0 = lineSplit2[0];
            std::string vp1 = lineSplit2[1];
            //std::fprintf(stdout, "'%s' '%s'\n", vp0.c_str(), vp1.c_str());
            values[vp0] = vp1;
        }
        free(lineSplit2);
    }
    free(lineSplit1);

    return true;
}

int MetaFile::getValueOfVariable(std::string variable)
{
    return std::stoi(values[variable]);
}

std::vector<int> MetaFile::getValuesOfVariable(std::string variable)
{
    std::string line = values[variable];

    char lineBuf[1024];
    memcpy(lineBuf, line.c_str(), line.size()+1);

    int splitLength = 0;
    char** numbers = split(lineBuf, NUMBER_SEPARATOR, &splitLength);

    std::vector<int> actualValues;

    for (int i = 0; i < splitLength; i++)
    {
        std::string val = numbers[i];
        actualValues.push_back(std::stoi(val));
    }
    free(numbers);

    return actualValues;
}

void MetaFile::close()
{
    reader->close();
    delete reader; INCR_DEL("ifstream");
    reader = nullptr;
}

void MetaFile::openFile(std::string filename)
{
    reader = new std::ifstream(Global::pathToEXE+filename); INCR_NEW("ifstream");
    if (!reader->is_open())
    {
        std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE+filename).c_str());
        reader->close();
        delete reader; INCR_DEL("ifstream");
        reader = nullptr;
    }
}

void MetaFile::loadPaddingData()
{
    processNextLine();
    this->padding = getValuesOfVariable("padding");
    this->paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
    this->paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];
}

void MetaFile::loadLineSizes()
{
    processNextLine();
    int lineHeightPixels = getValueOfVariable("lineHeight") - paddingHeight;
    verticalPerPixelSize = TextMeshCreator::LINE_HEIGHT / (float)lineHeightPixels;
    horizontalPerPixelSize = verticalPerPixelSize / aspectRatio;
}

void MetaFile::loadCharacterData(int imageWidth)
{
    processNextLine();
    processNextLine();
    while (processNextLine())
    {
        Character* c = loadCharacter(imageWidth);
        if (c != nullptr)
        {
            metaData[c->getId()] = (*c); //Put a copy of the character into the hash map
            delete c; INCR_DEL("Character");
        }
    }
}

Character* MetaFile::loadCharacter(int imageSize)
{
    int id = getValueOfVariable("id");
    if (id == TextMeshCreator::SPACE_ASCII)
    {
        this->spaceWidth = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
        return nullptr;
    }
    float xTex = ((float)getValueOfVariable("x") + (padding[PAD_LEFT] - DESIRED_PADDING)) / imageSize;
    float yTex = ((float)getValueOfVariable("y") + (padding[PAD_TOP] - DESIRED_PADDING)) / imageSize;
    int width = getValueOfVariable("width") - (paddingWidth - (2 * DESIRED_PADDING));
    int height = getValueOfVariable("height") - ((paddingHeight)-(2 * DESIRED_PADDING));
    float quadWidth = width * horizontalPerPixelSize;
    float quadHeight = height * verticalPerPixelSize;
    float xTexSize = (float)width / imageSize;
    float yTexSize = (float)height / imageSize;
    float xOff = (getValueOfVariable("xoffset") + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
    float yOff = (getValueOfVariable("yoffset") + (padding[PAD_TOP] - DESIRED_PADDING)) * verticalPerPixelSize;
    float xAdvance = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
    //std::fprintf(stdout, "%d %f %f %f %f %f %f %f %f %f\n", id, xTex, yTex, xTexSize, yTexSize, xOff, yOff, quadWidth, quadHeight, xAdvance);
    INCR_NEW("Character");
    return new Character(id, xTex, yTex, xTexSize, yTexSize, xOff, yOff, quadWidth, quadHeight, xAdvance);
}
