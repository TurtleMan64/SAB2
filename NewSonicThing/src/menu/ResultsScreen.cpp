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
#include "../guis/guiimageresources.hpp"
#include "../guis/guimanager.hpp"
#include "../guis/guiimage.hpp"
#include "mainmenu.hpp"
#include "../toolbox/maths.hpp"
#include "../fontMeshCreator/guitext.hpp"

ResultsScreen::ResultsScreen()
{
    fontSize = 0.05f;
    selectInputPrevious = false;
    backInputPrevious = false;
    loadResources();

    if (Global::menuResults != nullptr)
    {
        printf("Warning: Results Menu should be null but is not.\n");
    }

    Global::menuResults = this;
}

ResultsScreen::~ResultsScreen()
{
    unloadResources();

    Global::menuResults = nullptr;
}

void ResultsScreen::loadResources()
{
    if (textArcadeModeClear != nullptr)
    {
        printf("Warning: ResultsScreen loading resources when they are already loaded.\n");
    }

    if (MenuManager::arcadeModeIsDoneBadEnding)
    {
        textArcadeModeClear  = new GUIText("The world is doomed...", 0.1f, Global::fontVipnagorgialla, 0.5f, 0.35f, 4, false); INCR_NEW("GUIText");
        textArcadeModeClear2 = new GUIText("Try collecting all the Chaos Emeralds next time!", 0.06f, Global::fontVipnagorgialla, 0.5f, 0.5f, 4, false); INCR_NEW("GUIText");
    }
    else
    {
        textArcadeModeClear  = new GUIText("The world is saved!", 0.1f, Global::fontVipnagorgialla, 0.5f, 0.35f, 4, false); INCR_NEW("GUIText");
        textArcadeModeClear2 = new GUIText("You collected all the Chaos Emeralds!", 0.06f, Global::fontVipnagorgialla, 0.5f, -0.5f, 4, false); INCR_NEW("GUIText");
    }

    int totalTimeSeconds = (int)Global::gameArcadePlaytime;
    int hours = totalTimeSeconds/3600;
    int minutes = (totalTimeSeconds%3600)/60;
    int seconds = totalTimeSeconds%60;
    std::string sMin = std::to_string(minutes);
    std::string sSec = std::to_string(seconds);
    if (sMin.length() == 1)
    {
        sMin = "0" + sMin;
    }
    if (sSec.length() == 1)
    {
        sSec = "0" + sSec;
    }

    std::string clearType = "True Ending Clear Time: ";
    if (MenuManager::arcadeModeIsDoneBadEnding)
    {
        clearType = "Arcade Mode Clear Time: ";
    }

    std::string playtimeText = clearType+std::to_string(hours)+":"+sMin+":"+sSec;
    if (hours == 0)
    {
        playtimeText = clearType+sMin+":"+sSec;
    }

    textClearTime = new GUIText(playtimeText, 0.085f, Global::fontVipnagorgialla, 0.5f, 0.65f, 4, false); INCR_NEW("GUIText");
}

void ResultsScreen::unloadResources()
{
    if (textArcadeModeClear == nullptr)
    {
        printf("Warning: ResultsScreen unloading resources when they are empty.\n");
    }

    textArcadeModeClear ->deleteMe(); delete textArcadeModeClear;  textArcadeModeClear  = nullptr; INCR_DEL("GUIText");
    textArcadeModeClear2->deleteMe(); delete textArcadeModeClear2; textArcadeModeClear2 = nullptr; INCR_DEL("GUIText");
    textClearTime->deleteMe(); delete textClearTime; textClearTime = nullptr; INCR_DEL("GUIText");
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

    if (pressedBack && !backInputPrevious)
    {
        retVal = PopMenu::get();
        Global::menuResults = nullptr;

        textArcadeModeClear->visible = false;
        textArcadeModeClear2->visible = false;
        textClearTime->visible = false;

        Global::menuMain->setVisible(true);
        Global::menuMain->draw();
    }
    else
    {
        textArcadeModeClear->visible = true;
        textArcadeModeClear2->visible = true;
        textClearTime->visible = true;
    }

    selectInputPrevious = pressedSelect;
    backInputPrevious = pressedBack;

    return retVal;
}
