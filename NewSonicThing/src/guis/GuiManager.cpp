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

std::list<GuiImage*> GuiManager::guisToRender;

void GuiManager::init()
{
    GuiRenderer::init();
}

void GuiManager::refresh()
{
    GuiRenderer::render(&GuiManager::guisToRender);
}

void GuiManager::addImageToRender(GuiImage* newImage)
{
    GuiManager::guisToRender.push_back(newImage);
}

void GuiManager::removeImageToRender(GuiImage* imageToRemove)
{
    GuiManager::guisToRender.remove(imageToRemove);
}

void GuiManager::clearGuisToRender()
{
    GuiManager::guisToRender.clear();
}
