#ifndef TIMER_H
#define TIMER_H

class GUIText;
class GUINumber;
class FontType;

class Timer
{
private:
    // Parts of the timer
    GUIText* colon = nullptr;
    GUIText* dot = nullptr;
    GUINumber* minutes = nullptr;
    GUINumber* seconds = nullptr;
    GUINumber* centiseconds = nullptr;

public:
    float totalTime = 0.0f;
    bool frozen = false;

    Timer(FontType* font, float posX, float posY, float scale, int alignment, bool visible);
    ~Timer();
    float getTime();
    void setTime(float newTime);
    void refresh();
    void increment();
    void setVisible(bool newVisible);
    void freeze(bool freezeStatus);
};

#endif
