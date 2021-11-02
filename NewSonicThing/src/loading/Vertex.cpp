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
