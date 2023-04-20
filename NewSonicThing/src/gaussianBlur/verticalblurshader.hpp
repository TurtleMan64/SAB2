#ifndef VERTICALBLURSHADER_H
#define VERTICALBLURSHADER_H

#include <glad/glad.h>

class VerticalBlurShader
{
private:
    GLuint programId;
    GLuint vertexShaderId;
    GLuint fragmentShaderId;

    int location_targetHeight;
    int location_originalTexture;

public:
    VerticalBlurShader(const char* vFile, const char* fFile);

    void start();

    void stop();

    void cleanUp();

    void loadTargetHeight(float height);

    void loadOriginalTexture(GLuint texId);

protected:
    void bindAttributes();

    void bindAttribute(int attribute, const char* variableName);

    void getAllUniformLocations();

    int getUniformLocation(const char* uniformName);

    void loadFloat(int location, float value);
};

#endif
