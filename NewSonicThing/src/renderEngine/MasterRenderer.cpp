#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../entities/light.h"
#include "../entities/camera.h"
#include "../shaders/shaderprogram.h"
#include "../entities/entity.h"
#include "../models/models.h"
#include "renderEngine.h"
#include "../toolbox/maths.h"
#include "../toolbox/matrix.h"
#include "../engineTester/main.h"
#include "skymanager.h"
#include "../water/waterrenderer.h"
#include "../particles/particlemaster.h"
#include "../shadows/shadowmapmasterrenderer.h"
#include "../shadows2/shadowmapmasterrenderer2.h"

#include <iostream>
#include <list>
#include <unordered_map>
#include <stdexcept>

ShaderProgram* shader;
EntityRenderer* renderer;
ShadowMapMasterRenderer* shadowMapRenderer;
ShadowMapMasterRenderer2* shadowMapRenderer2;

std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMap;
std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapPass2;
std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesMapPass3;
std::unordered_map<TexturedModel*, std::list<Entity*>> entitiesTransparentMap;

Matrix4f* projectionMatrix;

float VFOV_BASE = 60; //Vertical fov
float VFOV_ADDITION = 0; //additional fov due to the vehicle going fast
const float NEAR_PLANE = 1.5f; //0.5
const float FAR_PLANE = 60000; //15000

float RED = 0.9f;
float GREEN = 0.95f;
float BLUE = 1.0f;

void prepare();
void prepareTransparentRender();
void prepareTransparentRenderDepthOnly();

GLuint randomMap = GL_NONE;

void Master_init()
{
	if (Global::renderShadowsFar)
	{
		if (Global::renderShadowsClose)
		{
			if (Global::renderBloom)
			{
				switch (Global::shadowsFarQuality)
				{
				case 0:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowBoth.txt", "res/Shaders/entity/fragmentShaderShadow1BothBloom.txt");  break;
				case 1:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowBoth.txt", "res/Shaders/entity/fragmentShaderShadow9BothBloom.txt");  break;
				default: shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowBoth.txt", "res/Shaders/entity/fragmentShaderShadow25BothBloom.txt"); break;
				}
			}
			else
			{
				switch (Global::shadowsFarQuality)
				{
				case 0:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowBoth.txt", "res/Shaders/entity/fragmentShaderShadow1Both.txt");  break;
				case 1:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowBoth.txt", "res/Shaders/entity/fragmentShaderShadow9Both.txt");  break;
				default: shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowBoth.txt", "res/Shaders/entity/fragmentShaderShadow25Both.txt"); break;
				}
			}
		}
		else
		{
			if (Global::renderBloom)
			{
				switch (Global::shadowsFarQuality)
				{
				case 0:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowFar.txt", "res/Shaders/entity/fragmentShaderShadow1FarBloom.txt");  break;
				case 1:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowFar.txt", "res/Shaders/entity/fragmentShaderShadow9FarBloom.txt");  break;
				default: shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowFar.txt", "res/Shaders/entity/fragmentShaderShadow25FarBloom.txt"); break;
				}
			}
			else
			{
				switch (Global::shadowsFarQuality)
				{
				case 0:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowFar.txt", "res/Shaders/entity/fragmentShaderShadow1Far.txt");  break;
				case 1:  shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowFar.txt", "res/Shaders/entity/fragmentShaderShadow9Far.txt");  break;
				default: shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowFar.txt", "res/Shaders/entity/fragmentShaderShadow25Far.txt"); break;
				}
			}
		}
	}
	else
	{
		if (Global::renderShadowsClose)
		{
			if (Global::renderBloom)
			{
				shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowClose.txt", "res/Shaders/entity/fragmentShaderShadowCloseBloom.txt");
			}
			else
			{
				shader = new ShaderProgram("res/Shaders/entity/vertexShaderShadowClose.txt", "res/Shaders/entity/fragmentShaderShadowClose.txt");
			}
		}
		else
		{
			if (Global::renderBloom)
			{
				shader = new ShaderProgram("res/Shaders/entity/vertexShader.txt", "res/Shaders/entity/fragmentShaderBloom.txt");
			}
			else
			{
				shader = new ShaderProgram("res/Shaders/entity/vertexShader.txt", "res/Shaders/entity/fragmentShader.txt");
			}
		}
	}
	INCR_NEW("ShaderProgram");

	projectionMatrix = new Matrix4f; INCR_NEW("Matrix4f");

	renderer = new EntityRenderer(shader, projectionMatrix); INCR_NEW("EntityRenderer");
	Master_makeProjectionMatrix();


	shadowMapRenderer = new ShadowMapMasterRenderer; INCR_NEW("ShadowMapMasterRenderer");
	shadowMapRenderer2 = new ShadowMapMasterRenderer2; INCR_NEW("ShadowMapMasterRenderer2");

	randomMap = Loader::loadTextureNoInterpolation("res/Images/randomMap.png");

	Master_disableCulling();
}

