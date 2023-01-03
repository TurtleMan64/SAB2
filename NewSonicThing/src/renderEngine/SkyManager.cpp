#include <cmath>
#include "../toolbox/maths.hpp"
#include "skymanager.hpp"
#include "../entities/entity.hpp"
#include "../engineTester/main.hpp"
#include "../entities/light.hpp"


float SkyManager::sunAngle;
float SkyManager::sunRadius;
float SkyManager::timeOfDay; //0 = morning, 360 = next morning
float SkyManager::sunModelRadius;

Vector3f SkyManager::currentFogColorDay;
Vector3f SkyManager::currentFogColorNight;
Vector3f SkyManager::colorFogInterpolated;

Vector3f SkyManager::colorSunNight;
Vector3f SkyManager::colorSunDay;
Vector3f SkyManager::colorSunInterpolated;

Vector3f SkyManager::colorMoonDay;
Vector3f SkyManager::colorMoonNight;
Vector3f SkyManager::colorMoonInterpolated;

float SkyManager::fogDensity = 0.00005f;
float SkyManager::fogGradient = 2.0f;
float SkyManager::fogBottomPosition = -10000000.0f;
float SkyManager::fogBottomThickness = 1.0f;

float SkyManager::dayFactor;
float SkyManager::nightFactor;

Entity* SkyManager::entitySun = nullptr;
Entity* SkyManager::centerObject = nullptr;

void SkyManager::initSkyManager(Entity* mySun, Entity* center)
{
    sunAngle = 0.0f;
    timeOfDay = 0.0f;
    sunRadius = 100000;
    sunModelRadius = 7400;

    currentFogColorDay.set(49 / 255.0f, 101 / 255.0f, 231 / 255.0f);
    currentFogColorNight.set(0.05f, 0.05f, 0.075f);

    colorSunNight.set(0.0f, 0.0f, 0.0f);
    colorSunDay.set(1.0f, 1.0f, 1.0f);

    colorMoonNight.set(0.3f, 0.3f, 0.45f);
    colorMoonDay.set(0.0f, 0.0f, 0.0f);

    colorSunInterpolated.set(1, 1, 1);
    colorFogInterpolated.set(1, 1, 1);
    colorMoonInterpolated.set(1, 1, 1);

    dayFactor = 1;
    nightFactor = 1;

    entitySun = mySun;
    centerObject = center;

    timeOfDay = 90;

    fogDensity = 0.00005f;
    fogGradient = 2.0f;
}

