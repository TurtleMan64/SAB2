#ifndef HORIZONTALBLURSHADER_H
#define HORIZONTALBLURSHADER_H

#include <glad/glad.h>

class HorizontalBlurShader
{
private:
	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	int location_targetWidth;

public:
	HorizontalBlurShader(const char* vFile, const char* fFile);

	void start();

	void stop();

	void cleanUp();

	void loadTargetWidth(float width);

protected:
	void bindAttributes();

	void bindAttribute(int attribute, const char* variableName);

	void getAllUniformLocations();

	int getUniformLocation(const char* uniformName);

	void loadFloat(int location, float value);
};

#endif