#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderEngine.h"
#include "../entities/entity.h"
#include "../shaders/shaderprogram.h"
#include "../toolbox/matrix.h"
#include "../toolbox/vector.h"
#include "../toolbox/maths.h"
#include "../models/models.h"
#include "../engineTester/main.h"
#include "skymanager.h"

#include <iostream>
#include <unordered_map>
#include <list>

EntityRenderer::EntityRenderer(ShaderProgram* shader, Matrix4f* projectionMatrix)
{
	shader->start();
	shader->loadProjectionMatrix(projectionMatrix);
	shader->stop();
	this->shader = shader;
}

void EntityRenderer::renderNEW(std::unordered_map<TexturedModel*, std::list<Entity*>>* entitiesMap, Matrix4f* toShadowSpaceFar, Matrix4f* toShadowSpaceClose)
{
	if (Global::renderShadowsFar)
	{
		shader->loadToShadowSpaceMatrixFar(toShadowSpaceFar);
	}

	if (Global::renderShadowsClose)
	{
		shader->loadToShadowSpaceMatrixClose(toShadowSpaceClose);
	}

	clockTime = Global::gameClock;

	shader->loadFogGradient(SkyManager::getFogGradient());
	shader->loadFogDensity(SkyManager::getFogDensity());

	for (auto entry : (*entitiesMap))
	{
		prepareTexturedModel(entry.first);
		std::list<Entity*>* entityList = &entry.second;

		for (Entity* entity : (*entityList))
		{
			prepareInstance(entity);
			glDrawElements(GL_TRIANGLES, (entry.first)->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
		}
		unbindTexturedModel();
	}
}

void EntityRenderer::prepareTexturedModel(TexturedModel* model)
{
	RawModel* rawModel = model->getRawModel();
	glBindVertexArray(rawModel->getVaoID());
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
	shader->loadFakeLighting(texture->getUsesFakeLighting());
	shader->loadShineVariables(texture->getShineDamper(), texture->getReflectivity());
	shader->loadTransparency(texture->getHasTransparency());
	shader->loadGlowAmount(texture->getGlowAmount());
	shader->loadTextureOffsets(clockTime*texture->getScrollX(), clockTime*texture->getScrollY());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getID());
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
	shader->loadTransformationMatrix(entity->getTransformationMatrix());
	shader->loadBaseColour(entity->getBaseColour());
}

void EntityRenderer::render(Entity* entity)
{
	if (entity->getVisible() == false)
	{
		return;
	}

	prepareInstance(entity);

	std::list<TexturedModel*>* models = entity->getModels();

	for (auto texturedModel : (*models))
	{
		RawModel* model = texturedModel->getRawModel();

		prepareTexturedModel(texturedModel);

		glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);

		unbindTexturedModel();
	}
}

void EntityRenderer::updateProjectionMatrix(Matrix4f* projectionMatrix)
{
	this->shader->start();
	this->shader->loadProjectionMatrix(projectionMatrix);
	this->shader->stop();
}