#ifndef GUISHADER_H
#define GUISHADER_H

class Matrix4f;
class Vector3f;

#include <glad/glad.h>


class GuiShader
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	int location_transformationMatrix;

public:
	GuiShader(const char* vertFile, const char* fragFile);

	void start();

	void stop();

	void cleanUp();

	void loadTransformation(Matrix4f* matrix);


protected:
	void bindAttributes();

	void bindAttribute(int location, const char* attrName);

	void getAllUniformLocations();

	int getUniformLocation(const char* uniName);

	void loadMatrix(int location, Matrix4f* matrix);
};

#endif