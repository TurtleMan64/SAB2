#include <glad/glad.h>

#include "../entities/light.hpp"
#include "../entities/camera.hpp"
#include "masterrenderer.hpp"
#include "../shaders/entityshader.hpp"
#include "entityrenderer.hpp"
#include "../entities/entity.hpp"
#include "../models/models.hpp"
#include "../loading/loadergl.hpp"
#include "display.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/matrix.hpp"
#include "../engineTester/main.hpp"
#include "skymanager.hpp"
#include "../water/waterrenderer.hpp"
#include "../particles/particlemaster.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>

EntityShader* MasterRenderer::entityShader = nullptr;
EntityRenderer* MasterRenderer::entityRenderer = nullptr;

std::unordered_map<TexturedModel*, std::vector<Entity*>> MasterRenderer::entitiesMap;
std::unordered_map<TexturedModel*, std::vector<Entity*>> MasterRenderer::entitiesMapPass2;
std::unordered_map<TexturedModel*, std::vector<Entity*>> MasterRenderer::entitiesMapPass3;
std::unordered_map<TexturedModel*, std::vector<Entity*>> MasterRenderer::entitiesMapNoDepth;
std::unordered_map<TexturedModel*, std::vector<Entity*>> MasterRenderer::entitiesMapTransparent;
std::unordered_map<TexturedModel*, std::vector<Entity*>> MasterRenderer::entitiesMapPass6;

Matrix4f* MasterRenderer::projectionMatrix = nullptr;

GLuint MasterRenderer::transparentFrameBuffer  = GL_NONE;
GLuint MasterRenderer::transparentDepthTexture = GL_NONE;
GLuint MasterRenderer::currentBoundFramebuffer = GL_NONE;

float MasterRenderer::VFOV_BASE = 75.0f;
float MasterRenderer::VFOV_ADDITION = 0.0f;
float MasterRenderer::FAR_PLANE = 60000.0f;

void MasterRenderer::init()
{
    if (Global::renderBloom)
    {
        entityShader = new EntityShader("res/Shaders/entity/EntityVert.glsl", "res/Shaders/entity/EntityBloomFrag.glsl");
    }
    else
    {
        entityShader = new EntityShader("res/Shaders/entity/EntityVert.glsl", "res/Shaders/entity/EntityFrag.glsl");
    }
    INCR_NEW("ShaderProgram");

    MasterRenderer::projectionMatrix = new Matrix4f; INCR_NEW("Matrix4f");

    MasterRenderer::entityRenderer = new EntityRenderer(MasterRenderer::entityShader, MasterRenderer::projectionMatrix); INCR_NEW("EntityRenderer");
    MasterRenderer::makeProjectionMatrix();

    //create frame buffer
    glGenFramebuffers(1, &MasterRenderer::transparentFrameBuffer); //generate name for frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, MasterRenderer::transparentFrameBuffer); //create the framebuffer
    MasterRenderer::currentBoundFramebuffer = MasterRenderer::transparentFrameBuffer;
    glDrawBuffer(GL_COLOR_ATTACHMENT0); //indicate that we will always render to color attachment 0

    //create a color texture for no reason
    //glGenTextures(1, &transparentColorTexture);
    //glBindTexture(GL_TEXTURE_2D, transparentColorTexture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, transparentColorTexture, 0);

    //create depth texture attachement
    glGenTextures(1, &MasterRenderer::transparentDepthTexture);
    glBindTexture(GL_TEXTURE_2D, MasterRenderer::transparentDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);  //here is the depth bits
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, MasterRenderer::transparentDepthTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    MasterRenderer::currentBoundFramebuffer = 0;
    glViewport(0, 0, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);

    MasterRenderer::disableCulling();
}

