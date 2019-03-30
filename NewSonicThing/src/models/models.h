#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>

#include <list>

#include "../textures/modeltexture.h"

class RawModel
{
private:
	GLuint vaoID;
	int vertexCount;
	std::list<GLuint> vboIDs;

public:
	RawModel();

	RawModel(GLuint vaoID, int vertexCount, std::list<GLuint>* vboIDs);

	GLuint getVaoID();
	void setVaoID(GLuint newID);

	void setVertexCount(int newCount);
	int getVertexCount();

	void deleteMe();

	//for use in textured model constructor only
	std::list<GLuint>* getVboIDs();
};

class TexturedModel
{
private:
	RawModel rawModel;
	ModelTexture texture;

public:
	TexturedModel(RawModel*, ModelTexture*);

	TexturedModel();

	RawModel* getRawModel();

	ModelTexture* getTexture();

	void deleteMe();
};
#endif
