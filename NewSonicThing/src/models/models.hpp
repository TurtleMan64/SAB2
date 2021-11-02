#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>

#include <list>

#include "../textures/modeltexture.hpp"

class RawModel
{
private:
    GLuint vaoId;
    int vertexCount;
    std::list<GLuint> vboIds;

public:
    RawModel();

    RawModel(GLuint vaoId, int vertexCount, std::list<GLuint>* vboIds);

    GLuint getVaoId();
    void setVaoId(GLuint newId);

    void setVertexCount(int newCount);
    int getVertexCount();

    void deleteMe();

    //for use in textured model constructor only
    std::list<GLuint>* getVboIds();
};

class TexturedModel
{
private:
    RawModel rawModel;
    ModelTexture texture;

public:
    char renderOrder = 0; //0 = rendered first (default), 1 = second, 2 = third, 3 = fifth + transparent (only render top), 4 = fourth + tmp disable depth test , 5 = sixth

    TexturedModel(RawModel* rawModel, ModelTexture* texture);

    TexturedModel();

    RawModel* getRawModel();

    ModelTexture* getTexture();

    void deleteMe();
};
#endif
