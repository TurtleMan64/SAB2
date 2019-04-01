#include <glad/glad.h>

#include <list>

#include "models.h"
#include "../textures/modeltexture.h"

TexturedModel::TexturedModel()
{

}

TexturedModel::TexturedModel(RawModel* model, ModelTexture* texture)
{
	//Copy over the RawModel data
	this->rawModel.setVaoID(model->getVaoID());
	this->rawModel.setVertexCount(model->getVertexCount());

	std::list<GLuint>* myVBOs = this->rawModel.getVboIDs();
	std::list<GLuint>* theirVBOs = model->getVboIDs();

	for (GLuint vbo : (*theirVBOs))
	{
		myVBOs->push_back(vbo);
	}

	//Copy over the ModelTexture data
    this->texture = ModelTexture(texture);
    this->texture.addMeToAnimationsSetIfNeeded();
}

RawModel* TexturedModel::getRawModel()
{
	return &rawModel;
}

ModelTexture* TexturedModel::getTexture()
{
	return &texture;
}

void TexturedModel::deleteMe()
{
	rawModel.deleteMe();
	texture.deleteMe();
}
