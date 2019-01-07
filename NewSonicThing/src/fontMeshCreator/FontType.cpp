#include <list>

#include "fonttype.h"
#include "textmeshcreator.h"
#include "textmeshdata.h"

#include "../renderEngine/renderEngine.h"

#include "../engineTester/main.h"

FontType::FontType(int textureAtlas, std::string fontFilename)
{
	this->textureAtlas = textureAtlas;
	this->loader = new TextMeshCreator(fontFilename);
	INCR_NEW
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