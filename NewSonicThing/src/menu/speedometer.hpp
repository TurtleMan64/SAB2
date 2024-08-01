#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include "../toolbox/maths.hpp"

class GUIText;
class GUINumber;
class FontType;

class Speedometer
{
private:
    const float speedometerScale = 1.5f;
    const float accelSmoothAlpha = 5.0f;
    const Vector3f colorPos {0.73f, 1.0f, 0.68f};
    const Vector3f colorNeg {1.0f, 0.77f, 0.84f};
    const Vector3f colorNeutral {1.0f, 1.0f, 1.0f};

    // Speedometer elements
    GUINumber* numberSpeed = nullptr;
    GUIText* textSpeedUnits = nullptr;
    float accelSmoothed = 0.0f;
    float prevSpeed = 0.0f;

public:
    Speedometer(float posX, float posY, float scale);
    ~Speedometer();

    void setVisible(bool newVisible);
    void update(float newSpeed);
    void refresh();
};

#endif
