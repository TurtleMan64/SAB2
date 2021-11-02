#ifndef GUITEXTURERESOURCES_H
#define GUITEXTURERESOURCES_H

class GuiTexture;

class GuiTextureResources
{
public:
    static GuiTexture* textureRing;
    static GuiTexture* textureLifeIcon;
    static GuiTexture* textureLineBlue;
    static GuiTexture* textureLineYellow;
    static GuiTexture* textureLineRed;
    static GuiTexture* textureRankA;
    static GuiTexture* textureRankB;
    static GuiTexture* textureRankC;
    static GuiTexture* textureRankD;
    static GuiTexture* textureRankE;

    static void loadGuiTextures();
};
#endif
