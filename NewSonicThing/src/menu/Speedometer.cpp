#include "speedometer.hpp"

#include "../engineTester/main.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../fontMeshCreator/guinumber.hpp"
#include "../fontMeshCreator/guitext.hpp"

Speedometer::Speedometer(float posX, float posY, float scale)
{
    this->textSpeedUnits = new GUIText("km/h", scale, Global::fontVipnagorgialla, posX, posY, 8, false); INCR_NEW("GUIText");
    this->numberSpeed = new GUINumber((int)Global::gameMainVehicleSpeed, posX - this->textSpeedUnits->maxLineWidth - 0.003f, posY, speedometerScale * scale, 8, false, 0, false); INCR_NEW("GUINumber");
    this->accelSmoothed = 0.0f;
    this->prevSpeed = Global::gameMainVehicleSpeed;
}

Speedometer::~Speedometer()
{
    this->numberSpeed->deleteMe(); delete this->numberSpeed; INCR_DEL("GUINumber");
    this->textSpeedUnits->deleteMe(); delete this->textSpeedUnits; INCR_DEL("GUIText");
}

void Speedometer::setVisible(bool newVisible)
{
    this->numberSpeed->visible = newVisible;
    this->textSpeedUnits->visible = newVisible;
}

void Speedometer::update(float newSpeed)
{
    extern float dt;

    // Derive accel from previous speed value
    float accel = (newSpeed - this->prevSpeed) / dt;

    // Smooth accel with simple lowpass
    this->accelSmoothed += (accel - this->accelSmoothed) * accelSmoothAlpha * dt;
    float clampedAccel = Maths::clamp(-1.0f, this->accelSmoothed * 0.05f, 1.0f);

    if (this->accelSmoothed > 0.0f)
    {
        this->numberSpeed->baseColor = this->colorNeutral.scaleCopy(1.0f - clampedAccel) + this->colorPos.scaleCopy(clampedAccel);
    }
    else
    {
        this->numberSpeed->baseColor = this->colorNeutral.scaleCopy(1.0f - (-clampedAccel)) + this->colorNeg.scaleCopy(-clampedAccel);
    }

    // Save speed for next tick
    this->prevSpeed = newSpeed;

    // And display the number
    this->numberSpeed->displayNumber = (int)newSpeed;
}

void Speedometer::refresh()
{
    this->numberSpeed->refresh();
}
