#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "guimanager.hpp"
#include "../engineTester/main.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/guinumber.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../menu/pausescreen.hpp"
#include "guirenderer.hpp"
#include "../toolbox/input.hpp"

#include <cmath>
#include <string>

std::list<GuiTexture*> GuiManager::guisToRender;

void GuiManager::init()
{
    GuiRenderer::init();
}

void GuiManager::refresh()
{
    //Render images
    GuiRenderer::render(&GuiManager::guisToRender);
}

void GuiManager::addGuiToRender(GuiTexture* newImage)
{
    GuiManager::guisToRender.push_back(newImage);
}

void GuiManager::removeGui(GuiTexture* imageToRemove)
{
    GuiManager::guisToRender.remove(imageToRemove);
}

void GuiManager::clearGuisToRender()
{
    GuiManager::guisToRender.clear();
}
