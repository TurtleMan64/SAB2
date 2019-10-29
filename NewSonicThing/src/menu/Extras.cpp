#include "extras.h"
#include "mainmenu.h"
#include "../engineTester/main.h"
#include "../fontMeshCreator/fonttype.h"
#include "../renderEngine/renderEngine.h"
#include "../fontMeshCreator/guitext.h"
#include "../toolbox/input.h"


Extras::Extras()
{
    FontType * font = new FontType(Loader::loadTexture("res/Fonts/vipnagorgialla.png"),
        "res/Fonts/vipnagorgialla.fnt"); INCR_NEW("FontType");

    timeText = new GUIText("Total Playtime:", 0.1f, font, 0.0f, 0.45f, 1.0f, true, false, true); INCR_NEW("GUIText");
    jebaited = new GUIText("bro...........................", 0.01f, font, 0.0f, 0.6f, 1.0f, true, false, true); INCR_NEW("GUIText");
}

Extras::~Extras()
{
    this->timeText->deleteMe(); delete this->timeText; INCR_DEL("GUIText");
    this->jebaited->deleteMe(); delete this->jebaited; INCR_DEL("GUIText");
}

Menu* Extras::step()
{
    //todo
    //bool shouldGoUp = false;
    //bool shouldGoDown = false;
    //bool shouldGoLeft = false;
    //bool shouldGoRight = false;
    //bool pressedSelect = (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1) || (Input::inputs.INPUT_START && !Input::inputs.INPUT_PREVIOUS_START);
    //bool pressedBack = (Input::inputs.INPUT_ACTION2 && !Input::inputs.INPUT_PREVIOUS_ACTION2);
    //
    //int moveX = (int)round(Input::inputs.INPUT_X);
    //int moveY = (int)round(Input::inputs.INPUT_Y);
    //
    //if (pressedBack)
    //{
    //    
    //}

    return nullptr;
}