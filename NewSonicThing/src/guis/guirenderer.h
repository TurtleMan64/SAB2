#ifndef GUIRENDERER_H
#define GUIRENDERER_H

class GuiTexture;
class GuiShader;

#include <list>
#include "../models/models.h"

class GuiRenderer
{
private:
	static RawModel quadModel;
	static GuiShader* shader;

public:
	static void init();
	
	static void render(std::list<GuiTexture*>* guis);
	
	static void cleanUp();
};

#endif