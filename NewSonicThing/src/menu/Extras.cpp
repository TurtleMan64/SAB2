#include "extras.h"
#include "mainmenu.h"
#include "../engineTester/main.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../fontMeshCreator/guitext.h"
#include "../toolbox/input.h"


Extras::Extras()
{
    selectInputPrevious = false;
    backInputPrevious = false;

    textTime = new GUIText("Total Playtime:", 0.1f, Global::fontVipnagorgialla, 0.0f, 0.45f, 1.0f, true, false, false); INCR_NEW("GUIText");
    timeBro  = new GUIText("bro...........................", 0.1f, Global::fontVipnagorgialla, 0.0f, 0.6f, 1.0f, true, false, false); INCR_NEW("GUIText");
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

    textTime->visible = true;
    timeBro->visible = true;

    if ((pressedSelect && !selectInputPrevious) ||
        (pressedBack && !backInputPrevious))
    {
        retVal = PopMenu::get();
    }

    selectInputPrevious = pressedSelect;
    backInputPrevious = pressedBack;

    return retVal;
}
