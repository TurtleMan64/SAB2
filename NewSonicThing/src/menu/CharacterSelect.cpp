#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "characterselect.hpp"
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
#include "missionmenu.hpp"
#include "../toolbox/maths.hpp"
#include "../renderEngine/display.hpp"

CharacterSelect::CharacterSelect()
{
    fontSize = 0.05f;
    loadResources();
    setVisible(true);

    switch (Global::currentCharacterType)
    {
        case Global::PlayableCharacter::Sonic:    currentButtonIndex = 0; break;
        case Global::PlayableCharacter::Tails:    currentButtonIndex = 1; break;
        case Global::PlayableCharacter::Knuckles: currentButtonIndex = 2; break;
        default: break;
    }

    offsetTarget = -currentButtonIndex * separation;
    offsetCurr = offsetTarget;
}

CharacterSelect::~CharacterSelect()
{
    unloadResources();
}

void CharacterSelect::loadResources()
{
    if (buttonsCharacterNames.size() != 0)
    {
        printf("Warning: CharacterSelect loading resources when they are already loaded.\n");
    }

    textureParallelogram         = LoaderGL::loadTexture("res/Images/MainMenu/Parallelogram.png");
    textureParallelogramBackdrop = LoaderGL::loadTexture("res/Images/MainMenu/ParallelogramBackdrop.png");
    textureSonic                 = LoaderGL::loadTextureNoInterpolation("res/Images/MainMenu/Characters/Sonic.png");
    textureTails                 = LoaderGL::loadTextureNoInterpolation("res/Images/MainMenu/Characters/Tails.png");
    textureKnuckles              = LoaderGL::loadTextureNoInterpolation("res/Images/MainMenu/Characters/Knuckles.png");

    float aspectRatio = Display::ASPECT_RATIO;

    buttonsCharacterNames.clear();
    buttonsCharacterNames.push_back(new Button("Sonic",    Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (separation*(0)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsCharacterNames.push_back(new Button("Tails",    Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (separation*(1)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");
    buttonsCharacterNames.push_back(new Button("Knuckles", Global::fontVipnagorgialla, textureParallelogram, textureParallelogramBackdrop, 0.32f, 0.5f + (separation*(2)), 0.56f / aspectRatio, 0.07f, true)); INCR_NEW("Button");

    characterIcons.clear();
    characterIcons.push_back(new GuiImage(textureSonic,    0.68f, 0.5f + (separation*(0)), separation/aspectRatio, separation, 0)); INCR_NEW("GuiImage");
    characterIcons.push_back(new GuiImage(textureTails,    0.68f, 0.5f + (separation*(1)), separation/aspectRatio, separation, 0)); INCR_NEW("GuiImage");
    characterIcons.push_back(new GuiImage(textureKnuckles, 0.68f, 0.5f + (separation*(2)), separation/aspectRatio, separation, 0)); INCR_NEW("GuiImage");
}

void CharacterSelect::unloadResources()
{
    if ((int)buttonsCharacterNames.size() == 0)
    {
        printf("Warning: CharacterSelect unloading resources when they are empty.\n");
    }

    LoaderGL::deleteTexture(textureParallelogram);
    LoaderGL::deleteTexture(textureParallelogramBackdrop);
    LoaderGL::deleteTexture(textureSonic);
    LoaderGL::deleteTexture(textureTails);
    LoaderGL::deleteTexture(textureKnuckles);
    textureParallelogram         = GL_NONE;
    textureParallelogramBackdrop = GL_NONE;
    textureSonic                 = GL_NONE;
    textureTails                 = GL_NONE;
    textureKnuckles              = GL_NONE;

    for (int i = 0; i < (int)buttonsCharacterNames.size(); i++)
    {
        delete buttonsCharacterNames[i]; INCR_DEL("Button");
    }
    buttonsCharacterNames.clear();

    for (int i = 0; i < (int)characterIcons.size(); i++)
    {
        delete characterIcons[i]; INCR_DEL("GuiImage");
    }
    characterIcons.clear();

    currentButtonIndex = 0;
}

void CharacterSelect::draw()
{
    extern float dt;

    offsetTarget = -currentButtonIndex*separation;
    offsetCurr = Maths::approach(offsetCurr, offsetTarget, 15.0f, dt);
    if (std::abs(offsetTarget - offsetCurr) < 0.001f)
    {
        offsetCurr = offsetTarget;
    }
    
    if (visible)
    {
        for (int i = 0; i < (int)buttonsCharacterNames.size(); i++)
        {
            buttonsCharacterNames[i]->setPos(0.32f, 0.5f + offsetCurr + (separation*(i)));
            buttonsCharacterNames[i]->setVisible(true);
            buttonsCharacterNames[i]->setHighlight(false);
        }

        buttonsCharacterNames[currentButtonIndex]->setHighlight(true);

        for (int i = 0; i < (int)characterIcons.size(); i++)
        {
            characterIcons[i]->setY(0.5f + offsetCurr + (separation*(i)));
            GuiManager::addImageToRender(characterIcons[i]);
        }
    }
}

void CharacterSelect::setVisible(bool visibleStatus)
{
    for (int i = 0; i < (int)buttonsCharacterNames.size(); i++)
    {
        buttonsCharacterNames[i]->setVisible(visibleStatus);
        buttonsCharacterNames[i]->setHighlight(visibleStatus);
    }

    visible = visibleStatus;
}

Menu* CharacterSelect::step()
{
    Menu* retVal = nullptr;

    extern float dt;
    static float holdTick;

    bool shouldGoDown = false;
    bool shouldGoUp = false;
    //bool shouldGoLeft = false;
    //bool shouldGoRight = false;
    bool pressedSelect = false;
    bool pressedBack = false;

    int moveY = (int)round(Input::inputs.INPUT_Y);
    //int moveX = (int)round(Input::inputs.INPUT_X);

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

    //if (moveX != moveXPrevious)
    //{
    //    if (moveX == 1)
    //    {
    //        shouldGoRight = true;
    //    }
    //    else if (moveX == -1)
    //    {
    //        shouldGoLeft = true;
    //    }
    //}

    //moveXPrevious = moveX;

    //if (moveX == 1)
    //{
    //    holdRightTimer += dt;
    //}
    //else
    //{
    //    holdRightTimer = 0.0f;
    //}
    //
    //if (moveX == -1)
    //{
    //    holdLeftTimer += dt;
    //}
    //else
    //{
    //    holdLeftTimer = 0.0f;
    //}

    //if (holdRightTimer >= 0.25f)
    //{
    //    holdTick += dt;
    //    if (holdTick >= 0.03f)
    //    {
    //        shouldGoRight = true;
    //        holdTick -= 0.03f;
    //    }
    //}
    //
    //if (holdLeftTimer >= 0.25f)
    //{
    //    holdTick += dt;
    //    if (holdTick >= 0.03f)
    //    {
    //        shouldGoLeft = true;
    //        holdTick -= 0.03f;
    //    }
    //}

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
        if (currentButtonIndex < (int)buttonsCharacterNames.size()-1)
        {
            currentButtonIndex++;
            AudioPlayer::play(36, Global::gameCamera->getFadePosition1());
        }
    }

    switch (currentButtonIndex)
    {
        case 0: Global::currentCharacterType = Global::PlayableCharacter::Sonic;    break;
        case 1: Global::currentCharacterType = Global::PlayableCharacter::Tails;    break;
        case 2: Global::currentCharacterType = Global::PlayableCharacter::Knuckles; break;
        default: break;
    }

    if (pressedSelect)
    {
        AudioPlayer::play(38, Global::gameCamera->getFadePosition1());
        setVisible(false);

        if (Global::gameIsArcadeMode)
        {
            Global::gameMissionNumber = 0;
            Global::gameArcadeIndex = 0;

            Global::levelId = (Global::gameArcadeLevelIds[Global::gameArcadeIndex]).first;
            Level* currentLevel = &Global::gameLevelData[Global::levelId];
            Global::levelName = currentLevel->fileName;
            Global::levelNameDisplay = currentLevel->displayName;
            Global::gameMissionDescription = (currentLevel->missionData[Global::gameMissionNumber])[(currentLevel->missionData[Global::gameMissionNumber]).size() - 1];
                
            Global::createTitleCard();

            Global::gameArcadePlaytime = 0.0f;

            Global::isNewLevel = true;
            Global::shouldLoadLevel = true;

            retVal = SwitchStack::get();
        }
        else
        {
            Global::menuMission = new MissionMenu; INCR_NEW("Menu");
            retVal = Global::menuMission;
            retVal->draw();

            offsetCurr = offsetTarget;
        }
    }
    else if (pressedBack)
    {
        AudioPlayer::play(37, Global::gameCamera->getFadePosition1());
        setVisible(false);

        retVal = PopMenu::get();
        Global::menuCharacterSelect = nullptr;

        Global::menuMain->setVisible(true);
        Global::menuMain->draw();
    }

    draw();

    return retVal;
}
