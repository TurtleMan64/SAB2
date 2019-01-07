#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../entities/camera.h"
#include "../toolbox/maths.h"
#include "../entities/light.h"
#include "../renderEngine/renderEngine.h"
#include "particleshader.h"

ParticleShader::ParticleShader()
{
	vertexShaderID = Loader::loadShader("res/Shaders/particles/particleVShader.txt", GL_VERTEX_SHADER);
	fragmentShaderID = Loader::loadShader("res/Shaders/particles/particleFShader.txt", GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	getAllUniformLocations();
}

void ParticleShader::start()
{
	glUseProgram(programID);
}

void ParticleShader::stop()
{
	glUseProgram(0);
}

void ParticleShader::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void ParticleShader::loadTextureCoordInfo(Vector2f* offset1, Vector2f* offset2, float numRows, float blend)
{
	load2DVector(location_texOffset1, offset1);
	load2DVector(location_texOffset2, offset2);
	Vector2f info(numRows, blend);
	load2DVector(location_texCoordInfo, &info);
}

void ParticleShader::loadBrightness(float brightness)
{
	loadFloat(location_brightness, brightness);
}

void ParticleShader::loadOpacity(float opacity)
{
	loadFloat(location_opacity, opacity);
}

void ParticleShader::loadGlow(float glow)
{
	loadFloat(location_glow, glow);
}

void ParticleShader::loadProjectionMatrix(Matrix4f* projectionMatrix)
{
	loadMatrix(location_projectionMatrix, projectionMatrix);
}

void ParticleShader::loadModelViewMatrix(Matrix4f* modelView)
{
	loadMatrix(location_modelViewMatrix, modelView);
}

void ParticleShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void ParticleShader::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void ParticleShader::getAllUniformLocations()
{
	location_modelViewMatrix  = getUniformLocation("modelViewMatrix");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_texOffset1       = getUniformLocation("texOffset1");
	location_texOffset2       = getUniformLocation("texOffset2");
	location_texCoordInfo     = getUniformLocation("texCoordInfo");
	location_brightness       = getUniformLocation("brightness");
	location_opacity          = getUniformLocation("opacity");
	location_glow             = getUniformLocation("glow");
}

int ParticleShader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

void ParticleShader::loadInt(int location, int value)
{
	glUniform1i(location, value);
}

void ParticleShader::loadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void ParticleShader::loadVector(int location, Vector3f* vect)
{
	glUniform3f(location, vect->x, vect->y, vect->z);
}

void ParticleShader::load2DVector(int location, Vector2f* vector)
{
	glUniform2f(location, vector->x, vector->y);
}

void ParticleShader::loadBoolean(int location, float value)
{
	glUniform1f(location, round(value));
}

void ParticleShader::loadMatrix(int location, Matrix4f* matrix)
{
	matrix->store(matrixBuffer);
	glUniformMatrix4fv(location, 1, GL_FALSE, matrixBuffer);
}
