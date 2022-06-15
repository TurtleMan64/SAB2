#ifndef GUISHADER_H
#define GUISHADER_H

class Matrix4f;
class Vector3f;

#include <glad/glad.h>

class GuiShader
{
private:
    GLuint programId        = GL_NONE;
    GLuint vertexShaderId   = GL_NONE;
    GLuint fragmentShaderId = GL_NONE;

    int location_transformationMatrix;
    int location_alpha;

public:
    GuiShader(const char* vertFile, const char* fragFile);

    void start();

    void stop();

    void cleanUp();

    void loadTransformation(Matrix4f* matrix);

    void loadAlpha(float alpha);

protected:
    void bindAttributes();

    void bindAttribute(int location, const char* attrName);

    void getAllUniformLocations();

    int getUniformLocation(const char* uniName);

    void loadMatrix(int location, Matrix4f* matrix);

    void loadFloat(int location, float alpha);
};

#endif
