#include <vector>

#include "textmeshdata.h"

TextMeshData::TextMeshData(std::vector<float>* vertexPositions, std::vector<float>* textureCoords)
{
	for (float f : (*vertexPositions))
	{
		this->vertexPositions.push_back(f);
	}

	for (float f : (*textureCoords))
	{
		this->textureCoords.push_back(f);
	}
}

std::vector<float>* TextMeshData::getVertexPositions()
{
	return &vertexPositions;
}

std::vector<float>* TextMeshData::getTextureCoords()
{
	return &textureCoords;
}

int TextMeshData::getVertexCount()
{
	return (int)(vertexPositions.size() / 2);
}