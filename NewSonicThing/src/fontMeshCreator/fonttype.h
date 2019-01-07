#ifndef FONTTYPE_H
#define FONTTYPE_H

class GUIText;
class TextMeshCreator;
class TextMeshData;

#include <string>


/**
* Represents a font. It holds the font's texture atlas as well as having the
* ability to create the quad vertices for any text using this font.
*
* @author Karl
*
*/
class FontType
{
private:
	int textureAtlas;
	TextMeshCreator* loader;

public:
	/**
	* Creates a new font and loads up the data about each character from the
	* font file.
	*
	* @param textureAtlas
	*            - the ID of the font atlas texture.
	* @param fontFile
	*            - the font file containing information about each character in
	*            the texture atlas.
	*/
	FontType(int textureAtlas, std::string fontFilename);

	/**
	* @return The font texture atlas.
	*/
	int getTextureAtlas();

	void deleteMe();

	/**
	* Takes in an unloaded text and calculate all of the vertices for the quads
	* on which this text will be rendered. The vertex positions and texture
	* coords and calculated based on the information from the font file.
	*
	* @param text
	*            - the unloaded text.
	* @return Information about the vertices of all the quads.
	*/
	TextMeshData* loadText(GUIText* text);
};

#endif