#include <fstream>
#include <cstring>
#include "speedometer.hpp"

#include "../engineTester/main.hpp"
#include "../fontMeshCreator/fonttype.hpp"
#include "../fontMeshCreator/guinumber.hpp"
#include "../fontMeshCreator/guitext.hpp"
#include "../toolbox/getline.hpp"
#include "../toolbox/split.hpp"
#include "../toolbox/maths.hpp"

Speedometer::Speedometer(float posX, float posY, float scale)
{
    this->textSpeedUnits = new GUIText("km/h", scale, Global::fontVipnagorgialla, posX, posY, 8, false); INCR_NEW("GUIText");
    this->numberSpeed = new GUINumber((int)Global::gameMainVehicleSpeed, posX - this->textSpeedUnits->maxLineWidth - 0.003f, posY, speedometerScale * scale, 8, false, 0, false); INCR_NEW("GUINumber");
    this->accelSmoothed = 0.0f;
    this->prevSpeed = Global::gameMainVehicleSpeed;

    if (colorNeutral.x < 0.0f)
    {
        colorNeutral = Vector3f(1.0f, 1.0f, 1.0f);
        colorPos     = Vector3f(1.0f, 1.0f, 1.0f);
        colorNeg     = Vector3f(1.0f, 1.0f, 1.0f);

        std::ifstream file(Global::pathToEXE + "Settings/SpeedometerSettings.ini");
        if (!file.is_open())
        {
            std::fprintf(stderr, "Error: Cannot load file '%s'\n", (Global::pathToEXE + "Settings/SpeedometerSettings.ini").c_str());
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

                if (splitLength == 4)
                {
                    if (strcmp(lineSplit[0], "Color_Neutral") == 0)
                    {
                        float r = Maths::clamp(0.0f, std::stof(lineSplit[1], nullptr), 1.0f);
                        float g = Maths::clamp(0.0f, std::stof(lineSplit[2], nullptr), 1.0f);
                        float b = Maths::clamp(0.0f, std::stof(lineSplit[3], nullptr), 1.0f);
                        colorNeutral = Vector3f(r, g, b);
                    }
                    else if (strcmp(lineSplit[0], "Color_Fast") == 0)
                    {
                        float r = Maths::clamp(0.0f, std::stof(lineSplit[1], nullptr), 1.0f);
                        float g = Maths::clamp(0.0f, std::stof(lineSplit[2], nullptr), 1.0f);
                        float b = Maths::clamp(0.0f, std::stof(lineSplit[3], nullptr), 1.0f);
                        colorPos = Vector3f(r, g, b);
                    }
                    else if (strcmp(lineSplit[0], "Color_Slow") == 0)
                    {
                        float r = Maths::clamp(0.0f, std::stof(lineSplit[1], nullptr), 1.0f);
                        float g = Maths::clamp(0.0f, std::stof(lineSplit[2], nullptr), 1.0f);
                        float b = Maths::clamp(0.0f, std::stof(lineSplit[3], nullptr), 1.0f);
                        colorNeg = Vector3f(r, g, b);
                    }
                }

                free(lineSplit);
            }
            file.close();
        }
    }
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
