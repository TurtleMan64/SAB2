#ifndef INPUT_H
#define INPUT_H

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
    static InputStruct inputs;

    static void init();

    static void pollInputs();
};

#endif
