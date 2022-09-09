#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "configmenu.hpp"
#include "../engineTester/main.hpp"

#include <cmath>
#include <iostream>
#include <vector>

#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../loading/loadergl.hpp"
#include "../toolbox/input.hpp"
#include "../loading/levelloader.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../audio/audioplayer.hpp"
#include "../audio/source.hpp"
#include "../entities/camera.hpp"
#include "../guis/guiimageresources.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guiimage.hpp"
#include "button.hpp"
#include "mainmenu.hpp"
#include "../toolbox/maths.hpp"
#include "../renderEngine/display.hpp"
#include "../renderEngine/masterrenderer.hpp"

ConfigMenu::ConfigMenu()
{
    loadResources();
    setVisible(true);
}

ConfigMenu::~ConfigMenu()
{
    unloadResources();
}

void ConfigMenu::loadResources()
{
    if (buttonsNames.size() != 0)
    {
        printf("Warning: ConfigMenu loading resources when they are already loaded.\n");
        return;
    }

    textureParallelogram              = LoaderGL::loadTexture("res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop      = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramBackdrop.png");
    textureParallelogramHalf          = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalf.png");
    textureParallelogramHalfBackdrop  = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalfBackdrop.png");
    textureParallelogramHalf2         = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalf2.png");
    textureParallelogramHalf2Backdrop = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramHalf2Backdrop.png");

    float aspectRatio = Display::ASPECT_RATIO;

    buttonsNames.clear();

    //buttonsNames.push_back(new Button("Window Width",     Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    //buttonsNames.push_back(new Button("Window Height",    Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    //buttonsNames.push_back(new Button("Fullscreen",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("V-Sync",           Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("FPS Limit",        Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("FOV",              Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(2)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("SFX Volume",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(3)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("BGM Volume",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(4)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Show Particles",   Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(5)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Show FPS",         Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(6)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Speedometer",      Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(7)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsNames.push_back(new Button("Controller",       Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (0.1f*(8)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");

    buttonsValues.clear();
    //buttonsValues.push_back(new Button(std::to_string(SCR_WIDTH),                         Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    //buttonsValues.push_back(new Button(std::to_string(SCR_HEIGHT),                        Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    //buttonsValues.push_back(new Button(boolToString(Global::useFullscreen),               Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(!Global::framerateUnlock),            Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(std::to_string((int)Global::fpsLimit),             Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(floatToString(MasterRenderer::VFOV_BASE),          Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(2)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(floatToString(100*AudioPlayer::soundLevelSFX)+"%", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(3)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(floatToString(100*AudioPlayer::soundLevelBGM)+"%", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(4)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(Global::renderParticles),             Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(5)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(Global::displayFPS),                  Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(6)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(boolToString(Global::displaySpeedometer),          Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(7)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsValues.push_back(new Button(Input::getControllerName(),                        Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.67f, 0.5f + (0.1f*(8)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");

    buttonsNames[1]->generateText("FPS Limit", !Global::framerateUnlock);
    if (Global::fpsLimit < 0)
    {
        buttonsValues[1]->generateText("Unlimited", !Global::framerateUnlock);
    }
    else
    {
        buttonsValues[1]->generateText(std::to_string((int)Global::fpsLimit), !Global::framerateUnlock);
    }
}

std::string ConfigMenu::boolToString(bool value)
{
    if (value)
    {
        return "Yes";
    }

    return "No";
}

std::string ConfigMenu::floatToString(float value)
{
    char buf[128];
    snprintf(buf, 128, "%g", value);
    return buf;
}

