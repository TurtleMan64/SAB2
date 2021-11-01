#include <glad/glad.h>

#include "../toolbox/vector.h"
#include "vertex.h"
#include "../toolbox/maths.h"

#include <iostream>


Vertex::Vertex(int index, Vector3f* position, Vector4f* color)
{
    this->index = index;
    this->color.set(color);
    this->position.set(position->x, position->y, position->z);
    this->length = position->length();
    this->textureIndex = NO_INDEX;
    this->normalIndex = NO_INDEX;
    this->duplicateVertex = nullptr;
}

Vertex::Vertex(int index, Vector3f* position)
{
    this->index = index;
    //this->color.set(Maths::nextUniform(), Maths::nextUniform(), Maths::nextUniform());
    this->color.set(1.0f, 1.0f, 1.0f, 1.0f);
    this->position.set(position->x, position->y, position->z);
    this->length = position->length();
    this->textureIndex = NO_INDEX;
    this->normalIndex = NO_INDEX;
    this->duplicateVertex = nullptr;
}

int Vertex::getIndex()
{
    return index;
}

float Vertex::getLength()
{
    return length;
}

int Vertex::isSet()
{
    if ((textureIndex != NO_INDEX) && (normalIndex != NO_INDEX))
    {
        return 1;
    }
    return 0;
}

int Vertex::hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther)
{
    if ((textureIndexOther == textureIndex) && (normalIndexOther == normalIndex))
    {
        return 1;
    }
    return 0;
}

void Vertex::setTextureIndex(int newTextureIndex)
{
    textureIndex = newTextureIndex;
}

void Vertex::setNormalIndex(int newNormalIndex)
{
    normalIndex = newNormalIndex;
}

Vector3f* Vertex::getPosition()
{
    return &position;
}

int Vertex::getTextureIndex()
{
    return textureIndex;
}

int Vertex::getNormalIndex()
{
    return normalIndex;
}

Vertex* Vertex::getDuplicateVertex()
{
    return duplicateVertex;
}

void Vertex::setDuplicateVertex(Vertex* duplicateVertexNew)
{
    duplicateVertex = duplicateVertexNew;
}
