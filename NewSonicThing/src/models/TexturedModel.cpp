#include <glad/glad.h>

#include <list>

#include "models.hpp"
#include "../textures/modeltexture.hpp"

TexturedModel::TexturedModel()
{

}

TexturedModel::TexturedModel(RawModel* model, ModelTexture* texture)
{
    //Copy over the RawModel data
    this->rawModel.setVaoId(model->getVaoId());
    this->rawModel.setVertexCount(model->getVertexCount());

    std::vector<GLuint>* myVBOs = this->rawModel.getVboIds();
    std::vector<GLuint>* theirVBOs = model->getVboIds();

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
