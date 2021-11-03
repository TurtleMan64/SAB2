#ifndef SKYMANAGER_H
#define SKYMANAGER_H

#include "../toolbox/vector.hpp"

class Camera;
class Light;
class Entity;



class SkyManager
{
private:
    static float sunAngle;
    static float sunRadius;
    static float timeOfDay; //0 = morning, 360 = next morning
    static float sunModelRadius;

    static Vector3f currentFogColorDay;
    static Vector3f currentFogColorNight;
    static Vector3f colorFogInterpolated;

    static Vector3f colorSunNight;
    static Vector3f colorSunDay;
    static Vector3f colorSunInterpolated;

    static Vector3f colorMoonDay;
    static Vector3f colorMoonNight;
    static Vector3f colorMoonInterpolated;

    static float fogDensity;// = 0.00005f;
    static float fogGradient;// = 2.0f;

    static float dayFactor;
    static float nightFactor;

    static Entity* entitySun;
    static Entity* centerObject;


public:
    static float fogBottomPosition;
    static float fogBottomThickness;

    static void initSkyManager(Entity* mySun, Entity* center);

    static void calculateValues();

    static Vector3f* getFogColor();

    static float getFogRed();

    static float getFogGreen();

    static float getFogBlue();

    static void increaseTimeOfDay(float timeIncrease);

    static void setTimeOfDay(float timeSet);

    static float getTimeOfDay();

    static float getOverallBrightness();

    static void setFogColors(Vector3f* newFogDay, Vector3f* newFogNight);

    static void setCenterObject(Entity* newCenter);

    static void setSunColorDay(Vector3f* newSunColor);

    static void setSunColorNight(Vector3f* newSunColor);

    static void setMoonColorDay(Vector3f* newSunColor);

    static void setMoonColorNight(Vector3f* newSunColor);

    static void setFogVars(float newDensity, float newGradient);

    static float getFogGradient();

    static float getFogDensity();
};

#endif
