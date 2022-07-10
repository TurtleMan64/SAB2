#include <glad/glad.h>

#include <vector>

#include "models.hpp"
#include "../loading/loadergl.hpp"

RawModel::RawModel()
{

}

RawModel::RawModel(GLuint vaoId, int vertexCount, std::vector<GLuint>* vboIds)
{
    this->vaoId = vaoId;
    this->vertexCount = vertexCount;

    for (GLuint id : (*vboIds))
    {
        this->vboIds.push_back(id);
    }
}

GLuint RawModel::getVaoId()
{
    return vaoId;
}

void RawModel::setVaoId(GLuint newId)
{
    this->vaoId = newId;
}

int RawModel::getVertexCount()
{
    return vertexCount;
}

void RawModel::setVertexCount(int newCount)
{
    this->vertexCount = newCount;
}

std::vector<GLuint>* RawModel::getVboIds()
{
    return &vboIds;
}

void RawModel::deleteMe()
{
    LoaderGL::deleteVAO(vaoId);
    for (auto vbo : vboIds)
    {
        LoaderGL::deleteVBO(vbo);
    }
    vboIds.clear();
}
