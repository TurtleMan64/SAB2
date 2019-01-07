#ifndef TEXTMESHDATA_H
#define TEXTMESHDATA_H

#include <vector>

/**
* Stores the vertex data for all the quads on which a text will be rendered.
* @author Karl
*
*/
class TextMeshData
{
private:
	std::vector<float> vertexPositions;
	std::vector<float> textureCoords;

public:
	TextMeshData(std::vector<float>* vertexPositions, std::vector<float>* textureCoords);

	std::vector<float>* getVertexPositions();

	std::vector<float>* getTextureCoords();

	int getVertexCount();
};

#endif