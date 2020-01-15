#ifndef WATERSHADER_H
#define WATERSHADER_H

class Matrix4f;
class Camera;
class Light;
class Vector3f;
class Vector4f;

#include <glad/glad.h>


class WaterShader
{
private:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    int location_modelMatrix;
    int location_viewMatrix;
    int location_projectionMatrix;
    int location_reflectionTexture;
    int location_refractionTexture;
    int location_dudvMap;
    int location_moveFactor;
    int location_cameraPosition;
    int location_normalMap;
    int location_sunDirection;
    int location_sunColor;
    int location_depthMap;
    int location_shadowMap;
    int location_toShadowMapSpace;
    int location_waterHeight;
    int location_clipPlaneBehind;

    float matrixBuffer[16];

public:
    WaterShader();

    void start();

    void stop();

    void cleanUp();

    void connectTextureUnits();

    void loadSun(Light* sun);

    void loadWaterHeight(float waterHeight);

    void loadMoveFactor(float factor);

    void loadProjectionMatrix(Matrix4f* projection);

    void loadViewMatrix(Camera* camera);

    void loadModelMatrix(Matrix4f* modelMatrix);

    void loadToShadowSpaceMatrix(Matrix4f* matrix);

    void loadClipPlaneBehind(Vector4f* plane);

protected:
    void bindAttributes();

    void bindAttribute(int attatchment, const char* variableName);

    void bindFragOutput(int attatchment, const char* variableName);

    void getAllUniformLocations();

    int getUniformLocation(const char*);

    void loadInt(int location, int value);

    void loadFloat(int, float);

    void loadVector(int, Vector3f*);

    void loadVector4f(int, Vector4f*);

    void loadBoolean(int, float);

    void loadMatrix(int, Matrix4f*);
};

#endif
