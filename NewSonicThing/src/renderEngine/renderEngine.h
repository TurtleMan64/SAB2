#ifndef RENDERENGINE_H
#define RENDERENGINE_H

class ShaderProgram;
class Entity;
class Matrix4f;
class Light;
class Camera;
class ShadowMapMasterRenderer;
class ShadowMapMasterRenderer2;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include "../models/models.h"


//DisplayManager
int createDisplay();
void updateDisplay();
void closeDisplay();
int displayWantsToClose();
GLFWwindow* getWindow();

//Master Renderer
void Master_render(Camera* camera, float clipX, float clipY, float clipZ, float clipW, float waterBlendAmount);

void Master_cleanUp();

void Master_processEntity(Entity* entity);
//void Master_processEntityPass2(Entity* entity);
//void Master_processEntityPass3(Entity* entity);
//void Master_processTransparentEntity(Entity* entity);

void Master_clearAllEntities();
//void Master_clearEntitiesPass2();
//void Master_clearEntitiesPass3();
//void Master_clearTransparentEntities();

void Master_init();

void Master_enableCulling();

void Master_disableCulling();

void Master_makeProjectionMatrix();

Matrix4f* Master_getProjectionMatrix();

float Master_getVFOV();

float Master_getNearPlane();

float Master_getFarPlane();

GLuint Master_getShadowMapTexture();

ShadowMapMasterRenderer* Master_getShadowRenderer();

GLuint Master_getShadowMapTexture2();

ShadowMapMasterRenderer2* Master_getShadowRenderer2();

void Master_renderShadowMaps(Light* sun);

//Renderer
class EntityRenderer
{
private:
    float clockTime;

    ShaderProgram* shader;

    void prepareTexturedModel(TexturedModel* model);

    void unbindTexturedModel();

    void prepareInstance(Entity* entity);

public:
    EntityRenderer(ShaderProgram* shader, Matrix4f* projectionMatrix);

    void render(Entity*);

    void renderNEW(std::unordered_map<TexturedModel*, std::list<Entity*>>* entities, Matrix4f* toShadowSpaceFar, Matrix4f* toShadowSpaceClose);

    void updateProjectionMatrix(Matrix4f* projectionMatrix);

};
#endif