void MasterRenderer::render(Camera* camera, float clipX, float clipY, float clipZ, float clipW, float waterBlendAmount)
{
    ANALYSIS_START("Master Render");

    float fogRed   = SkyManager::getFogRed();
    float fogGreen = SkyManager::getFogGreen();
    float fogBlue  = SkyManager::getFogBlue();

    //glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    MasterRenderer::prepareNormalRender();
    glClearColor(fogRed, fogGreen, fogBlue, 1.0f);
    MasterRenderer::entityShader->start();
    MasterRenderer::entityShader->loadClipPlane(clipX, clipY, clipZ, clipW);

    //calc behind clipm plane based on camera
    Vector3f camDir = camera->target - camera->eye;
    camDir.normalize();
    camDir.neg();
    Vector3f startPos(&camera->eye);
    Vector4f plane = Maths::calcPlaneValues(&startPos, &camDir);

    MasterRenderer::entityShader->loadClipPlaneBehind(plane.x, plane.y, plane.z, plane.w);
    MasterRenderer::entityShader->loadSkyColor(fogRed, fogGreen, fogBlue);
    MasterRenderer::entityShader->loadSun(Global::gameLightSun);
    MasterRenderer::entityShader->loadFogGradient(SkyManager::getFogGradient());
    MasterRenderer::entityShader->loadFogDensity(SkyManager::getFogDensity());
    MasterRenderer::entityShader->loadFogBottomPosition(SkyManager::fogBottomPosition);
    MasterRenderer::entityShader->loadFogBottomThickness(SkyManager::fogBottomThickness);
    MasterRenderer::entityShader->loadViewMatrix(camera);
    MasterRenderer::entityShader->loadIsRenderingTransparent(false);
    MasterRenderer::entityShader->loadIsRenderingDepth(false);
    MasterRenderer::entityShader->loadWaterColor(&Global::stageWaterColor);
    MasterRenderer::entityShader->loadWaterBlendAmount(waterBlendAmount);
    MasterRenderer::entityShader->connectTextureUnits();

    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMap);
    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapPass2);
    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapPass3);

    glDepthMask(false);
    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapNoDepth);
    MasterRenderer::prepareRenderDepthOnly();
    glDepthMask(true);
    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapNoDepth);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, 0);
    MasterRenderer::entityShader->stop();

    //Calculate the depth buffer of all transparent entities, before making a 2nd pass.
    glBindFramebuffer(GL_FRAMEBUFFER, MasterRenderer::transparentFrameBuffer);
    glViewport(0, 0, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);

    glClear(GL_DEPTH_BUFFER_BIT);
    //glDisable(GL_MULTISAMPLE); //dont multisample for depth buffer calc... doesnt really make a difference though

    MasterRenderer::entityShader->start();
    MasterRenderer::entityShader->loadClipPlane(clipX, clipY, clipZ, clipW);
    MasterRenderer::entityShader->loadClipPlaneBehind(plane.x, plane.y, plane.z, plane.w);
    MasterRenderer::entityShader->loadSkyColor(fogRed, fogGreen, fogBlue);
    MasterRenderer::entityShader->loadSun(Global::gameLightSun);
    MasterRenderer::entityShader->loadFogGradient(SkyManager::getFogGradient());
    MasterRenderer::entityShader->loadFogDensity(SkyManager::getFogDensity());
    MasterRenderer::entityShader->loadFogBottomPosition(SkyManager::fogBottomPosition);
    MasterRenderer::entityShader->loadFogBottomThickness(SkyManager::fogBottomThickness);
    MasterRenderer::entityShader->loadViewMatrix(camera);
    MasterRenderer::entityShader->loadIsRenderingTransparent(false);
    MasterRenderer::entityShader->loadIsRenderingDepth(true);
    MasterRenderer::entityShader->loadWaterColor(&Global::stageWaterColor);
    MasterRenderer::entityShader->loadWaterBlendAmount(waterBlendAmount);
    MasterRenderer::entityShader->connectTextureUnits();

    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapTransparent);
    MasterRenderer::entityShader->stop();
    
    //We have finished creating depth buffer for transparent entities. Now go and do a 2nd pass, this time
    // using the depth buffer to only render the pixels closest to the screen.
    glBindFramebuffer(GL_FRAMEBUFFER, MasterRenderer::currentBoundFramebuffer); //Switch back to rendering to whatever we were before.
    glViewport(0, 0, Display::WINDOW_WIDTH, Display::WINDOW_HEIGHT);

    //Note: There is some sort of bug somewhere around here. Transparent entities don't seem to get rendered to the bloom effect.
    // This means that things like the skybox color can "leak" into the bloom if your skybox model is all marked as transparent.
    // It will also just mean in general that transparent entities won't have bloom I think. Not really sure why this is happening
    // or how to fix it.

    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, MasterRenderer::transparentDepthTexture); //Assign the depth buffer we just calculated to TEXTURE8

    MasterRenderer::entityShader->start();
    MasterRenderer::entityShader->loadClipPlane(clipX, clipY, clipZ, clipW);
    MasterRenderer::entityShader->loadClipPlaneBehind(plane.x, plane.y, plane.z, plane.w);
    MasterRenderer::entityShader->loadSkyColor(fogRed, fogGreen, fogBlue);
    MasterRenderer::entityShader->loadSun(Global::gameLightSun);
    MasterRenderer::entityShader->loadFogGradient(SkyManager::getFogGradient());
    MasterRenderer::entityShader->loadFogDensity(SkyManager::getFogDensity());
    MasterRenderer::entityShader->loadFogBottomPosition(SkyManager::fogBottomPosition);
    MasterRenderer::entityShader->loadFogBottomThickness(SkyManager::fogBottomThickness);
    MasterRenderer::entityShader->loadViewMatrix(camera);
    MasterRenderer::entityShader->loadIsRenderingTransparent(true);
    MasterRenderer::entityShader->loadIsRenderingDepth(false);
    MasterRenderer::entityShader->loadWaterColor(&Global::stageWaterColor);
    MasterRenderer::entityShader->loadWaterBlendAmount(waterBlendAmount);
    MasterRenderer::entityShader->connectTextureUnits();

    MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapTransparent);
    glBindTexture(GL_TEXTURE_2D, 0);
    MasterRenderer::entityShader->stop();

    if (!MasterRenderer::entitiesMapPass6.empty())
    {
        MasterRenderer::entityShader->start();
        MasterRenderer::entityShader->loadClipPlane(clipX, clipY, clipZ, clipW);
        MasterRenderer::entityShader->loadClipPlaneBehind(plane.x, plane.y, plane.z, plane.w);
        MasterRenderer::entityShader->loadSkyColor(fogRed, fogGreen, fogBlue);
        MasterRenderer::entityShader->loadSun(Global::gameLightSun);
        MasterRenderer::entityShader->loadFogGradient(SkyManager::getFogGradient());
        MasterRenderer::entityShader->loadFogDensity(SkyManager::getFogDensity());
        MasterRenderer::entityShader->loadFogBottomPosition(SkyManager::fogBottomPosition);
        MasterRenderer::entityShader->loadFogBottomThickness(SkyManager::fogBottomThickness);
        MasterRenderer::entityShader->loadViewMatrix(camera);
        MasterRenderer::entityShader->loadIsRenderingTransparent(false);
        MasterRenderer::entityShader->loadIsRenderingDepth(false);
        MasterRenderer::entityShader->loadWaterColor(&Global::stageWaterColor);
        MasterRenderer::entityShader->loadWaterBlendAmount(waterBlendAmount);
        MasterRenderer::entityShader->connectTextureUnits();

        MasterRenderer::entityRenderer->renderNEW(&MasterRenderer::entitiesMapPass6);

        MasterRenderer::entityShader->stop();
    }

    ANALYSIS_DONE("Master Render");
}

