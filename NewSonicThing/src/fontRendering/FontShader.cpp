#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "../toolbox/vector.h"
#include "../toolbox/matrix.h"
#include "../toolbox/maths.h"
#include "fontshader.h"
#include "../engineTester/main.h"

FontShader::FontShader(const char* vertexFile, const char* fragmentFile)
{
	vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	bindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	getAllUniformLocations();
}

void FontShader::start()
{
	glUseProgram(programID);
}

void FontShader::stop()
{
	glUseProgram(0);
}

void FontShader::cleanUp()
{
	stop();
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(programID);
}


void FontShader::bindAttributes()
{
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
}

void FontShader::bindAttribute(int attribute, const char* variableName)
{
	glBindAttribLocation(programID, attribute, variableName);
}

void FontShader::getAllUniformLocations()
{
	location_colour = getUniformLocation("colour");
	location_translation = getUniformLocation("translation");
}

void FontShader::loadColour(Vector3f* colour)
{
	loadVector(location_colour, colour);
}

void FontShader::loadTranslation(Vector2f* translation)
{
	load2DVector(location_translation, translation);
}

int FontShader::getUniformLocation(const char* uniformName)
{
	return glGetUniformLocation(programID, uniformName);
}

void FontShader::loadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void FontShader::loadVector(int location, Vector3f* vect)
{
	glUniform3f(location, vect->x, vect->y, vect->z);
}

void FontShader::load2DVector(int location, Vector2f* vect)
{
	glUniform2f(location, vect->x, vect->y);
}

void FontShader::loadBoolean(int location, float value)
{
	glUniform1f(location, round(value));
}

void FontShader::loadMatrix(int location, Matrix4f* matrix)
{
	matrix->store(matrixBuffer);
	glUniformMatrix4fv(location, 1, GL_FALSE, matrixBuffer);
}

GLuint FontShader::loadShader(const char* fileName, int type)
{
	std::ifstream sourceFile;
	sourceFile.open(fileName);
	std::string filetext;

	if (sourceFile.is_open())
	{
		while (!sourceFile.eof())
		{
			std::string line;
			getline(sourceFile, line);
			filetext.append(line + "\n");
		}

		sourceFile.close();
	}
	else
	{
		std::fprintf(stdout, "Error: Could not find shader file '%s'\n", fileName);
		sourceFile.close();
		return 0;
	}

	unsigned int id = glCreateShader(type);
	const char* src = filetext.c_str();
	//const int len = filetext.size();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}