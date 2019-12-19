#ifndef TEXTMESHCREATOR_H
#define TEXTMESHCREATOR_H

class MetaFile;
class GUIText;

#include <string>
#include <vector>

#include "line.h"
#include "textmeshdata.h"


class TextMeshCreator
{
private:
    MetaFile* metaData;

    std::vector<Line> createStructure(GUIText* text);

    //void completeStructure(std::vector<Line> lines, Line currentLine, Word currentWord, GUIText* text);

    TextMeshData* createQuadVertices(GUIText* text, std::vector<Line>* lines);

    void addVerticesForCharacter(float curserX, float curserY, Character character, float fontSize, std::vector<float>* vertices);

    static void addVertices(std::vector<float>* vertices, float x, float y, float maxX, float maxY);

    static void addTexCoords(std::vector<float>* texCoords, float x, float y, float maxX, float maxY);

    //static float[] listToArray(std::vector<Float> listOfFloats);

public:
    static float LINE_HEIGHT;
    static const int SPACE_ASCII = 32;

    TextMeshCreator(std::string filename);

    TextMeshData* createTextMesh(GUIText* text);
    
};

#endif