void MasterRenderer::processEntity(Entity* entity)
{
    if (!entity->visible)
    {
        return;
    }

    if (entity->renderOrderOverride <= 5)
    {
        std::unordered_map<TexturedModel*, std::vector<Entity*>>* mapToUse = nullptr;

        switch (entity->renderOrderOverride)
        {
            case 0: mapToUse = &MasterRenderer::entitiesMap;            break;
            case 1: mapToUse = &MasterRenderer::entitiesMapPass2;       break;
            case 2: mapToUse = &MasterRenderer::entitiesMapPass3;       break;
            case 3: mapToUse = &MasterRenderer::entitiesMapTransparent; break;
            case 4: mapToUse = &MasterRenderer::entitiesMapNoDepth;     break;
            case 5: mapToUse = &MasterRenderer::entitiesMapPass6;       break;
            default: break;
        }

        std::vector<TexturedModel*>* models = entity->getModels();

        const int numModels = (int)models->size();
        for (int i = 0; i < numModels; i++)
        {
            TexturedModel* model = models->at(i);

            auto location = mapToUse->find(model);
            if (location == mapToUse->end())
            {
                std::vector<Entity*> newEntityList;

                newEntityList.push_back(entity);

                mapToUse->insert({model, newEntityList}); //this might break iterators...
            }
            else
            {
                std::vector<Entity*>* entityList = &(location->second);

                entityList->push_back(entity);
            }

            //std::vector<Entity*>* list = &(*mapToUse)[entityModel];
            //list->push_back(entity);
        }
    }
    else
    {
        std::vector<TexturedModel*>* models = entity->getModels();

        const int numModels = (int)models->size();
        for (int i = 0; i < numModels; i++)
        {
            TexturedModel* model = models->at(i);

            std::unordered_map<TexturedModel*, std::vector<Entity*>>* mapToUse = nullptr;

            switch (model->renderOrder)
            {
                case 0: mapToUse = &MasterRenderer::entitiesMap;            break;
                case 1: mapToUse = &MasterRenderer::entitiesMapPass2;       break;
                case 2: mapToUse = &MasterRenderer::entitiesMapPass3;       break;
                case 3: mapToUse = &MasterRenderer::entitiesMapTransparent; break;
                case 4: mapToUse = &MasterRenderer::entitiesMapNoDepth;     break;
                case 5: mapToUse = &MasterRenderer::entitiesMapPass6;       break;
                default: break;
            }

            auto location = mapToUse->find(model);
            if (location == mapToUse->end())
            {
                std::vector<Entity*> newEntityList;

                newEntityList.push_back(entity);

                mapToUse->insert({model, newEntityList}); //this might break iterators...
            }
            else
            {
                std::vector<Entity*>* entityList = &(location->second);

                entityList->push_back(entity);
            }

            // todo this dereference might be suboptimal. would need to check to see if entity is in the list before adding.
            //std::vector<Entity*>* list = &(*mapToUse)[entityModel];
            //list->push_back(entity);
        }
    }
}

