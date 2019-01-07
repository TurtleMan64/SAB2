#ifndef SHADOWSHADER2_H
#define SHADOWSHADER2_H

class Matrix4f;
class Vector3f;

#include <glad/glad.h>


class ShadowShader2
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	int location_mvpMatrix;

public:
	ShadowShader2(const char* vertFile, const char* fragFile);

	void start();

	void stop();

	void cleanUp();

	void loadMvpMatrix(Matrix4f* mvpMatrix);


protected:
	void bindAttributes();

	void bindAttribute(int, const char* attrName);

	void getAllUniformLocations();

	int getUniformLocation(const char* uniName);

	void loadMatrix(int, Matrix4f* mat);
};

#endif