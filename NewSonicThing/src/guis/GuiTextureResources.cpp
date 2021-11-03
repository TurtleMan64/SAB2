#include "guitextureresources.hpp"
#include "guitexture.hpp"
#include "../loading/loadergl.hpp"
#include "../renderEngine/display.hpp"
#include "../engineTester/main.hpp"

GuiTexture* GuiTextureResources::textureRing        = nullptr;
GuiTexture* GuiTextureResources::textureLifeIcon    = nullptr;
GuiTexture* GuiTextureResources::textureLineBlue    = nullptr;
GuiTexture* GuiTextureResources::textureLineYellow  = nullptr;
GuiTexture* GuiTextureResources::textureLineRed     = nullptr;
GuiTexture* GuiTextureResources::textureRankA       = nullptr;
GuiTexture* GuiTextureResources::textureRankB       = nullptr;
GuiTexture* GuiTextureResources::textureRankC       = nullptr;
GuiTexture* GuiTextureResources::textureRankD       = nullptr;
GuiTexture* GuiTextureResources::textureRankE       = nullptr;

void GuiTextureResources::loadGuiTextures()
{
    float px = 1.0f/(Display::WINDOW_WIDTH);  //1 pixel in x dimension
    float py = 1.0f/(Display::WINDOW_HEIGHT); //1 pixel in y dimension

    const float w = 0.02f;   //width of a single text character
    const float o = 0.0008f; //horizontal offset to adjust for centered vs non centered

    INCR_NEW("GuiTexture") textureRing        = new GuiTexture(LoaderGL::loadTexture("res/Images/Ring_SA2.png"), o + 0.5f*w + 16*px, 0.0212f+48*py, 64*px, 64*py, 0);
    INCR_NEW("GuiTexture") textureLifeIcon    = new GuiTexture(LoaderGL::loadTexture("res/Images/LifeIcon.png"), 0.0f, 1.0f, 128*px, 128*py, 0);
    INCR_NEW("GuiTexture") textureLineBlue    = new GuiTexture(LoaderGL::loadTexture("res/Images/LineBlue.png"),   0.5f, 0.5f, 10*16*px, 20*128*py, 29.5f);
    INCR_NEW("GuiTexture") textureLineYellow  = new GuiTexture(LoaderGL::loadTexture("res/Images/LineYellow.png"), 0.5f, 0.5f, 10*16*px, 20*128*py, 29.5f);
    INCR_NEW("GuiTexture") textureLineRed     = new GuiTexture(LoaderGL::loadTexture("res/Images/LineRed.png"),    0.5f, 0.5f, 10*16*px, 20*128*py, 29.5f);
    INCR_NEW("GuiTexture") textureRankA       = new GuiTexture(LoaderGL::loadTexture("res/Images/MainMenu/RankA.png"), 0.5f, 0.8f, 128*px, 128*py, 0);
    INCR_NEW("GuiTexture") textureRankB       = new GuiTexture(LoaderGL::loadTexture("res/Images/MainMenu/RankB.png"), 0.5f, 0.8f, 128*px, 128*py, 0);
    INCR_NEW("GuiTexture") textureRankC       = new GuiTexture(LoaderGL::loadTexture("res/Images/MainMenu/RankC.png"), 0.5f, 0.8f, 128*px, 128*py, 0);
    INCR_NEW("GuiTexture") textureRankD       = new GuiTexture(LoaderGL::loadTexture("res/Images/MainMenu/RankD.png"), 0.5f, 0.8f, 128*px, 128*py, 0);
    INCR_NEW("GuiTexture") textureRankE       = new GuiTexture(LoaderGL::loadTexture("res/Images/MainMenu/RankE.png"), 0.5f, 0.8f, 128*px, 128*py, 0);
}
