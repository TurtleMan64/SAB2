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
#include "../engineTester/main.h"
#include "shaderprogram.h"

float matrixBuffer[16];

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile)
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

void ShaderProgram::start()
{
	glUseProgram(programID);
}

void ShaderProgram::stop()
{
	glUseProgram(0);
}

void ShaderProgram::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}

void ShaderProgram::loadTransformationMatrix(Matrix4f* matrix)
{
	loadMatrix(location_transformationMatrix, matrix);
}

void ShaderProgram::loadProjectionMatrix(Matrix4f* projection)
{
	loadMatrix(location_projectionMatrix, projection);
}

void ShaderProgram::loadViewMatrix(Camera* cam)
{
	Matrix4f viewMatrix;
	Maths::createViewMatrix(&viewMatrix, cam);
	loadMatrix(location_viewMatrix, &viewMatrix);
}

void ShaderProgram::loadLight(Light* light)
{
	loadVector(location_lightPosition, light->getPosition());
	loadVector(location_lightColour, light->getColour());
}

void ShaderProgram::loadShineVariables(float damper, float reflectivity)
{
	loadFloat(location_shineDamper, damper);
	loadFloat(location_reflectivity, reflectivity);
}

void ShaderProgram::loadFakeLighting(int fakeLighting)
{
	loadFloat(location_useFakeLighting, (float)fakeLighting);
}

void ShaderProgram::loadTransparency(int transparency)
{
	loadInt(location_hasTransparency, transparency);
}

void ShaderProgram::loadGlowAmount(float glowAmount)
{
	loadFloat(location_glowAmount, glowAmount);
}

void ShaderProgram::loadBaseColour(Vector3f* baseColour)
{
	//std::fprintf(stdout, "baseColour = %f %f %f\n", baseColour->x, baseColour->y, baseColour->z);
	loadVector(location_baseColour, baseColour);
}

void ShaderProgram::loadTextureOffsets(float offX, float offY)
{
	loadFloat(location_texOffX, offX);
	loadFloat(location_texOffY, offY);
}

void ShaderProgram::loadSkyColour(float r, float g, float b)
{
	Vector3f newColour(r, g, b);
	loadVector(location_skyColour, &newColour);
}

void ShaderProgram::loadFogDensity(float density)
{
	loadFloat(location_fogDensity, density);
}

void ShaderProgram::loadFogGradient(float gradient)
{
	loadFloat(location_fogGradient, gradient);
}

void ShaderProgram::bindAttributes()
{
	if (Global::renderBloom)
	{
		bindFragOutput(0, "out_Colour");
		bindFragOutput(1, "out_BrightColour");
	}

	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
	bindAttribute(3, "vertexColor");
}

void ShaderProgram::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void ShaderProgram::bindFragOutput(int attatchment, const char* variableName)
{
	glBindFragDataLocation(programID, attatchment, variableName);
}

void ShaderProgram::getAllUniformLocations()
{
	location_transformationMatrix  = getUniformLocation("transformationMatrix");
	location_projectionMatrix      = getUniformLocation("projectionMatrix");
	location_viewMatrix            = getUniformLocation("viewMatrix");
	location_lightPosition         = getUniformLocation("lightPosition");
	location_lightColour           = getUniformLocation("lightColour");
	location_shineDamper           = getUniformLocation("shineDamper");
	location_reflectivity          = getUniformLocation("reflectivity");
	location_useFakeLighting       = getUniformLocation("useFakeLighting");
	location_hasTransparency       = getUniformLocation("hasTransparency");
	location_glowAmount            = getUniformLocation("glowAmount");
	location_baseColour            = getUniformLocation("baseColour");
	location_texOffX               = getUniformLocation("texOffX");
	location_texOffY               = getUniformLocation("texOffY");
	location_skyColour             = getUniformLocation("skyColour");
	location_fogDensity            = getUniformLocation("fogDensity");
	location_fogGradient           = getUniformLocation("fogGradient");
	location_clipPlane             = getUniformLocation("clipPlane");
	location_clipPlaneBehind       = getUniformLocation("clipPlaneBehind");
	location_shadowMapFar          = getUniformLocation("shadowMapFar");
	location_toShadowMapSpaceFar   = getUniformLocation("toShadowMapSpaceFar");
	location_shadowMapClose        = getUniformLocation("shadowMapClose");
	location_toShadowMapSpaceClose = getUniformLocation("toShadowMapSpaceClose");
	location_randomMap             = getUniformLocation("randomMap");
    location_mixFactor             = getUniformLocation("mixFactor");
    location_textureSampler2       = getUniformLocation("textureSampler2");
    location_fogScale              = getUniformLocation("fogScale");
}

int ShaderProgram::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

void ShaderProgram::loadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void ShaderProgram::loadInt(int location, int value)
{
	glUniform1i(location, value);
}

void ShaderProgram::loadVector(int location, Vector3f* vect)
{
	glUniform3f(location, vect->x, vect->y, vect->z);
}

void ShaderProgram::loadBoolean(int location, bool value)
{
	glUniform1i(location, (int)(value));
}

void ShaderProgram::loadMatrix(int location, Matrix4f* matrix)
{
	matrix->store(matrixBuffer);
	glUniformMatrix4fv(location, 1, GL_FALSE, matrixBuffer);
}

void ShaderProgram::loadClipPlane(float clipX, float clipY, float clipZ, float clipW)
{
	glUniform4f(location_clipPlane, clipX, clipY, clipZ, clipW);
}

void ShaderProgram::loadClipPlaneBehind(float clipX, float clipY, float clipZ, float clipW)
{
	glUniform4f(location_clipPlaneBehind, clipX, clipY, clipZ, clipW);
}

void ShaderProgram::connectTextureUnits()
{
	if (Global::renderShadowsFar || Global::renderShadowsClose)
	{
		if (Global::renderShadowsFar)
		{
			loadInt(location_shadowMapFar, 5);
		}
		if (Global::renderShadowsClose)
		{
			loadInt(location_shadowMapClose, 6);
		}

		loadInt(location_randomMap, 7);
	}

    loadInt(location_textureSampler2, 1);
}

void ShaderProgram::loadToShadowSpaceMatrixFar(Matrix4f* matrix)
{
	loadMatrix(location_toShadowMapSpaceFar, matrix);
}

void ShaderProgram::loadToShadowSpaceMatrixClose(Matrix4f* matrix)
{
	loadMatrix(location_toShadowMapSpaceClose, matrix);
}

void ShaderProgram::loadMixFactor(float factor)
{
    loadFloat(location_mixFactor, factor);
}

void ShaderProgram::loadFogScale(float scale)
{
    loadFloat(location_fogScale, scale);
}
