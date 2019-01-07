#ifndef VERTICALBLURSHADER_H
#define VERTICALBLURSHADER_H

#include <glad/glad.h>

class VerticalBlurShader
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	int location_targetHeight;

public:
	VerticalBlurShader(const char* vFile, const char* fFile);

	void start();

	void stop();

	void cleanUp();

	void loadTargetHeight(float height);

protected:
	void bindAttributes();

	void bindAttribute(int attribute, const char* variableName);

	void getAllUniformLocations();

	int getUniformLocation(const char* uniformName);

	void loadFloat(int location, float value);
};

#endif