void Master_render(Camera* camera, float clipX, float clipY, float clipZ, float clipW)
{
	prepare();
	shader->start();
	shader->loadClipPlane(clipX, clipY, clipZ, clipW);

	//calc behind clipm plane based on camera
	Vector3f camDir = camera->target - camera->eye;
	camDir.normalize();
	camDir.neg();
	Vector3f startPos(&camera->eye);
	//startPos = startPos + camDir.scaleCopy(-100);
	Vector4f plane = Maths::calcPlaneValues(&startPos, &camDir);
	shader->loadClipPlaneBehind(plane.x, plane.y, plane.z, plane.w);

	RED = SkyManager::getFogRed();
	GREEN = SkyManager::getFogGreen();
	BLUE = SkyManager::getFogBlue();
	shader->loadSkyColour(RED, GREEN, BLUE);
	shader->loadLight(Global::gameLightSun);
	shader->loadViewMatrix(camera);
	shader->connectTextureUnits();

	renderer->renderNEW(&entitiesMap, shadowMapRenderer->getToShadowMapSpaceMatrix(), shadowMapRenderer2->getToShadowMapSpaceMatrix());
	renderer->renderNEW(&entitiesMapPass2, shadowMapRenderer->getToShadowMapSpaceMatrix(), shadowMapRenderer2->getToShadowMapSpaceMatrix());
	renderer->renderNEW(&entitiesMapPass3, shadowMapRenderer->getToShadowMapSpaceMatrix(), shadowMapRenderer2->getToShadowMapSpaceMatrix());

	prepareTransparentRender();
	renderer->renderNEW(&entitiesTransparentMap, shadowMapRenderer->getToShadowMapSpaceMatrix(), shadowMapRenderer2->getToShadowMapSpaceMatrix());
	prepareTransparentRenderDepthOnly();
	renderer->renderNEW(&entitiesTransparentMap, shadowMapRenderer->getToShadowMapSpaceMatrix(), shadowMapRenderer2->getToShadowMapSpaceMatrix());

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	shader->stop();
}

void Master_processEntity(Entity* entity)
{
	if (!entity->visible)
	{
		return;
	}

	std::list<TexturedModel*>* modellist = entity->getModels();
	for (TexturedModel* entityModel : (*modellist))
	{
		std::list<Entity*>* list = &entitiesMap[entityModel];
		list->push_back(entity);
	}
}

void Master_processEntityPass2(Entity* entity)
{
	if (!entity->visible)
	{
		return;
	}

	std::list<TexturedModel*>* modellist = entity->getModels();
	for (TexturedModel* entityModel : (*modellist))
	{
		std::list<Entity*>* list = &entitiesMapPass2[entityModel];
		list->push_back(entity);
	}
}

void Master_processEntityPass3(Entity* entity)
{
	if (!entity->visible)
	{
		return;
	}

	std::list<TexturedModel*>* modellist = entity->getModels();
	for (TexturedModel* entityModel : (*modellist))
	{
		std::list<Entity*>* list = &entitiesMapPass3[entityModel];
		list->push_back(entity);
	}
}

void Master_processTransparentEntity(Entity* entity)
{
	if (!entity->visible)
	{
		return;
	}

	std::list<TexturedModel*>* modellist = entity->getModels();
	for (TexturedModel* entityModel : (*modellist))
	{
		std::list<Entity*>* list = &entitiesTransparentMap[entityModel];
		list->push_back(entity);
	}
}

