#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>

#include "renderEngine.h"
#include "../toolbox/input.h"
#include "../engineTester/main.h"
#include "../toolbox/split.h"
#include "../toolbox/getline.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_close_callback(GLFWwindow* window);

// default settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

unsigned int F_WIDTH = 1280;
unsigned int F_HEIGHT = 720;
unsigned int F_BITSRED = 8;
unsigned int F_BITSGREEN = 8;
unsigned int F_BITSBLUE = 8;
unsigned int F_HZ = 60;

unsigned int AA_SAMPLES = 4;

extern float VFOV_BASE;

extern float input_zoom_buffer;

GLFWwindow* window = nullptr;

void loadDisplaySettings();
void loadGraphicsSettings();

int createDisplay()
{
    // glfw: initialize and configure
    // ------------------------------
    if (glfwInit() == GL_FALSE)
    {
        std::fprintf(stdout, "Error: GLFW could not be initialized (glfwInit() failed).\n");
        return -1;
    }
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Request an OpenGL 4.0 core context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    loadDisplaySettings();
    loadGraphicsSettings();

    GLFWmonitor* monitor = nullptr;

    glfwWindowHint(GLFW_SAMPLES, AA_SAMPLES);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (Global::useFullscreen)
    {
        monitor = glfwGetPrimaryMonitor();

        SCR_WIDTH = F_WIDTH;
        SCR_HEIGHT = F_HEIGHT;
    }

    unsigned int screenWidth  = SCR_WIDTH;
    unsigned int screenHeight = SCR_HEIGHT;


    //int count;
    //const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

    //for (int i = 0; i < count; i++)
    //{
        //std::fprintf(stdout, "%s\n", modes[i].);
    //}

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(screenWidth, screenHeight, "Sonic Adventure Blast 2", monitor, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowCloseCallback(window, window_close_callback);

    GLFWimage icons[5];
    icons[0].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon16.png").c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
    icons[1].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon24.png").c_str(), &icons[1].width, &icons[1].height, 0, SOIL_LOAD_RGBA);
    icons[2].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon32.png").c_str(), &icons[2].width, &icons[2].height, 0, SOIL_LOAD_RGBA);
    icons[3].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon48.png").c_str(), &icons[3].width, &icons[3].height, 0, SOIL_LOAD_RGBA);
    icons[4].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon64.png").c_str(), &icons[4].width, &icons[4].height, 0, SOIL_LOAD_RGBA);
    glfwSetWindowIcon(window, 5, icons);
    SOIL_free_image_data(icons[0].pixels);
    SOIL_free_image_data(icons[1].pixels);
    SOIL_free_image_data(icons[2].pixels);
    SOIL_free_image_data(icons[3].pixels);
    SOIL_free_image_data(icons[4].pixels);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::fprintf(stdout, "Renderer:       %s\n", glGetString(GL_RENDERER));
    std::fprintf(stdout, "Vendor:         %s\n", glGetString(GL_VENDOR));
    std::fprintf(stdout, "OpenGL version: %s\n", glGetString(GL_VERSION));
    std::fprintf(stdout, "GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::fprintf(stdout, "GLFW Version: %s\n",   glfwGetVersionString());

    glfwSwapInterval(1); //1 = vsync. 0 = off. 2 = half monitor refresh rate
    glEnable(GL_MULTISAMPLE);

    //Center the window
    if (!Global::useFullscreen)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        int monitorWidth = mode->width;
        int monitorHeight = mode->height;

        if ((int)SCR_WIDTH  <= monitorWidth && 
            (int)SCR_HEIGHT <= monitorHeight)
        {
            int xpos = monitorWidth/2  - ((int)SCR_WIDTH)/2;
            int ypos = monitorHeight/2 - ((int)SCR_HEIGHT)/2;

            glfwSetWindowPos(window, xpos, ypos);
        }
    }

    //glfwGetWindowAttrib(window, GLFW_SAMPLES);
    //std::fprintf(stdout, "samples:   %d\n", glfwGetWindowAttrib(window, GLFW_SAMPLES));

    //float aniso = 0.0f;
    //glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &aniso);
    //std::fprintf(stdout, "max lod bias:   %f\n", aniso);

    //To check what extensions are avalible 
    //int ext_cnt;
    //glGetIntegerv(GL_NUM_EXTENSIONS, &ext_cnt);
    //for (int i = 0; i < ext_cnt; i++)
    {
        //std::fprintf(stdout, "extensions:   %s\n", glGetStringi(GL_EXTENSIONS, i));
    }

    //Master_makeProjectionMatrix();

    return 0;
}

void updateDisplay()
{
    // glfw: swap buffers
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
}

void closeDisplay()
{
    glfwDestroyWindow(window);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

int displayWantsToClose()
{
    return glfwWindowShouldClose(window);
}

GLFWwindow* getWindow()
{
    return window;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* /*windowHandle*/, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    //Global::displaySizeChanged = 2; too hard, memory leaks
    Master_makeProjectionMatrix();
}

void window_close_callback(GLFWwindow* /*windowHandle*/)
{
    Global::gameState = STATE_EXITING;
}

void loadDisplaySettings()
{
    std::ifstream file(Global::pathToEXE + "Settings/DisplaySettings.ini");
    if (!file.is_open())
    {
        std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "Settings/DisplaySettings.ini").c_str());
        file.close();
    }
    else
    {
        std::string line;

        while (!file.eof())
        {
            getlineSafe(file, line);

            char lineBuf[512];
            memcpy(lineBuf, line.c_str(), line.size()+1);

            int splitLength = 0;
            char** lineSplit = split(lineBuf, ' ', &splitLength);

            if (splitLength == 2)
            {
                if (strcmp(lineSplit[0], "Width") == 0)
                {
                    SCR_WIDTH = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Height") == 0)
                {
                    SCR_HEIGHT = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Fullscreen") == 0)
                {
                    Global::useFullscreen = (strcmp(lineSplit[1], "on") == 0);
                }
                else if (strcmp(lineSplit[0], "F_Width") == 0)
                {
                    F_WIDTH = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_Height") == 0)
                {
                    F_HEIGHT = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_Bits_Red") == 0)
                {
                    F_BITSRED = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_Bits_Green") == 0)
                {
                    F_BITSGREEN = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_Bits_Blue") == 0)
                {
                    F_BITSBLUE = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_RefreshRate") == 0)
                {
                    F_HZ = std::stoi(lineSplit[1], nullptr, 10);
                }
            }
            free(lineSplit);
        }
        file.close();
    }
}

void loadGraphicsSettings()
{
    std::ifstream file(Global::pathToEXE + "Settings/GraphicsSettings.ini");
    if (!file.is_open())
    {
        std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "Settings/GraphicsSettings.ini").c_str());
        file.close();
    }
    else
    {
        std::string line;

        while (!file.eof())
        {
            getlineSafe(file, line);

            char lineBuf[512];
            memcpy(lineBuf, line.c_str(), line.size()+1);

            int splitLength = 0;
            char** lineSplit = split(lineBuf, ' ', &splitLength);

            if (splitLength == 2)
            {
                if (strcmp(lineSplit[0], "HQ_Water") == 0)
                {
                    if (strcmp(lineSplit[1], "on") == 0)
                    {
                        Global::useHighQualityWater = true;
                    }
                    else
                    {
                        Global::useHighQualityWater = false;
                    }
                }
                else if (strcmp(lineSplit[0], "HQ_Water_Reflection_Width") == 0)
                {
                    Global::HQWaterReflectionWidth = (unsigned)std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "HQ_Water_Reflection_Height") == 0)
                {
                    Global::HQWaterReflectionHeight = (unsigned)std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "HQ_Water_Refraction_Width") == 0)
                {
                    Global::HQWaterRefractionWidth = (unsigned)std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "HQ_Water_Refraction_Height") == 0)
                {
                    Global::HQWaterRefractionHeight = (unsigned)std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "FOV") == 0)
                {
                    VFOV_BASE = std::stof(lineSplit[1], nullptr);
                }
                else if (strcmp(lineSplit[0], "Unlock_Framerate") == 0)
                {
                    if (strcmp(lineSplit[1], "on") == 0)
                    {
                        Global::framerateUnlock = true;
                    }
                    else
                    {
                        Global::framerateUnlock = false;
                    }
                }
                else if (strcmp(lineSplit[0], "FPS_Limit") == 0)
                {
                    Global::fpsLimit = (float)((int)std::stof(lineSplit[1], nullptr));
                    if (Global::fpsLimit < 30)
                    {
                        Global::fpsLimit = 30;
                    }
                    else if (Global::fpsLimit > 720)
                    {
                        Global::fpsLimit = 720;
                    }
                }
                else if (strcmp(lineSplit[0], "Anti-Aliasing_Samples") == 0)
                {
                    AA_SAMPLES = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Render_Particles") == 0)
                {
                    if (strcmp(lineSplit[1], "on") == 0)
                    {
                        Global::renderParticles = true;
                    }
                    else
                    {
                        Global::renderParticles = false;
                    }
                }
                else if (strcmp(lineSplit[0], "Render_Bloom") == 0)
                {
                    if (strcmp(lineSplit[1], "on") == 0)
                    {
                        Global::renderBloom = true;
                    }
                    else
                    {
                        Global::renderBloom = false;
                    }
                }
                else if (strcmp(lineSplit[0], "Render_Shadows_Far") == 0)
                {
                    if (strcmp(lineSplit[1], "on") == 0)
                    {
                        Global::renderShadowsFar = true;
                    }
                    else
                    {
                        Global::renderShadowsFar = false;
                    }
                }
                else if (strcmp(lineSplit[0], "Render_Shadows_Close") == 0)
                {
                    if (strcmp(lineSplit[1], "on") == 0)
                    {
                        Global::renderShadowsClose = true;
                    }
                    else
                    {
                        Global::renderShadowsClose = false;
                    }
                }
                else if (strcmp(lineSplit[0], "Shadows_Far_Quality") == 0)
                {
                    Global::shadowsFarQuality = std::stoi(lineSplit[1], nullptr, 10);
                }
            }
            free(lineSplit);
        }
        file.close();
    }
}
