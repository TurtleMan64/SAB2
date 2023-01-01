#include "guirenderer.hpp"
#include "guishader.hpp"
#include "guiimage.hpp"

#include <glad/glad.h>
#include "../engineTester/main.hpp"
#include "../loading/loadergl.hpp"
#include "../models/models.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/matrix.hpp"

#include <vector>

RawModel GuiRenderer::quadModel;
GuiShader* GuiRenderer::shader = nullptr;

void GuiRenderer::init()
{
    std::vector<float> positions;
    positions.push_back(-1);
    positions.push_back( 1);
    positions.push_back(-1);
    positions.push_back(-1);
    positions.push_back( 1);
    positions.push_back( 1);
    positions.push_back( 1);
    positions.push_back(-1);
    GuiRenderer::quadModel = LoaderGL::loadToVAO(&positions, 2);
    GuiRenderer::shader = new GuiShader("res/Shaders/guis/GuiVert.glsl", "res/Shaders/guis/GuiFrag.glsl"); INCR_NEW("GuiShader");
}

void GuiRenderer::render(std::vector<GuiImage*>* guis)
{
    GuiRenderer::shader->start();
    glBindVertexArray(GuiRenderer::quadModel.getVaoId());
    glEnableVertexAttribArray(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    std::unordered_set<GuiImage*> imagesAlreadyDrawn;

    const int numGuis = (int)guis->size();
    for (int i = 0; i < numGuis; i++)
    {
        GuiImage* gui = guis->at(i);
        if (gui->visible)
        {
            // make sure we dont render a guiimage twice in the same frame.
            if (imagesAlreadyDrawn.find(gui) == imagesAlreadyDrawn.end())
            {
                imagesAlreadyDrawn.insert(gui);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, gui->textureId);
                Matrix4f matrix;
                Maths::createTransformationMatrix(&matrix, gui->getPosition(), gui->rotation, gui->getSizeScaled());
                GuiRenderer::shader->loadTransformation(&matrix);
                GuiRenderer::shader->loadAlpha(gui->alpha);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, GuiRenderer::quadModel.getVertexCount());
            }
        }
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    GuiRenderer::shader->stop();
}

void GuiRenderer::cleanUp()
{
    GuiRenderer::shader->cleanUp();
    delete GuiRenderer::shader; INCR_DEL("GuiShader");
    GuiRenderer::shader = nullptr;
}