void MasterRenderer::clearAllEntities()
{
    // Clear the entities from the TexturedModel maps, but don't erase the map entries themselves.
    // This should make it so that there isn't new heap allocations happening every frame to re-create
    // the TexturedModel->std::vector entries, as well as having to rebuild the std::vector with more
    // allocations. std::vector.clear() should not actually reduce the capacity
    for (int i = 0; i < 6; i++)
    {
        std::unordered_map<TexturedModel*, std::vector<Entity*>>* mapToUse = nullptr;

        switch (i)
        {
            case 0: mapToUse = &MasterRenderer::entitiesMap;            break;
            case 1: mapToUse = &MasterRenderer::entitiesMapPass2;       break;
            case 2: mapToUse = &MasterRenderer::entitiesMapPass3;       break;
            case 3: mapToUse = &MasterRenderer::entitiesMapTransparent; break;
            case 4: mapToUse = &MasterRenderer::entitiesMapNoDepth;     break;
            case 5: mapToUse = &MasterRenderer::entitiesMapPass6;       break;
            default: break;
        }
        
        for (auto it = mapToUse->begin(); it != mapToUse->end(); it++)
        {
            std::vector<Entity*>* entities = &(it->second);
            entities->clear();
            entities->reserve(1000);
        }
    }

    //MasterRenderer::entitiesMap.clear();
    //MasterRenderer::entitiesMapPass2.clear();
    //MasterRenderer::entitiesMapPass3.clear();
    //MasterRenderer::entitiesMapTransparent.clear();
    //MasterRenderer::entitiesMapNoDepth.clear();
    //MasterRenderer::entitiesMapPass6.clear();
}

