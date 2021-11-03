#include <list>

#include "fonttype.hpp"
#include "textmeshcreator.hpp"
#include "textmeshdata.hpp"

#include "../loading/loadergl.hpp"

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
    LoaderGL::deleteTexture(textureAtlas);
    textureAtlas = -1;
}

TextMeshData* FontType::loadText(GUIText* text)
{
    return loader->createTextMesh(text);
}
