#ifndef HUD_H
#define HUD_H

class GUINumber;
class GUIText;
class GuiImage;
class Timer;

#include "menu.hpp"
#include <glad/glad.h>

class HUD : public Menu
{
private:
    float aspectRatio = 1.0f;
    const float safeAreaX = 0.015f;
    const float safeAreaY = 0.015f;
    const float s = 0.045f; //height of a character
    float w = 0.0f; //width of a single text character (calculated once aspect ratio is known)
    const float speedometerScale = 1.5f;

    float pauseTimer = 0.0f;

    GUINumber* numberFPS = nullptr;
    GUINumber* numberLives = nullptr;
    GUINumber* numberRings = nullptr;
    GUINumber* numberScore = nullptr;
    GUINumber* numberSpeed = nullptr;
    GUIText* textSpeedUnits = nullptr;
    GUIText* textLivesMission = nullptr;
    Timer* timer = nullptr;

    static float bonusTimer;
    static GuiImage* pointBonus;
    static GLuint pointBonusIds[10];

    static float itemTimer;
    static GuiImage* itemImage;
    static GLuint items[11];

public:
    HUD();
    ~HUD();
    void draw();
    void setVisible(bool) {}
    Menu* step();
    Timer* getTimer();

    static void displayPointBonus(int idx);
    static void displayItem(int idx);
};

#endif
