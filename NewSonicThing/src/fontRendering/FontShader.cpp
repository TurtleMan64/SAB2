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
#include "../toolbox/getline.h"

FontShader::FontShader(const char* vertexFile, const char* fragmentFile)
{
    vertexShaderId = loadShader(vertexFile, GL_VERTEX_SHADER);
    fragmentShaderId = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    bindAttributes();
    glLinkProgram(programId);
    glValidateProgram(programId);
    getAllUniformLocations();
}

void FontShader::start()
{
    glUseProgram(programId);
}

void FontShader::stop()
{
    glUseProgram(0);
}

void FontShader::cleanUp()
{
    stop();
    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    glDeleteProgram(programId);
}


void FontShader::bindAttributes()
{
    bindAttribute(0, "position");
    bindAttribute(1, "textureCoords");
}

void FontShader::bindAttribute(int attribute, const char* variableName)
{
    glBindAttribLocation(programId, attribute, variableName);
}

void FontShader::getAllUniformLocations()
{
    location_colour = getUniformLocation("colour");
    location_translation = getUniformLocation("translation");
    location_scale = getUniformLocation("scale");
}

void FontShader::loadColour(Vector3f* colour)
{
    loadVector(location_colour, colour);
}

void FontShader::loadTranslation(Vector2f* translation)
{
    load2DVector(location_translation, translation);
}

void FontShader::loadScale(float scale)
{
    loadFloat(location_scale, scale);
}

int FontShader::getUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(programId, uniformName);
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
    sourceFile.open(Global::pathToEXE+fileName);
    std::string filetext;

    if (sourceFile.is_open())
    {
        while (!sourceFile.eof())
        {
            std::string line;
            getlineSafe(sourceFile, line);
            filetext.append(line + "\n");
        }

        sourceFile.close();
    }
    else
    {
        std::fprintf(stdout, "Error: Could not find shader file '%s'\n", (Global::pathToEXE+fileName).c_str());
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
