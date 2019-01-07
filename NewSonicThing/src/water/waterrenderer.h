#ifndef WATERRENDERER_H
#define WATERRENDERER_H

class WaterShader;
class WaterFrameBuffers;
class Matrix4f;
class RawModel;
class Camera;
class WaterTile;
class Camera;
class Light;
class ShadowMapMasterRenderer;


#include "../renderEngine/renderEngine.h"
#include <list>
#include <glad/glad.h>

class WaterRenderer
{
private:
	float moveFactor;
	GLuint dudvTexture;
	GLuint normalMap;

	RawModel* quad;
	WaterShader* shader;
	WaterFrameBuffers* fbos;

	ShadowMapMasterRenderer* shadowMapRenderer;

	void prepareRender(Camera* camera, Light* sun);

	void unbind();

	void setUpVAO();

public:
	static const float WAVE_SPEED;

	WaterRenderer(WaterShader* shader, Matrix4f* projectionMatrix, WaterFrameBuffers* fbos, ShadowMapMasterRenderer* shadowMapRenderer);

	void render(std::list<WaterTile*>* water, Camera* camera, Light* sun);

	void updateProjectionMatrix(Matrix4f* projectionMatrix);
};
#endif