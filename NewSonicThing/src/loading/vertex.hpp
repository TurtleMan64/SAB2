#ifndef VERTEX_H
#define VERTEX_H

#include "../toolbox/vector.hpp"

class Vertex
{
public:
    const int NO_INDEX = -1;

    Vector3f position;
    Vector4f color;
    int textureIndex = NO_INDEX;
    int normalIndex = NO_INDEX;
    Vertex* duplicateVertex = nullptr;
    int index = 0;
    float length = 0.0f;

public:

    Vertex(int index, Vector3f* position, Vector4f* color);

    Vertex(int index, Vector3f* position);

    int getIndex();

    float getLength();

    int isSet();

    int hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther);

    void setTextureIndex(int textureIndex);

    void setNormalIndex(int normalIndex);

    Vector3f* getPosition();

    int getTextureIndex();

    int getNormalIndex();

    Vertex* getDuplicateVertex();

    void setDuplicateVertex(Vertex* duplicateVertex);

};
#endif
