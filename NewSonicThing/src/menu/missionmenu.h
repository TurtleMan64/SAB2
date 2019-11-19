#ifndef MISSIONMENU_H
#define MISSIONMENU_H

class Button;
class GuiTexture;

#include <glad/glad.h>
#include "menu.h"

class MissionMenu : public Menu
{
private:
    int currLevel;
    GLuint textureParallelogram;
    GLuint textureParallelogramBackdrop;
    GLuint textureParallelogramHalf;
    GLuint textureParallelogramHalfBackdrop;
    GLuint textureParallelogramHalf2;
    GLuint textureParallelogramHalf2Backdrop;
    GLuint textureRankA;
    GLuint textureRankB;
    GLuint textureRankC;
    GLuint textureRankD;
    GLuint textureRankE;
    GLuint textureBlank;
    GLuint textureSelect;

    int moveXPrevious;
    int moveYPrevious;
    bool selectInputPrevious;
    bool backInputPrevious;

    float holdUpTimer;
    float holdDownTimer;

    Button** levelButton;
    int numButtons;

    Button* missionButton = nullptr;
    GuiTexture* rankM1 = nullptr;
    GuiTexture* rankM2 = nullptr;
    GuiTexture* rankM3 = nullptr;
    GuiTexture* rankM4 = nullptr;
    GuiTexture* missionSelect = nullptr;

    Button*  timeButton = nullptr;
    Button*  scoreButton = nullptr;

    bool visible;

    // Animation
    float offsetTarget;
    float offsetCurr;

private:
    void draw(bool updateMissionText);
    void setVisible(bool visibleStatus);

public:
    MissionMenu();
    ~MissionMenu();
    void loadResources();
    void unloadResources();
    Menu* step();
    
    float fontSize;
};

#endif
