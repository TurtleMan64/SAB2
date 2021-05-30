#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

class Matrix4f;
class Camera;
class Light;
class Vector3f;
class Vector2f;

#include <glad/glad.h>


class ParticleShader
{
private:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    int location_projectionMatrix;
    int location_numberOfRows;
    int location_brightness;
    int location_opacity;
    int location_glow;

    float matrixBuffer[16];

public:
    ParticleShader();

    void start();

    void stop();

    void cleanUp();

    void loadBrightness(float brightness);

    void loadOpacity(float opacity);

    void loadGlow(float glow);

    void loadProjectionMatrix(Matrix4f* projectionMatrix);

    void loadNumberOfRows(float numRows);


protected:
    void bindAttributes();

    void bindAttribute(int attatchment, const char* variableName);

    void getAllUniformLocations();

    int getUniformLocation(const char* uniformName);

    void loadInt(int location, int value);

    void loadFloat(int, float);

    void loadVector(int, Vector3f*);

    void load2DVector(int location, Vector2f* vector);

    void loadBoolean(int, float);

    void loadMatrix(int, Matrix4f*);
};

#endif
