#ifndef HORIZONTALBLURSHADER_H
#define HORIZONTALBLURSHADER_H

#include <glad/glad.h>

class HorizontalBlurShader
{
private:
    GLuint programId;
    GLuint vertexShaderId;
    GLuint fragmentShaderId;

    int location_targetWidth;
    int location_originalTexture;

public:
    HorizontalBlurShader(const char* vFile, const char* fFile);

    void start();

    void stop();

    void cleanUp();

    void loadTargetWidth(float width);

    void loadOriginalTexture(GLuint texId);

protected:
    void bindAttributes();

    void bindAttribute(int attribute, const char* variableName);

    void getAllUniformLocations();

    int getUniformLocation(const char* uniformName);

    void loadFloat(int location, float value);
};

#endif
