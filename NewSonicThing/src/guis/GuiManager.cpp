#include "guimanager.hpp"
#include "guiimage.hpp"
#include "guirenderer.hpp"

#include <vector>

std::vector<GuiImage*> GuiManager::guisToRender;

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
    for (int i = 0; i < (int)guisToRender.size(); i++)
    {
        if (guisToRender.at(i) == imageToRemove)
        {
            guisToRender.erase(guisToRender.begin() + i);
            //i--;
            break;
        }
    }
}

void GuiManager::clearGuisToRender()
{
    GuiManager::guisToRender.clear();
}
