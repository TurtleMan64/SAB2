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
    this->rawModel.setVaoId(model->getVaoId());
    this->rawModel.setVertexCount(model->getVertexCount());

    std::list<GLuint>* myVBOs = this->rawModel.getVboIds();
    std::list<GLuint>* theirVBOs = model->getVboIds();

    for (GLuint vbo : (*theirVBOs))
    {
        myVBOs->push_back(vbo);
    }

    //Copy over the ModelTexture data
    this->texture = ModelTexture(texture);
    this->texture.addMeToAnimationsSetIfNeeded();

    renderOrder = texture->renderOrder;
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
