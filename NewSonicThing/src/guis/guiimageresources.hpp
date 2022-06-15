#ifndef GUITEXTURERESOURCES_H
#define GUITEXTURERESOURCES_H

class GuiImage;

class GuiImageResources
{
public:
    static GuiImage* textureRing;
    static GuiImage* textureLifeIcon;
    static GuiImage* textureLineBlue;
    static GuiImage* textureLineYellow;
    static GuiImage* textureLineRed;
    static GuiImage* textureRankA;
    static GuiImage* textureRankB;
    static GuiImage* textureRankC;
    static GuiImage* textureRankD;
    static GuiImage* textureRankE;

    static void loadGuiImages();
};
#endif
