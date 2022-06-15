#ifndef GUIRENDERER_H
#define GUIRENDERER_H

class GuiImage;
class GuiShader;

#include <list>
#include "../models/models.hpp"

class GuiRenderer
{
private:
    static RawModel quadModel;
    static GuiShader* shader;

public:
    static void init();
    
    static void render(std::list<GuiImage*>* guis);
    
    static void cleanUp();
};

#endif