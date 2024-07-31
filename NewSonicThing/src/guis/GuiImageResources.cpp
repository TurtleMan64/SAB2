#include "guiimageresources.hpp"
#include "guiimage.hpp"
#include "../loading/loadergl.hpp"
#include "../renderEngine/display.hpp"
#include "../engineTester/main.hpp"

GuiImage* GuiImageResources::textureRing        = nullptr;
GuiImage* GuiImageResources::textureLifeIcon    = nullptr;
GuiImage* GuiImageResources::textureLineBlue    = nullptr;
GuiImage* GuiImageResources::textureLineYellow  = nullptr;
GuiImage* GuiImageResources::textureLineRed     = nullptr;
GuiImage* GuiImageResources::textureRankA       = nullptr;
GuiImage* GuiImageResources::textureRankB       = nullptr;
GuiImage* GuiImageResources::textureRankC       = nullptr;
GuiImage* GuiImageResources::textureRankD       = nullptr;
GuiImage* GuiImageResources::textureRankE       = nullptr;

void GuiImageResources::loadGuiImages()
{
    float px = 1.0f/(Display::WINDOW_WIDTH);  //1 pixel in x dimension
    float py = 1.0f/(Display::WINDOW_HEIGHT); //1 pixel in y dimension

    float r = px/py; //resolution ratio

    const float w = 0.02f;   //width of a single text character
    const float o = 0.0008f; //horizontal offset to adjust for centered vs non centered

    INCR_NEW("GuiImage") textureRing        = new GuiImage(LoaderGL::loadTexture("res/Images/Ring_SA2.png"), o + 0.5f*w + 16*px, 0.0212f+48*py, 64*px, 64*py, 0);
    INCR_NEW("GuiImage") textureLifeIcon    = new GuiImage(LoaderGL::loadTexture("res/Images/LifeIcon.png"), 0.0f, 1.0f, 128*px, 128*py, 0);
    INCR_NEW("GuiImage") textureLineBlue    = new GuiImage(LoaderGL::loadTexture("res/Images/LineBlue.png"),   0.5f, 0.5f, 10*16*px, 20*128*py, 29.5f);
    INCR_NEW("GuiImage") textureLineYellow  = new GuiImage(LoaderGL::loadTexture("res/Images/LineYellow.png"), 0.5f, 0.5f, 10*16*px, 20*128*py, 29.5f);
    INCR_NEW("GuiImage") textureLineRed     = new GuiImage(LoaderGL::loadTexture("res/Images/LineRed.png"),    0.5f, 0.5f, 10*16*px, 20*128*py, 29.5f);
    INCR_NEW("GuiImage") textureRankA       = new GuiImage(LoaderGL::loadTexture("res/Images/MainMenu/RankA.png"), 0.5f, 0.8f, 0.15f*r, 0.15f, 0);
    INCR_NEW("GuiImage") textureRankB       = new GuiImage(LoaderGL::loadTexture("res/Images/MainMenu/RankB.png"), 0.5f, 0.8f, 0.15f*r, 0.15f, 0);
    INCR_NEW("GuiImage") textureRankC       = new GuiImage(LoaderGL::loadTexture("res/Images/MainMenu/RankC.png"), 0.5f, 0.8f, 0.15f*r, 0.15f, 0);
    INCR_NEW("GuiImage") textureRankD       = new GuiImage(LoaderGL::loadTexture("res/Images/MainMenu/RankD.png"), 0.5f, 0.8f, 0.15f*r, 0.15f, 0);
    INCR_NEW("GuiImage") textureRankE       = new GuiImage(LoaderGL::loadTexture("res/Images/MainMenu/RankE.png"), 0.5f, 0.8f, 0.15f*r, 0.15f, 0);
}
