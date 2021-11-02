#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>

#include "display.hpp"
#include "masterrenderer.hpp"
#include "../toolbox/input.hpp"
#include "../engineTester/main.hpp"
#include "../toolbox/split.hpp"
#include "../toolbox/getline.hpp"

GLFWwindow* Display::window = nullptr;

// default settings
int Display::WINDOW_WIDTH = 1280;
int Display::WINDOW_HEIGHT = 720;
float Display::ASPECT_RATIO = 1.7777777777f;

int Display::F_WIDTH = 1280;
int Display::F_HEIGHT = 720;
int Display::F_HZ = 60;

int Display::AA_SAMPLES = 4;

extern float input_zoom_buffer;

int Display::createDisplay()
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

    Display::loadDisplaySettings();
    Display::loadGraphicsSettings();

    GLFWmonitor* monitor = nullptr;

    glfwWindowHint(GLFW_SAMPLES, Display::AA_SAMPLES);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (Global::useFullscreen)
    {
        monitor = glfwGetPrimaryMonitor();

        Display::WINDOW_WIDTH  = Display::F_WIDTH;
        Display::WINDOW_HEIGHT = Display::F_HEIGHT;

        glfwWindowHint(GLFW_REFRESH_RATE, Display::F_HZ);
    }

    int screenWidth  = Display::WINDOW_WIDTH;
    int screenHeight = Display::WINDOW_HEIGHT;


    //int count;
    //const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

    //for (int i = 0; i < count; i++)
    //{
        //std::fprintf(stdout, "%s\n", modes[i].);
    //}

    // glfw window creation
    // --------------------
    Display::window = glfwCreateWindow(screenWidth, screenHeight, "Sonic Adventure Blast 2", monitor, nullptr);
    if (Display::window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Display::window);
    glfwSetFramebufferSizeCallback(Display::window, Display::framebufferSizeCallback);
    glfwSetWindowCloseCallback(Display::window, Display::windowCloseCallback);

    GLFWimage icons[5];
    icons[0].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon16.png").c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
    icons[1].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon24.png").c_str(), &icons[1].width, &icons[1].height, 0, SOIL_LOAD_RGBA);
    icons[2].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon32.png").c_str(), &icons[2].width, &icons[2].height, 0, SOIL_LOAD_RGBA);
    icons[3].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon48.png").c_str(), &icons[3].width, &icons[3].height, 0, SOIL_LOAD_RGBA);
    icons[4].pixels = SOIL_load_image((Global::pathToEXE+"res/Images/Icon64.png").c_str(), &icons[4].width, &icons[4].height, 0, SOIL_LOAD_RGBA);
    glfwSetWindowIcon(Display::window, 5, icons);
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

    std::fprintf(stdout, "Video renderer: %s\n", glGetString(GL_RENDERER));
    std::fprintf(stdout, "Video vendor:   %s\n", glGetString(GL_VENDOR));
    std::fprintf(stdout, "OpenGL version: %s\n", glGetString(GL_VERSION));
    std::fprintf(stdout, "GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::fprintf(stdout, "GLFW version:   %s\n", glfwGetVersionString());

    glfwSwapInterval(1); //1 = vsync. 0 = off. 2 = half monitor refresh rate
    glEnable(GL_MULTISAMPLE);

    //Center the window
    if (!Global::useFullscreen)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        int monitorWidth = mode->width;
        int monitorHeight = mode->height;

        if (Display::WINDOW_WIDTH  <= monitorWidth && 
            Display::WINDOW_HEIGHT <= monitorHeight)
        {
            int xpos = monitorWidth/2  - (Display::WINDOW_WIDTH)/2;
            int ypos = monitorHeight/2 - (Display::WINDOW_HEIGHT)/2;

            glfwSetWindowPos(Display::window, xpos, ypos);
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

    Display::ASPECT_RATIO = ((float)Display::WINDOW_WIDTH)/Display::WINDOW_HEIGHT;

    return 0;
}

void Display::updateDisplay()
{
    ANALYSIS_START("Glfw Swap Buffers");
    glfwSwapBuffers(Display::window);
    ANALYSIS_DONE("Glfw Swap Buffers");
}

void Display::closeDisplay()
{
    glfwDestroyWindow(Display::window);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

int Display::displayWantsToClose()
{
    return glfwWindowShouldClose(Display::window);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Display::framebufferSizeCallback(GLFWwindow* /*windowHandle*/, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    Display::WINDOW_WIDTH = width;
    Display::WINDOW_HEIGHT = height;
    //Global::displaySizeChanged = 2; too hard, memory leaks
    MasterRenderer::makeProjectionMatrix();
}

void Display::windowCloseCallback(GLFWwindow* /*windowHandle*/)
{
    Global::gameState = STATE_EXITING;
}

void Display::loadDisplaySettings()
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
                    Display::WINDOW_WIDTH = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Height") == 0)
                {
                    Display::WINDOW_HEIGHT = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Fullscreen") == 0)
                {
                    Global::useFullscreen = (strcmp(lineSplit[1], "on") == 0);
                }
                else if (strcmp(lineSplit[0], "F_Width") == 0)
                {
                    Display::F_WIDTH = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_Height") == 0)
                {
                    Display::F_HEIGHT = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "F_RefreshRate") == 0)
                {
                    Display::F_HZ = std::stoi(lineSplit[1], nullptr, 10);
                }
            }
            free(lineSplit);
        }
        file.close();
    }
}

void Display::loadGraphicsSettings()
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
                    MasterRenderer::VFOV_BASE = std::stof(lineSplit[1], nullptr);
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
                    if (Global::fpsLimit < 0)
                    {
                        Global::fpsLimit = -1;
                    }
                    else if (Global::fpsLimit < 30)
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
                    Display::AA_SAMPLES = std::stoi(lineSplit[1], nullptr, 10);
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
