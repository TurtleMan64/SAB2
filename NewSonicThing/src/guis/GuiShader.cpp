#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "guishader.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../toolbox/maths.h"
#include "../renderEngine/renderEngine.h"

GuiShader::GuiShader(const char* vertexFile, const char* fragmentFile)
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

void GuiShader::start()
{
	glUseProgram(programID);
}

void GuiShader::stop()
{
	glUseProgram(0);
}

void GuiShader::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void GuiShader::loadTransformation(Matrix4f* matrix)
{
	loadMatrix(location_transformationMatrix, matrix);
}

void GuiShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void GuiShader::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void GuiShader::getAllUniformLocations()
{
	location_transformationMatrix = getUniformLocation("transformationMatrix");
}

int GuiShader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

float guiMatrixBuffer[16];

void GuiShader::loadMatrix(int location, Matrix4f* matrix)
{
	matrix->store(guiMatrixBuffer);
	glUniformMatrix4fv(location, 1, GL_FALSE, guiMatrixBuffer);
}
