#ifndef DISPLAY_H
#define DISPLAY_H

#include <GLFW/glfw3.h>

class Display
{
public:
    static GLFWwindow* window;

    static int createDisplay();

    static void updateDisplay();

    static void closeDisplay();

    static int displayWantsToClose();
};

#endif
