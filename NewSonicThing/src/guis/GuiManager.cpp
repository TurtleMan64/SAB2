#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "guimanager.h"
#include "../engineTester/main.h"
#include "../fontMeshCreator/guitext.h"
#include "../fontMeshCreator/guinumber.h"
#include "../fontMeshCreator/fonttype.h"
#include "../menu/pausescreen.h"
#include "guirenderer.h"
#include "../toolbox/input.h"

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
