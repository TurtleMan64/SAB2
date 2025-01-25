#ifndef DISPLAY_H
#define DISPLAY_H

//#include <GLFW/glfw3.h>
struct GLFWwindow;
struct GLFWmonitor;

class Display
{
private:
    static void loadDisplaySettings();
    static void loadGraphicsSettings();
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void windowCloseCallback(GLFWwindow* window);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

public:
    static GLFWwindow* window;
    static GLFWmonitor* monitor;

    static int WINDOW_WIDTH;
    static int WINDOW_HEIGHT;
    static float ASPECT_RATIO;

    static int F_WIDTH;
    static int F_HEIGHT;
    static int F_HZ;
    static int F_MONITOR_INDEX;

    static int AA_SAMPLES;

    static void centerWindow();
    static void moveWindow(int x, int y);
    static int createDisplay();

    static void updateDisplay();

    static void closeDisplay();

    static int displayWantsToClose();
};

#endif