void SkyManager::calculateValues()
{
    timeOfDay = fmodf(timeOfDay + 360, 360);
    sunAngle = Maths::toRadians(timeOfDay);
    float sunPeakAngle = 60;
    float sunVangle  = Maths::toRadians(sunPeakAngle*sinf(sunAngle));
    float sunHangle  = Maths::toRadians(90 * cosf(sunAngle));
    float moonVangle = Maths::toRadians(sunPeakAngle*sinf(sunAngle - Maths::PI));
    float moonHangle = Maths::toRadians(90 * cosf(sunAngle));

    Vector3f dummy(0, 0, 0);
    Vector3f* center = &dummy;
    if (centerObject != nullptr)
    {
        center = centerObject->getPosition();
    }
    Vector3f sunOffset   = Maths::spherePositionFromAngles(sunHangle, sunVangle, sunRadius);
    Vector3f moonOffset  = Maths::spherePositionFromAngles(moonHangle, moonVangle, sunRadius);
    //Vector3f modelOffset = spherePositionFromAngles(sunHangle, sunVangle, sunModelRadius);


    Global::gameLightSun->position.set(center->x + sunOffset.x,
                                       sunOffset.y,
                                       center->z - sunOffset.z);
    sunOffset.normalize();
    sunOffset.neg();
    //Global::gameLightSun->direction = sunOffset;

    //entitySun.position.x = (center->x + modelOffset.x);
    //entitySun.position.y = (modelOffset.y);
    //entitySun.position.z = (center->z - modelOffset.z);

    Global::gameLightMoon->position.set(center->x + moonOffset.x,
                                        moonOffset.y,
                                        center->z - moonOffset.z);

    if (timeOfDay >= 180)
    {
        colorFogInterpolated.set(&currentFogColorNight);
        colorSunInterpolated.set(&colorSunNight);
        colorMoonInterpolated.set(&colorMoonNight);
    }
    else
    {
        colorFogInterpolated.set(&currentFogColorDay);
        colorSunInterpolated.set(&colorSunDay);
        colorMoonInterpolated.set(&colorMoonDay);
    }

    if (sin(sunAngle) <= 0.2f && sin(sunAngle) >= -0.2f)//  sunrise / sunset
    {
        dayFactor = ((sinf(sunAngle)*2.5f) + 0.5f);
        nightFactor = 1 - dayFactor;

        colorFogInterpolated.x = ((currentFogColorNight.x*nightFactor) + (currentFogColorDay.x*dayFactor));
        colorFogInterpolated.y = ((currentFogColorNight.y*nightFactor) + (currentFogColorDay.y*dayFactor));
        colorFogInterpolated.z = ((currentFogColorNight.z*nightFactor) + (currentFogColorDay.z*dayFactor));

        colorSunInterpolated.x = ((colorSunNight.x*nightFactor) + (colorSunDay.x*dayFactor));
        colorSunInterpolated.y = ((colorSunNight.y*nightFactor) + (colorSunDay.y*dayFactor));
        colorSunInterpolated.z = ((colorSunNight.z*nightFactor) + (colorSunDay.z*dayFactor));

        colorMoonInterpolated.x = ((colorMoonNight.x*nightFactor) + (colorMoonDay.x*dayFactor));
        colorMoonInterpolated.y = ((colorMoonNight.y*nightFactor) + (colorMoonDay.y*dayFactor));
        colorMoonInterpolated.z = ((colorMoonNight.z*nightFactor) + (colorMoonDay.z*dayFactor));
    }

    Global::gameLightSun->color = colorSunInterpolated;
    Global::gameLightMoon->color = colorMoonInterpolated;
}

Vector3f* SkyManager::getFogColor()
{
    return &colorFogInterpolated;
}

float SkyManager::getFogRed()
{
    return colorFogInterpolated.x;
}

float SkyManager::getFogGreen()
{
    return colorFogInterpolated.y;
}

float SkyManager::getFogBlue()
{
    return colorFogInterpolated.z;
}

void SkyManager::increaseTimeOfDay(float timeIncrease)
{
    setTimeOfDay(fmodf(timeOfDay + timeIncrease, 360.0f));
}

void SkyManager::setTimeOfDay(float timeSet)
{
    timeOfDay = timeSet;
    calculateValues();
}

float SkyManager::getTimeOfDay()
{
    return timeOfDay;
}

float SkyManager::getOverallBrightness()
{
    float rawVal = sinf(sunAngle);
    if (rawVal >= 0)
    {
        return (rawVal*0.5f + 0.5f);
    }
    rawVal = rawVal*-0.3f;
    return rawVal*0.75f + 0.25f;
}

void SkyManager::setFogColors(Vector3f* newFogDay, Vector3f* newFogNight)
{
    currentFogColorDay.set(newFogDay);
    currentFogColorNight.set(newFogNight);
}

void SkyManager::setCenterObject(Entity* newCenter)
{
    centerObject = newCenter;
}

void SkyManager::setSunColorDay(Vector3f* newSunColor)
{
    colorSunDay.set(newSunColor);
}

void SkyManager::setSunColorNight(Vector3f* newSunColor)
{
    colorSunNight.set(newSunColor);
}

void SkyManager::setMoonColorDay(Vector3f* newSunColor)
{
    colorMoonDay.set(newSunColor);
}

void SkyManager::setMoonColorNight(Vector3f* newSunColor)
{
    colorMoonNight.set(newSunColor);
}

void SkyManager::setFogVars(float newDensity, float newGradient)
{
    SkyManager::fogDensity = newDensity;
    SkyManager::fogGradient = newGradient;
}

float SkyManager::getFogGradient()
{
    return SkyManager::fogGradient;
}

float SkyManager::getFogDensity()
{
    return SkyManager::fogDensity;
}
