#include <glad/glad.h>

#if defined(_WIN32) || defined(__APPLE__)
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#else
#include "/usr/include/GLFW/glfw3.h"
#endif

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>

#include "input.hpp"
#include "../renderEngine/skymanager.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../entities/controllableplayer.hpp"
#include "../entities/TwinkleCircuit/tckart.hpp"
#include "maths.hpp"
#include "../toolbox/split.hpp"
#include "../toolbox/getline.hpp"
#include <random>
#include <chrono>

#include "../renderEngine/display.hpp"

#ifdef DEV_MODE
#include <iostream>
#include <fstream>
#include "../collision/collisionchecker.hpp"
#endif

SDL_GameController* Input::controller = nullptr;

InputStruct Input::inputs{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

double Input::mousePreviousX = 0.0;
double Input::mousePreviousY = 0.0;

// Settings

bool Input::freeMouse = true;

float Input::mouseSensitivityX = 0.25f;
float Input::mouseSensitivityY = 0.25f;

float Input::stickSensitivityX = 2.5f;
float Input::stickSensitivityY = 2.5f;

float Input::triggerSensitivity = 2.0f;

int Input::CONTROLLER_ID = 0;

int Input::BUTTON_IDX_A      =  0;
int Input::BUTTON_IDX_X      =  1;
int Input::BUTTON_IDX_B      =  2;
int Input::BUTTON_IDX_Y      =  3;
int Input::BUTTON_IDX_LB     =  9;
int Input::BUTTON_IDX_RB     = 10;
int Input::BUTTON_IDX_SELECT =  4;
int Input::BUTTON_IDX_START  =  6;
int Input::BUTTON_IDX_DPADU  = 11;
int Input::BUTTON_IDX_DPADD  = 12;

int   Input::STICK_LX       = 0;
float Input::STICK_LX_SCALE = 1.05f;
int   Input::STICK_LY       = 1;
float Input::STICK_LY_SCALE = 1.05f;
int   Input::STICK_RX       = 2;
float Input::STICK_RX_SCALE = 1.05f;
int   Input::STICK_RY       = 3;
float Input::STICK_RY_SCALE = 1.05f;

float Input::STICK_LXDEADZONE = 0.1f;
float Input::STICK_LYDEADZONE = 0.1f;
float Input::STICK_RXDEADZONE = 0.1f;
float Input::STICK_RYDEADZONE = 0.1f;

int   Input::TRIGGER_L  =  4;
float Input::LT_NEUTRAL = -1.0f;
float Input::LT_MAX     =  1.0f;
float Input::LT_RANGE   =  2.0f;
int   Input::TRIGGER_R  =  5;
float Input::RT_NEUTRAL = -1.0f;
float Input::RT_MAX     =  1.0f;
float Input::RT_RANGE   =  2.0f;

float Input::TRIGGER_DEADZONE = 0.3f;

void Input::pollInputs()
{
    glfwPollEvents();

    Input::inputs.INPUT_PREVIOUS_ACTION1 = Input::inputs.INPUT_ACTION1;
    Input::inputs.INPUT_PREVIOUS_ACTION2 = Input::inputs.INPUT_ACTION2;
    Input::inputs.INPUT_PREVIOUS_ACTION3 = Input::inputs.INPUT_ACTION3;
    Input::inputs.INPUT_PREVIOUS_ACTION4 = Input::inputs.INPUT_ACTION4;
    Input::inputs.INPUT_PREVIOUS_LB      = Input::inputs.INPUT_LB;
    Input::inputs.INPUT_PREVIOUS_RB      = Input::inputs.INPUT_RB;
    Input::inputs.INPUT_PREVIOUS_START   = Input::inputs.INPUT_START;
    Input::inputs.INPUT_PREVIOUS_ESC     = Input::inputs.INPUT_ESC;

    Input::inputs.INPUT_PREVIOUS_X  = Input::inputs.INPUT_X;
    Input::inputs.INPUT_PREVIOUS_Y  = Input::inputs.INPUT_Y;
    Input::inputs.INPUT_PREVIOUS_X2 = Input::inputs.INPUT_X2;
    Input::inputs.INPUT_PREVIOUS_Y2 = Input::inputs.INPUT_Y2;
    Input::inputs.INPUT_PREVIOUS_R2 = Input::inputs.INPUT_L2;
    Input::inputs.INPUT_PREVIOUS_L2 = Input::inputs.INPUT_R2;

    Input::inputs.INPUT_ACTION1 = false;
    Input::inputs.INPUT_ACTION2 = false;
    Input::inputs.INPUT_ACTION3 = false;
    Input::inputs.INPUT_ACTION4 = false;
    Input::inputs.INPUT_LB      = false;
    Input::inputs.INPUT_RB      = false;
    Input::inputs.INPUT_SELECT  = false;
    Input::inputs.INPUT_START   = false;
    Input::inputs.INPUT_ESC     = false;

    Input::inputs.INPUT_X  = 0;
    Input::inputs.INPUT_Y  = 0;
    Input::inputs.INPUT_X2 = 0;
    Input::inputs.INPUT_Y2 = 0;
    Input::inputs.INPUT_L2 = 0;
    Input::inputs.INPUT_R2 = 0;

    bool joystickIsPresent = false;

    if (Input::controller != nullptr)
    {
        joystickIsPresent = true;

        SDL_GameControllerUpdate();

        Input::inputs.INPUT_ACTION1 = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_A);
        Input::inputs.INPUT_ACTION2 = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_X);
        Input::inputs.INPUT_ACTION3 = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_B);
        Input::inputs.INPUT_ACTION4 = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_Y);

        Input::inputs.INPUT_LB      = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_LB);
        Input::inputs.INPUT_RB      = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_RB);
        Input::inputs.INPUT_SELECT  = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_SELECT);
        Input::inputs.INPUT_START   = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_START);

        Input::inputs.INPUT_DPADU   = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_DPADU);
        Input::inputs.INPUT_DPADD   = (bool)SDL_GameControllerGetButton(Input::controller, (SDL_GameControllerButton)BUTTON_IDX_DPADD);

        int leftX    = (int)SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)STICK_LX);
        int leftY    = (int)SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)STICK_LY);
        int rightX   = (int)SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)STICK_RX);
        int rightY   = (int)SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)STICK_RY);
        int triggerL = (int)SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)TRIGGER_L);
        int triggerR = (int)SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)TRIGGER_R);

        //positive range is from 0 to 32767. lets make it 32768
        if (leftX    > 0) { leftX    += 1; }
        if (leftY    > 0) { leftY    += 1; }
        if (rightX   > 0) { rightX   += 1; }
        if (rightY   > 0) { rightY   += 1; }
        if (triggerL > 0) { triggerL += 1; }
        if (triggerR > 0) { triggerR += 1; }

        Input::inputs.INPUT_X  = leftX   /32768.0f;
        Input::inputs.INPUT_Y  = leftY   /32768.0f;
        Input::inputs.INPUT_X2 = rightX  /32768.0f;
        Input::inputs.INPUT_Y2 = rightY  /32768.0f;
        Input::inputs.INPUT_L2 = 2*(triggerL/32768.0f) - 1.0f;
        Input::inputs.INPUT_R2 = 2*(triggerR/32768.0f) - 1.0f;
    }

    if (joystickIsPresent) //apply scaling and deadzones
    {
        if (abs(Input::inputs.INPUT_X)  < STICK_LXDEADZONE)
        { 
            Input::inputs.INPUT_X  = 0;
        }
        else
        {
            const float RANGE = 1.0f - STICK_LXDEADZONE;
            if (Input::inputs.INPUT_X >= 0)
            {
                Input::inputs.INPUT_X = (Input::inputs.INPUT_X - STICK_LXDEADZONE)/RANGE;
            }
            else
            {
                Input::inputs.INPUT_X = (Input::inputs.INPUT_X + STICK_LXDEADZONE)/RANGE;
            }
        }

        if (abs(Input::inputs.INPUT_Y)  < STICK_LYDEADZONE)
        {
            Input::inputs.INPUT_Y  = 0;
        }
        else
        {
            const float RANGE = 1.0f - STICK_LYDEADZONE;
            if (Input::inputs.INPUT_Y >= 0)
            {
                Input::inputs.INPUT_Y = (Input::inputs.INPUT_Y - STICK_LYDEADZONE)/RANGE;
            }
            else
            {
                Input::inputs.INPUT_Y = (Input::inputs.INPUT_Y + STICK_LYDEADZONE)/RANGE;
            }
        }

        if (abs(Input::inputs.INPUT_X2) < STICK_RXDEADZONE)
        {
            Input::inputs.INPUT_X2 = 0;
        }
        else
        {
            const float RANGE = 1.0f - STICK_RXDEADZONE;
            if (Input::inputs.INPUT_X2 >= 0)
            {
                Input::inputs.INPUT_X2 = (Input::inputs.INPUT_X2 - STICK_RXDEADZONE)/RANGE;
            }
            else
            {
                Input::inputs.INPUT_X2 = (Input::inputs.INPUT_X2 + STICK_RXDEADZONE)/RANGE;
            }
        }

        if (abs(Input::inputs.INPUT_Y2) < STICK_RYDEADZONE)
        {
            Input::inputs.INPUT_Y2 = 0;
        }
        else
        {
            const float RANGE = 1.0f - STICK_RYDEADZONE;
            if (Input::inputs.INPUT_Y2 >= 0)
            {
                Input::inputs.INPUT_Y2 = (Input::inputs.INPUT_Y2 - STICK_RYDEADZONE)/RANGE;
            }
            else
            {
                Input::inputs.INPUT_Y2 = (Input::inputs.INPUT_Y2 + STICK_RYDEADZONE)/RANGE;
            }
        }

        Input::inputs.INPUT_X  *= STICK_LX_SCALE;
        Input::inputs.INPUT_Y  *= STICK_LY_SCALE;
        Input::inputs.INPUT_X2 *= STICK_RX_SCALE;
        Input::inputs.INPUT_Y2 *= STICK_RY_SCALE;

        Input::inputs.INPUT_X2 *= stickSensitivityX;
        Input::inputs.INPUT_Y2 *= stickSensitivityY;


        float triggerLValue = 0;
        float triggerRValue = 0;


        float rawValue = (Input::inputs.INPUT_L2 - LT_NEUTRAL) / LT_RANGE;
        if (rawValue >= TRIGGER_DEADZONE) { triggerLValue = rawValue; }

        rawValue = (Input::inputs.INPUT_R2 - RT_NEUTRAL) / RT_RANGE;
        if (rawValue >= TRIGGER_DEADZONE) { triggerRValue = rawValue; }


        Input::inputs.INPUT_X2 += triggerSensitivity * (triggerLValue - triggerRValue);

        Input::inputs.INPUT_L2 = triggerLValue;
        Input::inputs.INPUT_R2 = triggerRValue;
    }

    double xpos, ypos;
    glfwGetCursorPos(Display::window, &xpos, &ypos);

    if (freeMouse == false)
    {
        float mouseDiffX = (float)(mouseSensitivityX*(xpos - mousePreviousX));
        float mouseDiffY = (float)(mouseSensitivityY*(ypos - mousePreviousY));

        extern float dt;
        float fpsFactor = 1/(dt*60);

        Input::inputs.INPUT_X2 += mouseDiffX*fpsFactor;
        Input::inputs.INPUT_Y2 += mouseDiffY*fpsFactor;
    }
    mousePreviousX = xpos;
    mousePreviousY = ypos;

    //if (glfwGetKey(Display::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    //{
    //    glfwSetWindowShouldClose(Display::window, true);
    //}

    if (glfwGetKey(Display::window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Input::inputs.INPUT_ACTION1 = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Input::inputs.INPUT_ACTION2 = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        Input::inputs.INPUT_ACTION3 = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_F) == GLFW_PRESS)
    {
        Input::inputs.INPUT_ACTION4 = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        Input::inputs.INPUT_START = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        Input::inputs.INPUT_LB = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_E) == GLFW_PRESS)
    {
        Input::inputs.INPUT_RB = true;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        Input::inputs.INPUT_ESC = true;
    }

    if (glfwGetKey(Display::window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Input::inputs.INPUT_Y = -1;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Input::inputs.INPUT_Y = 1;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Input::inputs.INPUT_X = -1;
    }
    if (glfwGetKey(Display::window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Input::inputs.INPUT_X = 1;
    }

    #ifdef DEV_MODE
    if (glfwGetKey(Display::window, GLFW_KEY_H) == GLFW_PRESS)
    {
        //CollisionChecker::debug = true;
    }
    #endif


    if (glfwGetKey(Display::window, GLFW_KEY_0) == GLFW_PRESS)
    {
        SkyManager::increaseTimeOfDay(0.5f);
    }
    if (glfwGetKey(Display::window, GLFW_KEY_9) == GLFW_PRESS)
    {
        SkyManager::increaseTimeOfDay(-0.5f);
    }

    #ifdef DEV_MODE
    if (glfwGetKey(Display::window, GLFW_KEY_J) == GLFW_PRESS)
    {
        if (Global::gameMainPlayer != nullptr)
        {
            extern float dt;
            //Global::gameMainVehicle->getPosition()->y = Global::gameMainVehicle->getY()+600*dt;
            Vector3f* v = &Global::gameMainPlayer->vel;
            Global::gameMainPlayer->vel.set(v->x, v->y+600*dt, v->z);
        }
    }
    #endif


    #ifdef DEV_MODE
    if (Input::inputs.INPUT_LB && !Input::inputs.INPUT_PREVIOUS_LB)
    {
        if (Global::gameMainPlayer != nullptr)
        {
            float yrot = 0;
            float zrot = 0;
            Maths::sphereAnglesFromPosition(&Global::gameMainPlayer->vel, &yrot, &zrot);
            //std::fprintf(stdout, "Time of day: %f\n", SkyManager::getTimeOfDay());
            std::fprintf(stdout, "position = %f %f %f\n", Global::gameMainPlayer->position.x, Global::gameMainPlayer->position.y, Global::gameMainPlayer->position.z);
            //std::fprintf(stdout, "%f %f %f\n", Global::gameMainPlayer->position.x, Global::gameMainPlayer->position.y, Global::gameMainPlayer->position.z);
            std::fprintf(stdout, "velocity = %f %f %f\n", Global::gameMainPlayer->vel.x, Global::gameMainPlayer->vel.y, Global::gameMainPlayer->vel.z);
            std::fprintf(stdout, "normal   = %f %f %f\n", Global::gameMainPlayer->relativeUp.x, Global::gameMainPlayer->relativeUp.y, Global::gameMainPlayer->relativeUp.z);
            std::fprintf(stdout, "yrot     = %f\n", yrot);
            //std::fprintf(stdout, "cam yaw = %f   cam pitch: %f\n", Global::gameCamera->getYaw(), Global::gameCamera->getPitch());
            std::fprintf(stdout, "cam pos  = %f %f %f\n", Global::gameCamera->eye.x, Global::gameCamera->eye.y, Global::gameCamera->eye.z);
            std::fprintf(stdout, "cam dir  = %f %f %f\n", Global::gameMainPlayer->camDir.x, Global::gameMainPlayer->camDir.y, Global::gameMainPlayer->camDir.z);
            std::fprintf(stdout, "\n");

            //std::fprintf(stdout, "106 1  %f %f %f  %f %f %f   %f %f %f\n", 
            //    Global::gameKart->position.x, Global::gameKart->position.y, Global::gameKart->position.z,
            //    Global::gameKart->vel.x, Global::gameKart->vel.y, Global::gameKart->vel.z, 
            //    Global::gameKart->currNorm.x, Global::gameKart->currNorm.y, Global::gameKart->currNorm.z);
        }

        //std::fprintf(stdout, "diff = %d\n", Global::countNew-Global::countDelete);
        //extern std::unordered_map<std::string, int> heapObjects;
        //std::unordered_map<std::string, int>::iterator it = heapObjects.begin();
        //while (it != heapObjects.end())
        {
            //std::fprintf(stdout, "'%s' count: %d\n", it->first.c_str(), it->second);
            //it++;
        }

        //Loader::printInfo();

        //if (Global::raceLogSize > 0)
        {
            //std::ofstream raceLogFile;
            //raceLogFile.open("RaceLog.txt", std::ios::out | std::ios::trunc);
            //for (int i = 0; i < Global::raceLogSize; i++)
            //{
            //    std::string line = Global::raceLog[i].toString();
            //    raceLogFile << line << "\n";
            //}
            //raceLogFile.close();
            //Global::raceLogSize = 0;
        }
    }
    if (Input::inputs.INPUT_RB && !Input::inputs.INPUT_PREVIOUS_RB)
    {
        Global::shouldLogRace = !Global::shouldLogRace;
    }
    #endif


    if (Input::inputs.INPUT_ESC && !Input::inputs.INPUT_PREVIOUS_ESC)
    {
        if (!Input::freeMouse)
        {
            Input::freeMouse = true;
            glfwSetInputMode(Display::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }


    float mag = sqrtf(Input::inputs.INPUT_X*Input::inputs.INPUT_X + Input::inputs.INPUT_Y*Input::inputs.INPUT_Y);
    if (mag > 1)
    {
        Input::inputs.INPUT_X = Input::inputs.INPUT_X / mag;
        Input::inputs.INPUT_Y = Input::inputs.INPUT_Y / mag;
    }

    Input::inputs.approxXLeftPrevious = Input::inputs.approxXLeft;
    Input::inputs.approxXLeft = (int)round(Input::inputs.INPUT_X);
    Input::inputs.approxYLeftPrevious = Input::inputs.approxYLeft;
    Input::inputs.approxYLeft = (int)round(Input::inputs.INPUT_Y);

    if (Input::inputs.approxXLeft != 0)
    {
        Input::inputs.MENU_X = Input::inputs.approxXLeft - Input::inputs.approxXLeftPrevious;
    }

    if (Input::inputs.approxYLeft != 0)
    {
        Input::inputs.MENU_Y = Input::inputs.approxYLeft - Input::inputs.approxYLeftPrevious;
    }
}

void Input::init()
{
    Input::inputs.uniqueVar = 1149650285; //Value that is very easy to find with a memory scan

    SDL_SetHintWithPriority(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "0", SDL_HINT_OVERRIDE);
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    SDL_version linked;
    SDL_GetVersion(&linked);
    printf("SDL version:    %d.%d.%d\n", linked.major, linked.minor, linked.patch);

    //load sensitivity and button mappings from external file

    std::ifstream file(Global::pathToEXE + "Settings/CameraSensitivity.ini");
    if (!file.is_open())
    {
        std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "Settings/CameraSensitivity.ini").c_str());
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
                if (strcmp(lineSplit[0], "Mouse_X") == 0)
                {
                    mouseSensitivityX = std::stof(lineSplit[1], nullptr);
                }
                else if (strcmp(lineSplit[0], "Mouse_Y") == 0)
                {
                    mouseSensitivityY = std::stof(lineSplit[1], nullptr);
                }
                else if (strcmp(lineSplit[0], "Stick_X") == 0)
                {
                    stickSensitivityX = std::stof(lineSplit[1], nullptr);
                }
                else if (strcmp(lineSplit[0], "Stick_Y") == 0)
                {
                    stickSensitivityY = std::stof(lineSplit[1], nullptr);
                }
                else if (strcmp(lineSplit[0], "Triggers") == 0)
                {
                    triggerSensitivity = std::stof(lineSplit[1], nullptr);
                }
            }

            free(lineSplit);
        }
        file.close();
    }


    std::ifstream file2(Global::pathToEXE + "Settings/ControllerConfig.ini");
    if (!file2.is_open())
    {
        std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "Settings/ControllerConfig.ini").c_str());
        file2.close();
    }
    else
    {
        std::string line;

        while (!file2.eof())
        {
            getlineSafe(file2, line);

            char lineBuf[512];
            memcpy(lineBuf, line.c_str(), line.size()+1);

            int splitLength = 0;
            char** lineSplit = split(lineBuf, ' ', &splitLength);

            if (splitLength == 2)
            {
                if (strcmp(lineSplit[0], "A") == 0)
                {
                    BUTTON_IDX_A = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "B") == 0)
                {
                    BUTTON_IDX_B = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "X") == 0)
                {
                    BUTTON_IDX_X = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Y") == 0)
                {
                    BUTTON_IDX_Y = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "LB") == 0)
                {
                    BUTTON_IDX_LB = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "RB") == 0)
                {
                    BUTTON_IDX_RB = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Select") == 0)
                {
                    BUTTON_IDX_SELECT = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Start") == 0)
                {
                    BUTTON_IDX_START = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "DpadU") == 0)
                {
                    BUTTON_IDX_DPADU = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "DpadD") == 0)
                {
                    BUTTON_IDX_DPADD = std::stoi(lineSplit[1], nullptr, 10);
                }
                else if (strcmp(lineSplit[0], "Controller_Id") == 0)
                {
                    CONTROLLER_ID = std::stoi(lineSplit[1], nullptr, 10);
                }
            }
            else if (splitLength == 4)
            {
                if (strcmp(lineSplit[0], "Stick_LX") == 0)
                {
                    STICK_LX = std::stoi(lineSplit[1], nullptr, 10);
                    STICK_LXDEADZONE = std::stof(lineSplit[2], nullptr);
                    STICK_LX_SCALE = std::stof(lineSplit[3], nullptr);
                }
                else if (strcmp(lineSplit[0], "Stick_LY") == 0)
                {
                    STICK_LY = std::stoi(lineSplit[1], nullptr, 10);
                    STICK_LYDEADZONE = std::stof(lineSplit[2], nullptr);
                    STICK_LY_SCALE = std::stof(lineSplit[3], nullptr);
                }
                else if (strcmp(lineSplit[0], "Stick_RX") == 0)
                {
                    STICK_RX = std::stoi(lineSplit[1], nullptr, 10);
                    STICK_RXDEADZONE = std::stof(lineSplit[2], nullptr);
                    STICK_RX_SCALE = std::stof(lineSplit[3], nullptr);
                }
                else if (strcmp(lineSplit[0], "Stick_RY") == 0)
                {
                    STICK_RY = std::stoi(lineSplit[1], nullptr, 10);
                    STICK_RYDEADZONE = std::stof(lineSplit[2], nullptr);
                    STICK_RY_SCALE = std::stof(lineSplit[3], nullptr);
                }
            }
            else if (splitLength == 5)
            {
                if (strcmp(lineSplit[0], "Trigger_L") == 0)
                {
                    TRIGGER_L = std::stoi(lineSplit[1], nullptr, 10);
                    LT_NEUTRAL = std::stof(lineSplit[2], nullptr);
                    LT_MAX = std::stof(lineSplit[3], nullptr);
                    TRIGGER_DEADZONE = std::stof(lineSplit[4], nullptr);
                    LT_RANGE = LT_MAX - LT_NEUTRAL;
                }
                else if (strcmp(lineSplit[0], "Trigger_R") == 0)
                {
                    TRIGGER_R = std::stoi(lineSplit[1], nullptr, 10);
                    RT_NEUTRAL = std::stof(lineSplit[2], nullptr);
                    RT_MAX = std::stof(lineSplit[3], nullptr);
                    TRIGGER_DEADZONE = std::stof(lineSplit[4], nullptr);
                    RT_RANGE = RT_MAX - RT_NEUTRAL;
                }
            }

            free(lineSplit);
        }
        file2.close();
    }

    glfwPollEvents();

    //make sure no index goes out of bounds
    if (Input::controller != nullptr)
    {
        STICK_LX  = std::min(STICK_LX,  SDL_CONTROLLER_AXIS_MAX - 1);
        STICK_LY  = std::min(STICK_LY,  SDL_CONTROLLER_AXIS_MAX - 1);
        STICK_RX  = std::min(STICK_RX,  SDL_CONTROLLER_AXIS_MAX - 1);
        STICK_RY  = std::min(STICK_RY,  SDL_CONTROLLER_AXIS_MAX - 1);
        TRIGGER_L = std::min(TRIGGER_L, SDL_CONTROLLER_AXIS_MAX - 1);
        TRIGGER_R = std::min(TRIGGER_R, SDL_CONTROLLER_AXIS_MAX - 1);

        BUTTON_IDX_A      = std::min(BUTTON_IDX_A,      SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_B      = std::min(BUTTON_IDX_B,      SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_X      = std::min(BUTTON_IDX_X,      SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_Y      = std::min(BUTTON_IDX_Y,      SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_LB     = std::min(BUTTON_IDX_LB,     SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_RB     = std::min(BUTTON_IDX_RB,     SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_SELECT = std::min(BUTTON_IDX_SELECT, SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_START  = std::min(BUTTON_IDX_START,  SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_DPADU  = std::min(BUTTON_IDX_DPADU,  SDL_CONTROLLER_BUTTON_MAX - 1);
        BUTTON_IDX_DPADD  = std::min(BUTTON_IDX_DPADD,  SDL_CONTROLLER_BUTTON_MAX - 1);
    }

    //log the controllers we see for humans if needed
    std::ofstream joyLog;
    joyLog.open((Global::pathToEXE + "Settings/ControllerLog.txt").c_str(), std::ios::out | std::ios::trunc);

    if (!joyLog.is_open())
    {
        std::fprintf(stderr, "Error: Failed to create/access '%s'\n", (Global::pathToEXE + "Settings/ControllerLog.txt").c_str());
        joyLog.close();
    }
    else
    {
        int numJoysticks = SDL_NumJoysticks();

        if (CONTROLLER_ID >= numJoysticks)
        {
            CONTROLLER_ID = -1;
        }

        for (int i = 0; i < numJoysticks; i++)
        {
            if (SDL_IsGameController(i))
            {
                SDL_GameController* c = SDL_GameControllerOpen(i);
                if (c != nullptr)
                {
                    joyLog << "Controller " + std::to_string(i) + " Name: " + SDL_GameControllerName(c) << "\n";
                    SDL_GameControllerClose(c);
                }
            }
            else
            {
                SDL_Joystick* j = SDL_JoystickOpen(i);
                if (j != nullptr)
                {
                    joyLog << "Controller " + std::to_string(i) + " Name: " + SDL_JoystickName(j) << " (Unsupported)" << "\n";
                    SDL_JoystickClose(j);
                }
            }
        }

        joyLog.close();
    }

    // Open the controller
    if (CONTROLLER_ID > -1)
    {
        Input::controller = SDL_GameControllerOpen(CONTROLLER_ID);
    }
}

std::string Input::getControllerName()
{
    if (Input::controller == nullptr)
    {
        return "None";
    }

    return SDL_GameControllerName(Input::controller);
}

bool Input::changeController(int direction)
{
    if (Input::controller != nullptr)
    {
        SDL_GameControllerClose(Input::controller);
        Input::controller = nullptr;
    }

    int originalControllerId = CONTROLLER_ID;
    int numJoysticks = SDL_NumJoysticks();
    int maxAttempts = numJoysticks + 1;
    int currentAttempt = 0;
    if (direction >= 0)
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    if (numJoysticks == 0)
    {
        return false;
    }

    while (currentAttempt < maxAttempts)
    {
        CONTROLLER_ID = (CONTROLLER_ID + direction);
        if (CONTROLLER_ID < -1)
        {
            CONTROLLER_ID = numJoysticks - 1;
        }
        else if (CONTROLLER_ID >= numJoysticks)
        {
            CONTROLLER_ID = -1;
        }

        if (CONTROLLER_ID == -1)
        {
            return true;
        }

        if (SDL_IsGameController(CONTROLLER_ID))
        {
            Input::controller = SDL_GameControllerOpen(CONTROLLER_ID);
            if (Input::controller != nullptr)
            {
                return true;
            }
        }

        currentAttempt++;
    }

    CONTROLLER_ID = originalControllerId;
    return false;
}

void Input::cleanUp()
{
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    SDL_Quit();
}