void MasterRenderer::addTexturedModels(std::vector<TexturedModel*>* models)
{
    std::vector<Entity*> emptyEntityList;

    const int numModels = (int)models->size();
    for (int i = 0; i < numModels; i++)
    {
        TexturedModel* model = models->at(i);

        MasterRenderer::entitiesMap            .insert({model, emptyEntityList});
        MasterRenderer::entitiesMapPass2       .insert({model, emptyEntityList});
        MasterRenderer::entitiesMapPass3       .insert({model, emptyEntityList});
        MasterRenderer::entitiesMapTransparent .insert({model, emptyEntityList});
        MasterRenderer::entitiesMapNoDepth     .insert({model, emptyEntityList});
        MasterRenderer::entitiesMapPass6       .insert({model, emptyEntityList});

        MasterRenderer::entitiesMap            [model].reserve(1000);
        MasterRenderer::entitiesMapPass2       [model].reserve(1000);
        MasterRenderer::entitiesMapPass3       [model].reserve(1000);
        MasterRenderer::entitiesMapTransparent [model].reserve(1000);
        MasterRenderer::entitiesMapNoDepth     [model].reserve(1000);
        MasterRenderer::entitiesMapPass6       [model].reserve(1000);
    }
}

void MasterRenderer::removeTexturedModels(std::vector<TexturedModel*>*)
{
    
}

void MasterRenderer::prepareNormalRender()
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (Global::renderWithCulling)
    {
        MasterRenderer::enableCulling();
    }
    else
    {
        MasterRenderer::disableCulling();
    }
}

void MasterRenderer::prepareTransparentRender()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //new
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(false);

    if (Global::renderWithCulling)
    {
        MasterRenderer::enableCulling();
    }
    else
    {
        MasterRenderer::disableCulling();
    }
}

void MasterRenderer::prepareRenderDepthOnly()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glEnable(GL_DEPTH_TEST);

    if (Global::renderWithCulling)
    {
        MasterRenderer::enableCulling();
    }
    else
    {
        MasterRenderer::disableCulling();
    }
}

void MasterRenderer::cleanUp()
{
    MasterRenderer::entityShader->cleanUp();
    delete MasterRenderer::entityShader;     INCR_DEL("ShaderProgram");
    delete MasterRenderer::entityRenderer;   INCR_DEL("EntityRenderer");
    delete MasterRenderer::projectionMatrix; INCR_DEL("Matrix4f");
}

void MasterRenderer::enableCulling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void MasterRenderer::disableCulling()
{
    glDisable(GL_CULL_FACE);
}

void MasterRenderer::makeProjectionMatrix()
{
    float aspectRatio = Display::ASPECT_RATIO;


    //FOV = 50;
    float y_scale = 1.0f/tanf(Maths::toRadians((MasterRenderer::VFOV_BASE + MasterRenderer::VFOV_ADDITION)/2.0f));
    float x_scale = y_scale/aspectRatio;


    //FOV = 88.88888;
    //float x_scale = (float)((1.0f / tan(toRadians(HFOV / 2.0f))));
    //float y_scale = x_scale * aspectRatio;


    float frustum_length = MasterRenderer::FAR_PLANE - MasterRenderer::NEAR_PLANE;

    MasterRenderer::projectionMatrix->m00 = x_scale;
    MasterRenderer::projectionMatrix->m11 = y_scale;
    MasterRenderer::projectionMatrix->m22 = -((MasterRenderer::FAR_PLANE + MasterRenderer::NEAR_PLANE)/frustum_length);
    MasterRenderer::projectionMatrix->m23 = -1;
    MasterRenderer::projectionMatrix->m32 = -((2*MasterRenderer::NEAR_PLANE*MasterRenderer::FAR_PLANE)/frustum_length);
    MasterRenderer::projectionMatrix->m33 = 0;

    MasterRenderer::entityRenderer->updateProjectionMatrix(MasterRenderer::projectionMatrix);

    if (Global::renderParticles)
    {
        ParticleMaster::updateProjectionMatrix(MasterRenderer::projectionMatrix);
    }

    if (Global::useHighQualityWater && Global::gameWaterRenderer != nullptr)
    {
        Global::gameWaterRenderer->updateProjectionMatrix(MasterRenderer::projectionMatrix);
    }
}

float MasterRenderer::getVFOV()
{
    return MasterRenderer::VFOV_BASE + MasterRenderer::VFOV_ADDITION;
}

