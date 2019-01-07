#ifndef GUITEXTURERESOURCES_H
#define GUITEXTURERESOURCES_H

#include "../renderEngine/renderEngine.h"

class GuiTexture;

class GuiTextureResources
{
public:
	static GuiTexture* textureRing;
	static GuiTexture* textureBlueLine;
	static GuiTexture* textureRankDisplay;

	static void loadGuiTextures();
};
#endif