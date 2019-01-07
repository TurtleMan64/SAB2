#include "verticalblurshader.h"
#include "../renderEngine/renderEngine.h"

#include <glad/glad.h>

VerticalBlurShader::VerticalBlurShader(const char* vFile, const char* fFile)
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

void VerticalBlurShader::start()
{
	glUseProgram(programID);
}

void VerticalBlurShader::stop()
{
	glUseProgram(0);
}

void VerticalBlurShader::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void VerticalBlurShader::loadTargetHeight(float height)
{
	loadFloat(location_targetHeight, height);
}

void VerticalBlurShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void VerticalBlurShader::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void VerticalBlurShader::getAllUniformLocations()
{
	location_targetHeight = getUniformLocation("targetHeight");
}

int VerticalBlurShader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

void VerticalBlurShader::loadFloat(int location, float value)
{
	glUniform1f(location, value);
}
