#ifndef MISSIONMENU_H
#define MISSIONMENU_H

class Button;

#include <glad/glad.h>
#include "menu.h"

class MissionMenu : public Menu
{
private:
    int index;
    GLuint textureParallelogram;
    GLuint textureParallelogramBackdrop;
    GLuint textureParallelogramHalf;
    GLuint textureParallelogramHalfBackdrop;
    GLuint textureParallelogramHalf2;
    GLuint textureParallelogramHalf2Backdrop;

    int moveXPrevious;
    int moveYPrevious;
    bool selectInputPrevious;
    bool backInputPrevious;

    float holdUpTimer;
    float holdDownTimer;

    Button** levelButton;
    int counter;

    bool visible;

    // Animation
    //int oldSelection;
    //float animationTime;
    //bool animating;
    //int animationDirection;
    float offsetTarget;
    float offsetCurr;

private:
    void draw();
    void setVisible(bool visibleStatus);

public:
    MissionMenu();
    ~MissionMenu();
    void init();
    void loadResources();
    void unloadResources();
    Menu* step();
    
    float fontSize;
};

#endif
