#include <cmath>
#include "../toolbox/maths.h"
#include "skymanager.h"
#include "../entities/entity.h"
#include "../engineTester/main.h"
#include "../entities/light.h"


float SkyManager::sunAngle;
float SkyManager::sunRadius;
float SkyManager::timeOfDay; //0 = morning, 360 = next morning
float SkyManager::sunModelRadius;

Vector3f SkyManager::currentFogColourDay;
Vector3f SkyManager::currentFogColourNight;
Vector3f SkyManager::colourFogInterpolated;

Vector3f SkyManager::colourSunNight;
Vector3f SkyManager::colourSunDay;
Vector3f SkyManager::colourSunInterpolated;

Vector3f SkyManager::colourMoonDay;
Vector3f SkyManager::colourMoonNight;
Vector3f SkyManager::colourMoonInterpolated;

float SkyManager::fogDensity = 0.00005f;
float SkyManager::fogGradient = 2.0f;

float SkyManager::dayFactor;
float SkyManager::nightFactor;

Entity* SkyManager::entitySun = nullptr;
Entity* SkyManager::centerObject = nullptr;


void SkyManager::initSkyManager(Entity* mySun, Entity *center)
{
    sunAngle = 0.0f;
    timeOfDay = 0.0f;
    sunRadius = 100000;
    sunModelRadius = 7400;

    currentFogColourDay.set(49 / 255.0f, 101 / 255.0f, 231 / 255.0f);
    currentFogColourNight.set(0.05f, 0.05f, 0.075f);

    colourSunNight.set(0.0f, 0.0f, 0.0f);
    colourSunDay.set(1.0f, 1.0f, 1.0f);

    colourMoonNight.set(0.3f, 0.3f, 0.45f);
    colourMoonDay.set(0.0f, 0.0f, 0.0f);

    colourSunInterpolated.set(1, 1, 1);
    colourFogInterpolated.set(1, 1, 1);
    colourMoonInterpolated.set(1, 1, 1);

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

    //entitySun.setX(center->x + modelOffset.x);
    //entitySun.setY(modelOffset.y);
    //entitySun.setZ(center->z - modelOffset.z);

    Global::gameLightMoon->position.set(center->x + moonOffset.x,
                                        moonOffset.y,
                                        center->z - moonOffset.z);

    if (timeOfDay >= 180)
    {
        colourFogInterpolated.set(&currentFogColourNight);
        colourSunInterpolated.set(&colourSunNight);
        colourMoonInterpolated.set(&colourMoonNight);
    }
    else
    {
        colourFogInterpolated.set(&currentFogColourDay);
        colourSunInterpolated.set(&colourSunDay);
        colourMoonInterpolated.set(&colourMoonDay);
    }

    if (sin(sunAngle) <= 0.2f && sin(sunAngle) >= -0.2f)//  sunrise / sunset
    {
        dayFactor = ((sinf(sunAngle)*2.5f) + 0.5f);
        nightFactor = 1 - dayFactor;

        colourFogInterpolated.x = ((currentFogColourNight.x*nightFactor) + (currentFogColourDay.x*dayFactor));
        colourFogInterpolated.y = ((currentFogColourNight.y*nightFactor) + (currentFogColourDay.y*dayFactor));
        colourFogInterpolated.z = ((currentFogColourNight.z*nightFactor) + (currentFogColourDay.z*dayFactor));

        colourSunInterpolated.x = ((colourSunNight.x*nightFactor) + (colourSunDay.x*dayFactor));
        colourSunInterpolated.y = ((colourSunNight.y*nightFactor) + (colourSunDay.y*dayFactor));
        colourSunInterpolated.z = ((colourSunNight.z*nightFactor) + (colourSunDay.z*dayFactor));

        colourMoonInterpolated.x = ((colourMoonNight.x*nightFactor) + (colourMoonDay.x*dayFactor));
        colourMoonInterpolated.y = ((colourMoonNight.y*nightFactor) + (colourMoonDay.y*dayFactor));
        colourMoonInterpolated.z = ((colourMoonNight.z*nightFactor) + (colourMoonDay.z*dayFactor));
    }

    Global::gameLightSun->color = colourSunInterpolated;
    Global::gameLightMoon->color = colourMoonInterpolated;
}

Vector3f* SkyManager::getFogColour()
{
    return &colourFogInterpolated;
}

float SkyManager::getFogRed()
{
    return colourFogInterpolated.x;
}

float SkyManager::getFogGreen()
{
    return colourFogInterpolated.y;
}

float SkyManager::getFogBlue()
{
    return colourFogInterpolated.z;
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

void SkyManager::setFogColours(Vector3f* newFogDay, Vector3f* newFogNight)
{
    currentFogColourDay.set(newFogDay);
    currentFogColourNight.set(newFogNight);
}

void SkyManager::setCenterObject(Entity* newCenter)
{
    centerObject = newCenter;
}

void SkyManager::setSunColorDay(Vector3f* newSunColor)
{
    colourSunDay.set(newSunColor);
}

void SkyManager::setSunColorNight(Vector3f* newSunColor)
{
    colourSunNight.set(newSunColor);
}

void SkyManager::setMoonColorDay(Vector3f* newSunColor)
{
    colourMoonDay.set(newSunColor);
}

void SkyManager::setMoonColorNight(Vector3f* newSunColor)
{
    colourMoonNight.set(newSunColor);
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
