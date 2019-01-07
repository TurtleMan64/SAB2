#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

class Matrix4f;
class Camera;
class Light;
class Vector3f;
class Vector2f;

#include <glad/glad.h>


class ParticleShader
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	int location_modelViewMatrix;
	int location_projectionMatrix;
	int location_texOffset1;
	int location_texOffset2;
	int location_texCoordInfo;
	int location_brightness;
	int location_opacity;
	int location_glow;

	float matrixBuffer[16];

public:
	ParticleShader();

	void start();

	void stop();

	void cleanUp();

	void loadTextureCoordInfo(Vector2f* offset1, Vector2f* offset2, float numRows, float blend);

	void loadBrightness(float brightness);

	void loadOpacity(float opacity);

	void loadGlow(float glow);

	void loadProjectionMatrix(Matrix4f* projectionMatrix);

	void loadModelViewMatrix(Matrix4f* modelView);


protected:
	void bindAttributes();

	void bindAttribute(int attatchment, const char* variableName);

	void getAllUniformLocations();

	int getUniformLocation(const char* uniformName);

	void loadInt(int location, int value);

	void loadFloat(int, float);

	void loadVector(int, Vector3f*);

	void load2DVector(int location, Vector2f* vector);

	void loadBoolean(int, float);

	void loadMatrix(int, Matrix4f*);
};

#endif