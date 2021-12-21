#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <SDL2/SDL.h>

struct InputStruct
{
    volatile int uniqueVar;

    volatile float INPUT_X;
    volatile float INPUT_Y;
    volatile float INPUT_X2;
    volatile float INPUT_Y2;
    volatile float INPUT_R2;
    volatile float INPUT_L2;

    volatile bool INPUT_ACTION1; //A button
    volatile bool INPUT_ACTION2; //B button
    volatile bool INPUT_ACTION3; //X button
    volatile bool INPUT_ACTION4; //Y button
    volatile bool INPUT_RB;
    volatile bool INPUT_LB;
    volatile bool INPUT_SELECT;
    volatile bool INPUT_START;
    volatile bool INPUT_TAB;
    volatile bool INPUT_DPADU;
    volatile bool INPUT_DPADD;

    volatile float INPUT_PREVIOUS_X;
    volatile float INPUT_PREVIOUS_Y;
    volatile float INPUT_PREVIOUS_X2;
    volatile float INPUT_PREVIOUS_Y2;
    volatile float INPUT_PREVIOUS_R2;
    volatile float INPUT_PREVIOUS_L2;

    volatile bool INPUT_PREVIOUS_ACTION1;
    volatile bool INPUT_PREVIOUS_ACTION2;
    volatile bool INPUT_PREVIOUS_ACTION3;
    volatile bool INPUT_PREVIOUS_ACTION4;
    volatile bool INPUT_PREVIOUS_RB;
    volatile bool INPUT_PREVIOUS_LB;
    volatile bool INPUT_PREVIOUS_START;
    volatile bool INPUT_PREVIOUS_TAB;

    volatile int MENU_X;
    volatile int MENU_Y;

    volatile int approxXLeft;
    volatile int approxXLeftPrevious;
    volatile int approxYLeft;
    volatile int approxYLeftPrevious;
};

class Input
{
public:
    static SDL_GameController* controller;

    static double mousePreviousX;
    static double mousePreviousY;


    // Settings

    static bool freeMouse;

    static float mouseSensitivityX;
    static float mouseSensitivityY;

    static float stickSensitivityX;
    static float stickSensitivityY;

    static float triggerSensitivity;

    static int CONTROLLER_ID; //-1 = no controller. otherwise, controller id

    static int BUTTON_IDX_A;
    static int BUTTON_IDX_X;
    static int BUTTON_IDX_B;
    static int BUTTON_IDX_Y;
    static int BUTTON_IDX_LB;
    static int BUTTON_IDX_RB;
    static int BUTTON_IDX_SELECT;
    static int BUTTON_IDX_START;
    static int BUTTON_IDX_DPADU;
    static int BUTTON_IDX_DPADD;

    static int   STICK_LX;
    static float STICK_LX_SCALE;
    static int   STICK_LY;
    static float STICK_LY_SCALE;
    static int   STICK_RX;
    static float STICK_RX_SCALE;
    static int   STICK_RY;
    static float STICK_RY_SCALE;

    static float STICK_LXDEADZONE;
    static float STICK_LYDEADZONE;
    static float STICK_RXDEADZONE;
    static float STICK_RYDEADZONE;

    static int   TRIGGER_L;
    static float LT_NEUTRAL;
    static float LT_MAX;
    static float LT_RANGE;
    static int   TRIGGER_R;
    static float RT_NEUTRAL;
    static float RT_MAX;
    static float RT_RANGE;

    static float TRIGGER_DEADZONE;

public:
    static InputStruct inputs;

    static void init();

    static void pollInputs();

    static std::string getControllerName();

    static bool changeController(int direction);

    static void cleanUp();
};

#endif
