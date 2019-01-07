#include <glad/glad.h>

#include <list>

#include "models.h"
#include "../renderEngine/renderEngine.h"

RawModel::RawModel()
{

}

RawModel::RawModel(GLuint vaoID, int vertexCount, std::list<GLuint>* vboIDs)
{
	this->vaoID = vaoID;
	this->vertexCount = vertexCount;

	for (auto id : (*vboIDs))
	{
		this->vboIDs.push_back(id);
	}
}

GLuint RawModel::getVaoID()
{
	return vaoID;
}

void RawModel::setVaoID(GLuint newID)
{
	this->vaoID = newID;
}

int RawModel::getVertexCount()
{
	return vertexCount;
}

void RawModel::setVertexCount(int newCount)
{
	this->vertexCount = newCount;
}

std::list<GLuint>* RawModel::getVboIDs()
{
	return &vboIDs;
}

void RawModel::deleteMe()
{
	Loader::deleteVAO(vaoID);
	for (auto vbo : vboIDs)
	{
		Loader::deleteVBO(vbo);
	}
	vboIDs.clear();
}