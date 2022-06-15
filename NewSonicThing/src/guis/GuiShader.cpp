#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "guishader.hpp"
#include "../toolbox/vector.hpp"
#include "../toolbox/matrix.hpp"
#include "../toolbox/maths.hpp"
#include "../loading/loadergl.hpp"

GuiShader::GuiShader(const char* vertexFile, const char* fragmentFile)
{
    vertexShaderId = LoaderGL::loadShader(vertexFile, GL_VERTEX_SHADER);
    fragmentShaderId = LoaderGL::loadShader(fragmentFile, GL_FRAGMENT_SHADER);
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

void GuiShader::loadAlpha(float alpha)
{
    loadFloat(location_alpha, alpha);
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
    location_alpha                = getUniformLocation("alpha");
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

void GuiShader::loadFloat(int location, float alpha)
{
    glUniform1f(location, alpha);
}
