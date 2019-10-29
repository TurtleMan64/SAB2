#include <glad/glad.h>
#include <unordered_map>
#include <list>

#include "shadowshader.h"
#include "shadowmapentityrenderer.h"
#include "../toolbox/matrix.h"
#include "../entities/entity.h"
#include "../models/models.h"
#include "../toolbox/maths.h"

ShadowMapEntityRenderer::ShadowMapEntityRenderer(ShadowShader* shader, Matrix4f* projectionViewMatrix)
{
    this->shader = shader;
    this->projectionViewMatrix = projectionViewMatrix;
}

void ShadowMapEntityRenderer::render(std::unordered_map<TexturedModel*, std::list<Entity*>>* entities)
{
    for (auto entry : (*entities))
    {
        TexturedModel* texturedModel = entry.first;
        RawModel* rawModel = texturedModel->getRawModel();
        bindModel(rawModel);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturedModel->getTexture()->getID());

        std::list<Entity*>* entityList = &entry.second;

        for (Entity* entity : (*entityList))
        {
            prepareInstance(entity);
            glDrawElements(GL_TRIANGLES, rawModel->getVertexCount(), GL_UNSIGNED_INT, 0);
        }
    }
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void ShadowMapEntityRenderer::bindModel(RawModel* rawModel)
{
    glBindVertexArray(rawModel->getVaoID());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void ShadowMapEntityRenderer::prepareInstance(Entity* entity)
{
    Matrix4f modelMatrix;
    Maths::createTransformationMatrix(
        &modelMatrix,         entity->getPosition(), 
        entity->getRotX(),    entity->getRotY(),     entity->getRotZ(),
        entity->getRotSpin(), entity->getScale());

    Matrix4f mvpMatrix;
    projectionViewMatrix->multiply(&modelMatrix, &mvpMatrix);
    shader->loadMvpMatrix(&mvpMatrix);
}
