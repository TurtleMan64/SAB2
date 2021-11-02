#include <list>

#include "fonttype.hpp"
#include "textmeshcreator.hpp"
#include "textmeshdata.hpp"

#include "../loading/loader.hpp"

#include "../engineTester/main.hpp"

FontType::FontType(int textureAtlas, std::string fontFilename)
{
    this->textureAtlas = textureAtlas;
    this->loader = new TextMeshCreator(fontFilename); INCR_NEW("TextMeshCreator");
}

int FontType::getTextureAtlas()
{
    return textureAtlas;
}

void FontType::deleteMe()
{
    Loader::deleteTexture(textureAtlas);
    textureAtlas = -1;
}

TextMeshData* FontType::loadText(GUIText* text)
{
    return loader->createTextMesh(text);
}
