#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "shadowshader.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../toolbox/maths.h"
#include "../entities/light.h"
#include "../renderEngine/renderEngine.h"

ShadowShader::ShadowShader(const char* vertexFile, const char* fragmentFile)
{
	vertexShaderID = Loader::loadShader(vertexFile, GL_VERTEX_SHADER);
	fragmentShaderID = Loader::loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	getAllUniformLocations();
}

void ShadowShader::start()
{
	glUseProgram(programID);
}

void ShadowShader::stop()
{
	glUseProgram(0);
}

void ShadowShader::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void ShadowShader::loadMvpMatrix(Matrix4f* matrix)
{
	loadMatrix(location_mvpMatrix, matrix);
}

void ShadowShader::bindAttributes()
{
	bindAttribute(0, "in_position");
	bindAttribute(1, "in_textureCoords");
}

void ShadowShader::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void ShadowShader::getAllUniformLocations()
{
	location_mvpMatrix = getUniformLocation("mvpMatrix");
}

int ShadowShader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

float shadowMatrixBuffer[16];

void ShadowShader::loadMatrix(int location, Matrix4f* matrix)
{
	matrix->store(shadowMatrixBuffer);
	glUniformMatrix4fv(location, 1, GL_FALSE, shadowMatrixBuffer);
}