void ConfigMenu::unloadResources()
{
    if ((int)buttonsNames.size() == 0)
    {
        printf("Warning: ConfigMenu unloading resources when they are empty.\n");
        return;
    }

    LoaderGL::deleteTexture(textureParallelogram);
    LoaderGL::deleteTexture(textureParallelogramBackdrop);
    LoaderGL::deleteTexture(textureParallelogramHalf);
    LoaderGL::deleteTexture(textureParallelogramHalfBackdrop);
    LoaderGL::deleteTexture(textureParallelogramHalf2);
    LoaderGL::deleteTexture(textureParallelogramHalf2Backdrop);
    textureParallelogram              = GL_NONE;
    textureParallelogramBackdrop      = GL_NONE;
    textureParallelogramHalf          = GL_NONE;
    textureParallelogramHalfBackdrop  = GL_NONE;
    textureParallelogramHalf2         = GL_NONE;
    textureParallelogramHalf2Backdrop = GL_NONE;

    for (int i = 0; i < (int)buttonsNames.size(); i++)
    {
        delete buttonsNames[i]; INCR_DEL("Button");
    }
    buttonsNames.clear();

    for (int i = 0; i < (int)buttonsValues.size(); i++)
    {
        delete buttonsValues[i]; INCR_DEL("Button");
    }
    buttonsValues.clear();

    currentButtonIndex = 0;
}

void ConfigMenu::draw()
{
    extern float dt;

    offsetTarget = -currentButtonIndex*0.1f;
    offsetCurr = Maths::approach(offsetCurr, offsetTarget, 15.0f, dt);
    if (std::abs(offsetTarget - offsetCurr) < 0.001f)
    {
        offsetCurr = offsetTarget;
    }
    
    if (visible)
    {
        for (int i = 0; i < (int)buttonsNames.size(); i++)
        {
            buttonsNames[i]->setPos(0.32f, 0.5f + offsetCurr + (0.1f*(i)));
            buttonsNames[i]->setVisible(true);
            buttonsNames[i]->setHighlight(false);

            buttonsValues[i]->setPos(0.67f, 0.5f + offsetCurr + (0.1f*(i)));
            buttonsValues[i]->setVisible(true);
            buttonsValues[i]->setHighlight(false);
        }

        buttonsNames[currentButtonIndex]->setHighlight(true);
        buttonsValues[currentButtonIndex]->setHighlight(true);
    }
}

void ConfigMenu::setVisible(bool visibleStatus)
{
    for (int i = 0; i < (int)buttonsNames.size(); i++)
    {
        buttonsNames[i]->setVisible(visibleStatus);
        buttonsNames[i]->setHighlight(visibleStatus);
        buttonsValues[i]->setVisible(visibleStatus);
        buttonsValues[i]->setHighlight(visibleStatus);
    }

    visible = visibleStatus;
}

