#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "entityrenderer.hpp"
#include "../entities/entity.hpp"
#include "../shaders/entityshader.hpp"
#include "../toolbox/matrix.hpp"
#include "../toolbox/vector.hpp"
#include "../toolbox/maths.hpp"
#include "../models/models.hpp"
#include "../engineTester/main.hpp"
#include "skymanager.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>

EntityRenderer::EntityRenderer(EntityShader* shader, Matrix4f* projectionMatrix)
{
    shader->start();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
    this->shader = shader;
}

void EntityRenderer::renderNEW(std::unordered_map<TexturedModel*, std::vector<Entity*>>* entitiesMap)
{
    clockTime = Global::gameClock;

    for (auto it = entitiesMap->cbegin(); it != entitiesMap->cend(); it++)
    {
        std::vector<Entity*>* entitiesToRender = &(it._Ptr->_Myval.second);
        const int numEntitiesToRender = (int)entitiesToRender->size();

        if (numEntitiesToRender == 0)
        {
            continue;
        }

        prepareTexturedModel(it->first);

        for (int i = 0; i < numEntitiesToRender; i++)
        {
            Entity* entity = entitiesToRender->at(i);
            prepareInstance(entity);
            glDrawElements(GL_TRIANGLES, (it->first)->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
        }

        unbindTexturedModel();
    }
}

void EntityRenderer::prepareTexturedModel(TexturedModel* model)
{
    RawModel* rawModel = model->getRawModel();
    glBindVertexArray(rawModel->getVaoId());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    ModelTexture* texture = model->getTexture();
    //if (texture->getHasTransparency() != 0)
    {
        //Master_disableCulling();
    }
    //else
    {
        //Master_enableCulling();
    }
    shader->loadFakeLighting(texture->useFakeLighting);
    shader->loadShineVariables(texture->shineDamper, texture->reflectivity);
    shader->loadTransparency(texture->hasTransparency);
    shader->loadGlowAmount(texture->glowAmount);
    shader->loadTextureOffsets(clockTime * (texture->scrollX), clockTime * (texture->scrollY));
    //if (texture->hasMultipleImages())
    {
        //printf("mix factor = %f\n", texture->animationSpeed);
    }
    shader->loadMixFactor(texture->mixFactor());
    shader->loadFogScale(texture->fogScale);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture->getId2());
}

void EntityRenderer::unbindTexturedModel()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(Entity* entity)
{
    shader->loadTransformationMatrix(&entity->transformationMatrix);
    shader->loadBaseColor(&entity->baseColor);
    shader->loadBaseAlpha(entity->baseAlpha);
}

void EntityRenderer::render(Entity* entity)
{
    if (!entity->visible)
    {
        return;
    }

    prepareInstance(entity);

    std::vector<TexturedModel*>* models = entity->getModels();

    const int numModels = (int)models->size();
    for (int i = 0; i < numModels; i++)
    {
        TexturedModel* modelTextured = models->at(i);

        RawModel* modelRaw = modelTextured->getRawModel();

        prepareTexturedModel(modelTextured);

        glDrawElements(GL_TRIANGLES, modelRaw->getVertexCount(), GL_UNSIGNED_INT, 0);

        unbindTexturedModel();
    }
}

void EntityRenderer::updateProjectionMatrix(Matrix4f* projectionMatrix)
{
    shader->start();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
}
