#include "horizontalblurshader.h"
#include "../renderEngine/renderEngine.h"

#include <glad/glad.h>

HorizontalBlurShader::HorizontalBlurShader(const char* vFile, const char* fFile)
{
	vertexShaderID = Loader::loadShader(vFile, GL_VERTEX_SHADER);
	fragmentShaderID = Loader::loadShader(fFile, GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	getAllUniformLocations();
}

void HorizontalBlurShader::start()
{
	glUseProgram(programID);
}

void HorizontalBlurShader::stop()
{
	glUseProgram(0);
}

void HorizontalBlurShader::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void HorizontalBlurShader::loadTargetWidth(float width)
{
	loadFloat(location_targetWidth, width);
}

void HorizontalBlurShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void HorizontalBlurShader::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void HorizontalBlurShader::getAllUniformLocations()
{
	location_targetWidth = getUniformLocation("targetWidth");
}

int HorizontalBlurShader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

void HorizontalBlurShader::loadFloat(int location, float value)
{
	glUniform1f(location, value);
}
