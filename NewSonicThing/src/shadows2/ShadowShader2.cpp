#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "shadowshader2.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../toolbox/maths.h"
#include "../entities/light.h"
#include "../renderEngine/renderEngine.h"

ShadowShader2::ShadowShader2(const char* vertexFile, const char* fragmentFile)
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

void ShadowShader2::start()
{
	glUseProgram(programID);
}

void ShadowShader2::stop()
{
	glUseProgram(0);
}

void ShadowShader2::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void ShadowShader2::loadMvpMatrix(Matrix4f* matrix)
{
	loadMatrix(location_mvpMatrix, matrix);
}

void ShadowShader2::bindAttributes()
{
	bindAttribute(0, "in_position");
	bindAttribute(1, "in_textureCoords");
}

void ShadowShader2::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void ShadowShader2::getAllUniformLocations()
{
	location_mvpMatrix = getUniformLocation("mvpMatrix");
}

int ShadowShader2::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

float shadow2MatrixBuffer[16];

void ShadowShader2::loadMatrix(int location, Matrix4f* matrix)
{
	matrix->store(shadow2MatrixBuffer);
	glUniformMatrix4fv(location, 1, GL_FALSE, shadow2MatrixBuffer);
}
