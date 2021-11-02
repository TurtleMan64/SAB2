#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resultsscreen.hpp"
#include "../engineTester/main.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../fontMeshCreator/guitext.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../toolbox/input.hpp"
#include "../loading/levelloader.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../audio/audioplayer.hpp"
#include "../audio/source.hpp"
#include "../entities/camera.hpp"
#include "../guis/guitextureresources.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guitexture.hpp"
#include "mainmenu.hpp"
#include "../toolbox/maths.hpp"
#include "../fontMeshCreator/guitext.hpp"

ResultsScreen::ResultsScreen()
{
    fontSize = 0.05f;
    selectInputPrevious = false;
    backInputPrevious = false;
    loadResources();
}

ResultsScreen::~ResultsScreen()
{
    unloadResources();
}

void ResultsScreen::loadResources()
{
    if (textArcadeModeClear != nullptr)
    {
        std::fprintf(stdout, "Warning: ResultsScreen loading resources when they are already loaded.\n");
    }

    textArcadeModeClear = new GUIText("Congrats bro", 0.2f, Global::fontVipnagorgialla, 0.5f, 0.5f, 4, false); INCR_NEW("GUIText")

    int totalTime = (int)Global::gameArcadePlaytime;
    int hours = totalTime/3600;
    int minutes = (totalTime%3600)/60;
    int seconds = totalTime%60;
    int decimal = (int)(100*fmodf(Global::gameArcadePlaytime, 1.0f));
    std::string playtime = "Clear time: "+std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(seconds)+"."+std::to_string(decimal);

    textClearTime = new GUIText(playtime, 0.2f, Global::fontVipnagorgialla, 0.5f, 0.25f, 4, false); INCR_NEW("GUIText")
}

void ResultsScreen::unloadResources()
{
    if (textArcadeModeClear == nullptr)
    {
        std::fprintf(stdout, "Warning: ResultsScreen unloading resources when they are empty.\n");
    }

    textArcadeModeClear->deleteMe(); delete textArcadeModeClear; textArcadeModeClear = nullptr; INCR_DEL("GUIText")
    textClearTime->deleteMe(); delete textClearTime; textClearTime = nullptr; INCR_DEL("GUIText")
}

Menu* ResultsScreen::step()
{
    Menu* retVal = nullptr;

    bool pressedSelect = false;
    bool pressedBack = false;

    if ((Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || 
        (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START))
    {
        pressedSelect = true;
    }

    if (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2)
    {
        pressedBack = true;
    }

    textArcadeModeClear->visible = true;
    textClearTime->visible = true;

    if ((pressedSelect && !selectInputPrevious) ||
        (pressedBack && !backInputPrevious))
    {
        retVal = PopMenu::get();
    }

    selectInputPrevious = pressedSelect;
    backInputPrevious = pressedBack;

    return retVal;
}
