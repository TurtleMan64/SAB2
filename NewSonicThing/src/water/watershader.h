#ifndef WATERSHADER_H
#define WATERSHADER_H

class Matrix4f;
class Camera;
class Light;
class Vector3f;

#include <glad/glad.h>


class WaterShader
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
	int location_reflectionTexture;
	int location_refractionTexture;
	int location_dudvMap;
	int location_moveFactor;
	int location_cameraPosition;
	int location_normalMap;
	int location_lightPosition;
	int location_lightColour;
	int location_depthMap;
	int location_shadowMap;
	int location_toShadowMapSpace;

	float matrixBuffer[16];

public:
	WaterShader();

	void start();

	void stop();

	void cleanUp();

	void connectTextureUnits();

	void loadLight(Light* sun);

	void loadMoveFactor(float factor);

	void loadProjectionMatrix(Matrix4f* projection);

	void loadViewMatrix(Camera* camera);

	void loadModelMatrix(Matrix4f* modelMatrix);

	void loadToShadowSpaceMatrix(Matrix4f* matrix);

protected:
	void bindAttributes();

	void bindAttribute(int attatchment, const char* variableName);

	void bindFragOutput(int attatchment, const char* variableName);

	void getAllUniformLocations();

	int getUniformLocation(const char*);

	void loadInt(int location, int value);

	void loadFloat(int, float);

	void loadVector(int, Vector3f*);

	void loadBoolean(int, float);

	void loadMatrix(int, Matrix4f*);
};

#endif