Menu* ConfigMenu::step()
{
    Menu* retVal = nullptr;

    extern float dt;
    static float holdTick;

    //int holdMultiplier = 1;
    bool shouldGoDown = false;
    bool shouldGoUp = false;
    bool shouldGoLeft = false;
    bool shouldGoRight = false;
    bool pressedSelect = false;
    bool pressedBack = false;

    int moveY = (int)round(Input::inputs.INPUT_Y);
    int moveX = (int)round(Input::inputs.INPUT_X);

    setVisible(true);

    if ((Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || 
        (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START))
    {
        pressedSelect = true;
    }

    if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2)
    {
        pressedBack = true;
    }

    if (moveY != moveYPrevious)
    {
        if (moveY == 1)
        {
            shouldGoDown = true;
        }
        else if (moveY == -1)
        {
            shouldGoUp = true;
        }
    }

    moveYPrevious = moveY;

    if (moveY == 1)
    {
        holdDownTimer += dt;
    }
    else
    {
        holdDownTimer = 0.0f;
    }

    if (moveY == -1)
    {
        holdUpTimer += dt;
    }
    else
    {
        holdUpTimer = 0.0f;
    }

    if (holdDownTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.1f)
        {
            shouldGoDown = true;
            holdTick -= 0.1f;
        }
    }

    if (holdUpTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.1f)
        {
            shouldGoUp = true;
            holdTick -= 0.1f;
        }
    }

    if (moveX != moveXPrevious)
    {
        if (moveX == 1)
        {
            shouldGoRight = true;
        }
        else if (moveX == -1)
        {
            shouldGoLeft = true;
        }
    }

    moveXPrevious = moveX;

    if (moveX == 1)
    {
        holdRightTimer += dt;
    }
    else
    {
        holdRightTimer = 0.0f;
    }

    if (moveX == -1)
    {
        holdLeftTimer += dt;
    }
    else
    {
        holdLeftTimer = 0.0f;
    }

    if (holdRightTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.03f)
        {
            shouldGoRight = true;
            holdTick -= 0.03f;
            //holdMultiplier = 5;
        }
    }

    if (holdLeftTimer >= 0.25f)
    {
        holdTick += dt;
        if (holdTick >= 0.03f)
        {
            shouldGoLeft = true;
            holdTick -= 0.03f;
            //holdMultiplier = 5;
        }
    }

    if (shouldGoUp)
    {
        if (currentButtonIndex > 0)
        {
            currentButtonIndex--;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }
    if (shouldGoDown)
    {
        if (currentButtonIndex < (int)buttonsNames.size()-1)
        {
            currentButtonIndex++;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }

    //extern unsigned int SCR_WIdTH;
    //extern unsigned int SCR_HEIGHT;
    //extern float VFOV_BASE;

    if (shouldGoLeft)
    {
        switch (currentButtonIndex)
        {
            case 0:
                Global::framerateUnlock = !Global::framerateUnlock;
                buttonsValues[0]->generateText(boolToString(!Global::framerateUnlock));
                if (Global::fpsLimit < 0)
                {
                    buttonsValues[1]->generateText("Unlimited", !Global::framerateUnlock);
                }
                else
                {
                    buttonsValues[1]->generateText(std::to_string((int)Global::fpsLimit), !Global::framerateUnlock);
                }
                buttonsNames[1]->generateText("FPS Limit", !Global::framerateUnlock);
                break;

            case 1:
                if (Global::framerateUnlock)
                {
                    if (Global::fpsLimit < 0.0f)
                    {
                        Global::fpsLimit = 720.0f;
                    }
                    else
                    {
                        Global::fpsLimit -= 1.0f;
                        if (Global::fpsLimit < 30)
                        {
                            Global::fpsLimit = 30;
                        }
                    }

                    if (Global::fpsLimit < 0)
                    {
                        buttonsValues[1]->generateText("Unlimited", !Global::framerateUnlock);
                    }
                    else
                    {
                        buttonsValues[1]->generateText(std::to_string((int)Global::fpsLimit), !Global::framerateUnlock);
                    }
                }
                break;

            case 2:
                MasterRenderer::VFOV_BASE = MasterRenderer::VFOV_BASE - 1.0f;
                buttonsValues[2]->generateText(floatToString(MasterRenderer::VFOV_BASE));
                MasterRenderer::makeProjectionMatrix();
                break;

            case 3:
                if (AudioPlayer::soundLevelSFX > 0.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelSFX = std::fmaxf(0.0f, AudioPlayer::soundLevelSFX - 0.01f);
                AudioPlayer::soundLevelSFX = std::roundf(100*AudioPlayer::soundLevelSFX)/100.0f;
                buttonsValues[3]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelSFX))+"%");
                break;

            case 4:
                if (AudioPlayer::soundLevelBGM > 0.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelBGM = std::fmaxf(0.0f, AudioPlayer::soundLevelBGM - 0.01f);
                AudioPlayer::soundLevelBGM = std::roundf(100*AudioPlayer::soundLevelBGM)/100.0f;
                buttonsValues[4]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelBGM))+"%");
                break;

            case 5:
                Global::renderParticles = !Global::renderParticles;
                buttonsValues[5]->generateText(boolToString(Global::renderParticles));
                break;

            case 6:
                Global::displayFPS = !Global::displayFPS;
                buttonsValues[6]->generateText(boolToString(Global::displayFPS));
                break;

            case 7:
                Global::displaySpeedometer = !Global::displaySpeedometer;
                buttonsValues[7]->generateText(boolToString(Global::displaySpeedometer));
                break;

            case 8:
                if (Input::changeController(-1))
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                    buttonsValues[8]->generateText(Input::getControllerName());
                }
                break;

            default:
                break;
        }
    }

    if (shouldGoRight)
    {
        switch (currentButtonIndex)
        {
            case 0:
                Global::framerateUnlock = !Global::framerateUnlock;
                buttonsValues[0]->generateText(boolToString(!Global::framerateUnlock));
                if (Global::fpsLimit < 0)
                {
                    buttonsValues[1]->generateText("Unlimited", !Global::framerateUnlock);
                }
                else
                {
                    buttonsValues[1]->generateText(std::to_string((int)Global::fpsLimit), !Global::framerateUnlock);
                }
                buttonsNames[1]->generateText("FPS Limit", !Global::framerateUnlock);
                break;

            case 1:
                if (Global::framerateUnlock)
                {
                    if (Global::fpsLimit < 0.0f)
                    {
                        //do nothing, already unlimited
                    }
                    else
                    {
                        Global::fpsLimit += 1.0f;
                        if (Global::fpsLimit > 720)
                        {
                            Global::fpsLimit = -1.0f;
                        }
                    }

                    if (Global::fpsLimit < 0)
                    {
                        buttonsValues[1]->generateText("Unlimited", !Global::framerateUnlock);
                    }
                    else
                    {
                        buttonsValues[1]->generateText(std::to_string((int)Global::fpsLimit), !Global::framerateUnlock);
                    }
                }
                break;

            case 2:
                MasterRenderer::VFOV_BASE = MasterRenderer::VFOV_BASE + 1.0f;
                buttonsValues[2]->generateText(floatToString(MasterRenderer::VFOV_BASE));
                MasterRenderer::makeProjectionMatrix();
                break;

            case 3:
                if (AudioPlayer::soundLevelSFX < 1.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelSFX = std::fminf(1.0f, AudioPlayer::soundLevelSFX + 0.01f);
                AudioPlayer::soundLevelSFX = std::roundf(100*AudioPlayer::soundLevelSFX)/100.0f;
                buttonsValues[3]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelSFX))+"%");
                break;

            case 4:
                if (AudioPlayer::soundLevelBGM < 1.0f)
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                }
                AudioPlayer::soundLevelBGM = std::fminf(1.0f, AudioPlayer::soundLevelBGM + 0.01f);
                AudioPlayer::soundLevelBGM = std::roundf(100*AudioPlayer::soundLevelBGM)/100.0f;
                buttonsValues[4]->generateText(floatToString(std::roundf(100*AudioPlayer::soundLevelBGM))+"%");
                break;

            case 5:
                Global::renderParticles = !Global::renderParticles;
                buttonsValues[5]->generateText(boolToString(Global::renderParticles));
                break;

            case 6:
                Global::displayFPS = !Global::displayFPS;
                buttonsValues[6]->generateText(boolToString(Global::displayFPS));
                break;

            case 7:
                Global::displaySpeedometer = !Global::displaySpeedometer;
                buttonsValues[7]->generateText(boolToString(Global::displaySpeedometer));
                break;

            case 8:
                if (Input::changeController(1))
                {
                    AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
                    buttonsValues[8]->generateText(Input::getControllerName());
                }
                break;

            default:
                break;
        }
    }

    if (pressedSelect)
    {
        switch (currentButtonIndex)
        {
            case 0:
                Global::framerateUnlock = !Global::framerateUnlock;
                buttonsValues[0]->generateText(boolToString(!Global::framerateUnlock));
                buttonsValues[1]->generateText(std::to_string((int)Global::fpsLimit), !Global::framerateUnlock);
                buttonsNames[1]->generateText("FPS Limit", !Global::framerateUnlock);
                break;

            case 5:
                Global::renderParticles = !Global::renderParticles;
                buttonsValues[5]->generateText(boolToString(Global::renderParticles));
                break;

            case 6:
                Global::displayFPS = !Global::displayFPS;
                buttonsValues[6]->generateText(boolToString(Global::displayFPS));
                break;

            case 7:
                Global::displaySpeedometer = !Global::displaySpeedometer;
                buttonsValues[7]->generateText(boolToString(Global::displaySpeedometer));
                break;

            default:
                break;
        }
    }
    
    if (pressedBack)
    {
        AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
        setVisible(false);

        retVal = PopMenu::get();
        Global::menuConfig = nullptr;

        Global::menuMain->setVisible(true);
        Global::menuMain->draw();

        Global::saveConfigData();
    }
    else
    {
        draw();
    }

    return retVal;
}
