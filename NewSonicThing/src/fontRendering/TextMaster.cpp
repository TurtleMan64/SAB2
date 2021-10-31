#include <list>
#include <unordered_map>

#include "textmaster.h"
#include "../renderEngine/loader.h"
#include "../fontMeshCreator/textmeshdata.h"
#include "../fontMeshCreator/fonttype.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/guinumber.h"
#include "fontrenderer.h"
#include "../engineTester/main.h"

std::unordered_map<FontType*, std::list<GUIText*>> TextMaster::texts;
std::unordered_map<FontType*, std::list<GUINumber*>> TextMaster::numbers;
FontRenderer* TextMaster::renderer;

void TextMaster::init()
{
    GUINumber::loadMeshData();
    renderer = new FontRenderer; INCR_NEW("FontRenderer");
}

void TextMaster::render()
{
    renderer->render(&TextMaster::texts, &TextMaster::numbers);
}

void TextMaster::loadText(GUIText* text)
{
    FontType* font = text->getFont();
    TextMeshData* data = font->loadText(text);
    std::vector<int> vertexObjects = Loader::loadToVAO(data->getVertexPositions(), data->getTextureCoords());
    int vao = vertexObjects[0];
    std::vector<int> vbos;
    vbos.push_back(vertexObjects[1]);
    vbos.push_back(vertexObjects[2]);
    text->setMeshInfo(vao, &vbos, data->getVertexCount());

    delete data; INCR_DEL("TextMeshData");

    std::list<GUIText*>* textBatch = &texts[font];
    textBatch->push_back(text);
}

void TextMaster::removeText(GUIText* text)
{
    std::list<GUIText*>* textBatch = &texts[text->getFont()];
    textBatch->remove(text);
    if (textBatch->empty())
    {
        texts.erase(text->getFont());
    }
}

void TextMaster::loadNumber(GUINumber* number)
{
    FontType* font = number->getFont();

    std::list<GUINumber*>* numbersBatch = &TextMaster::numbers[font];
    numbersBatch->push_back(number);
}

void TextMaster::removeNumber(GUINumber* number)
{
    std::list<GUINumber*>* numberBatch = &TextMaster::numbers[number->getFont()];
    numberBatch->remove(number);
    if (numberBatch->empty())
    {
        TextMaster::numbers.erase(number->getFont());
    }
}

void TextMaster::cleanUp()
{
    renderer->cleanUp();
}
