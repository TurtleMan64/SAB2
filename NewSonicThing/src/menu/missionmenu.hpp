#ifndef MISSIONMENU_H
#define MISSIONMENU_H

class Button;
class GuiImage;

#include <glad/glad.h>
#include <vector>
#include "menu.hpp"

class MissionMenu : public Menu
{
private:
    GLuint textureParallelogram              = GL_NONE;
    GLuint textureParallelogramBackdrop      = GL_NONE;
    GLuint textureParallelogramHalf          = GL_NONE;
    GLuint textureParallelogramHalfBackdrop  = GL_NONE;
    GLuint textureParallelogramHalf2         = GL_NONE;
    GLuint textureParallelogramHalf2Backdrop = GL_NONE;
    GLuint textureRankA       = GL_NONE;
    GLuint textureRankB       = GL_NONE;
    GLuint textureRankC       = GL_NONE;
    GLuint textureRankD       = GL_NONE;
    GLuint textureRankE       = GL_NONE;
    GLuint textureBlank       = GL_NONE;
    GLuint textureSelect      = GL_NONE;
    GLuint textureNpcFound    = GL_NONE;
    GLuint textureNpcLost     = GL_NONE;
    GLuint textureEmerald0    = GL_NONE;
    GLuint textureEmerald1    = GL_NONE;
    GLuint textureEmerald2    = GL_NONE;
    GLuint textureEmerald3    = GL_NONE;
    GLuint textureEmerald4    = GL_NONE;
    GLuint textureEmerald5    = GL_NONE;
    GLuint textureEmerald6    = GL_NONE;
    GLuint textureEmeraldLost = GL_NONE;


    int moveXPrevious = 0;
    int moveYPrevious = 0;
    bool selectInputPrevious = false;
    bool backInputPrevious = false;

    float holdUpTimer = 0;
    float holdDownTimer = 0;

    int currButtonId = 0;
    std::vector<Button*> levelButtons;

    Button* missionButton = nullptr;
    GuiImage* rankM1 = nullptr;
    GuiImage* rankM2 = nullptr;
    GuiImage* rankM3 = nullptr;
    GuiImage* rankM4 = nullptr;
    GuiImage* missionSelect = nullptr;
    GuiImage* npc = nullptr;
    GuiImage* emerald = nullptr;

    Button* timeButton  = nullptr;
    Button* scoreButton = nullptr;
    Button* ringsButton = nullptr;

    bool visible = false;

    // Animation
    float offsetTarget = 0;
    float offsetCurr = 0;

public:
    MissionMenu();
    ~MissionMenu();
    void loadResources();
    void unloadResources();
    Menu* step();
    void draw();
    void setVisible(bool isVisible);
    
    float fontSize = 1.0f;
};

#endif
