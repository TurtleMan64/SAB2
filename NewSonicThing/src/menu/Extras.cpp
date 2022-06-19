#include "extras.hpp"
#include "mainmenu.hpp"
#include "../engineTester/main.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../toolbox/input.hpp"

Extras::Extras()
{
    selectInputPrevious = false;
    backInputPrevious = false;

    int hours = Global::gameTotalPlaytime/3600;
    int minutes = (Global::gameTotalPlaytime%3600)/60;
    int seconds = Global::gameTotalPlaytime%60;
    std::string playtime = "Total Playtime: "+std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(seconds);
    textTime = new GUIText(playtime, 0.2f, Global::fontVipnagorgialla, 0.0f, 0.45f, 1.0f, true, false, true); INCR_NEW("GUIText");
    timeBro  = new GUIText("bro...........................", 0.1f, Global::fontVipnagorgialla, 0.0f, 0.6f, 1.0f, true, false, true); INCR_NEW("GUIText");

    //GUIText::GUIText(std::string text, float fontSize, FontType* font, float x, float y, float maxLineLength,
    //bool centered, bool rightAligned, bool visible)
}

Extras::~Extras()
{
    textTime->deleteMe(); delete textTime; INCR_DEL("GUIText");
    timeBro->deleteMe(); delete timeBro; INCR_DEL("GUIText");
}

Menu* Extras::step()
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

    if ((pressedSelect && !selectInputPrevious) ||
        (pressedBack && !backInputPrevious))
    {
        textTime->visible = false;
        timeBro->visible = false;

        retVal = PopMenu::get();
        Global::menuExtras = nullptr;

        Global::menuMain->setVisible(true);
        Global::menuMain->draw();
    }
    else
    {
        //draw
        textTime->visible = true;
        timeBro->visible = true;
    }

    selectInputPrevious = pressedSelect;
    backInputPrevious = pressedBack;

    return retVal;
}
