#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "guishader.h"
#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../toolbox/maths.h"
#include "../loading/loader.h"

GuiShader::GuiShader(const char* vertexFile, const char* fragmentFile)
{
    vertexShaderId = Loader::loadShader(vertexFile, GL_VERTEX_SHADER);
    fragmentShaderId = Loader::loadShader(fragmentFile, GL_FRAGMENT_SHADER);
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bindAttributes();
    glLinkProgram(programId);
    glValidateProgram(programId);
    getAllUniformLocations();
}

void GuiShader::start()
{
    glUseProgram(programId);
}

void GuiShader::stop()
{
    glUseProgram(0);
}

void GuiShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
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
    glBindAttribLocation(programId, attribute, variableName);
}

void GuiShader::getAllUniformLocations()
{
    location_transformationMatrix = getUniformLocation("transformationMatrix");
}

int GuiShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
}

float guiMatrixBuffer[16];

void GuiShader::loadMatrix(int location, Matrix4f* matrix)
{
    matrix->store(guiMatrixBuffer);
    glUniformMatrix4fv(location, 1, GL_FALSE, guiMatrixBuffer);
}