void Master_clearEntities()
{
	entitiesMap.clear();
}

void Master_clearEntitiesPass2()
{
	entitiesMapPass2.clear();
}

void Master_clearEntitiesPass3()
{
	entitiesMapPass3.clear();
}

void Master_clearTransparentEntities()
{
	entitiesTransparentMap.clear();
}

void prepare()
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(RED, GREEN, BLUE, 1);

	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, Master_getShadowMapTexture());

	//glActiveTexture(GL_TEXTURE6);
	//glBindTexture(GL_TEXTURE_2D, Master_getShadowMapTexture2());

	//glActiveTexture(GL_TEXTURE7);
	//glBindTexture(GL_TEXTURE_2D, randomMap);

    if (Global::renderWithCulling)
    {
        Master_enableCulling();
    }
    else
    {
        Master_disableCulling();
    }
}

void prepareTransparentRender()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(false);

    if (Global::renderWithCulling)
    {
        Master_enableCulling();
    }
    else
    {
        Master_disableCulling();
    }
}

void prepareTransparentRenderDepthOnly()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);

    if (Global::renderWithCulling)
    {
        Master_enableCulling();
    }
    else
    {
        Master_disableCulling();
    }
}

void Master_cleanUp()
{
	shader->cleanUp();
	delete shader; INCR_DEL("ShaderProgram");
	delete renderer; INCR_DEL("EntityRenderer");
	delete projectionMatrix; INCR_DEL("Matrix4f");

	shadowMapRenderer->cleanUp();
	delete shadowMapRenderer; INCR_DEL("ShadowMapMasterRenderer");

	shadowMapRenderer2->cleanUp();
	delete shadowMapRenderer2; INCR_DEL("ShadowMapMasterRenderer2");
}

void Master_enableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Master_disableCulling()
{
	glDisable(GL_CULL_FACE);
}

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

void Master_makeProjectionMatrix()
{
	int displayWidth;
	int displayHeight;
	glfwGetWindowSize(getWindow(), &displayWidth, &displayHeight);

	float aspectRatio = (float)displayWidth / (float)displayHeight;


	//FOV = 50;
	float y_scale = 1.0f / tanf(Maths::toRadians((VFOV_BASE+VFOV_ADDITION) / 2.0f));
	float x_scale = y_scale / aspectRatio;


	//FOV = 88.88888;
	//float x_scale = (float)((1.0f / tan(toRadians(HFOV / 2.0f))));
	//float y_scale = x_scale * aspectRatio;


	float frustum_length = FAR_PLANE - NEAR_PLANE;

	projectionMatrix->m00 = x_scale;
	projectionMatrix->m11 = y_scale;
	projectionMatrix->m22 = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix->m23 = -1;
	projectionMatrix->m32 = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix->m33 = 0;

	renderer->updateProjectionMatrix(projectionMatrix);

	if (Global::renderParticles)
	{
		ParticleMaster::updateProjectionMatrix(projectionMatrix);
	}
}

Matrix4f* Master_getProjectionMatrix()
{
	return projectionMatrix;
}

float Master_getVFOV()
{
	return VFOV_BASE;
}

float Master_getNearPlane()
{
	return NEAR_PLANE;
}

float Master_getFarPlane()
{
	return FAR_PLANE;
}

GLuint Master_getShadowMapTexture()
{
	return shadowMapRenderer->getShadowMap();
}

GLuint Master_getShadowMapTexture2()
{
	return shadowMapRenderer2->getShadowMap();
}

ShadowMapMasterRenderer* Master_getShadowRenderer()
{
	return shadowMapRenderer;
}

ShadowMapMasterRenderer2* Master_getShadowRenderer2()
{
	return shadowMapRenderer2;
}

void Master_renderShadowMaps(Light* sun)
{
	if (Global::renderShadowsFar)
	{
		shadowMapRenderer->render(&entitiesMap, sun);
	}
	if (Global::renderShadowsClose)
	{
		shadowMapRenderer2->render(&entitiesMap, sun);
	}
}
