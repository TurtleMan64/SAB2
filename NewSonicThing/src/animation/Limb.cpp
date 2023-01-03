#include <cmath>

#include "body.hpp"
#include "keyframe.hpp"
#include "../toolbox/maths.hpp"
#include "limb.hpp"



Limb::Limb(std::vector<TexturedModel*>* models, float pivotX, float pivotY, float pivotZ, Body* body, Limb* limb)
{
    myModels = models;
    this->pivotX = pivotX;
    this->pivotY = pivotY;
    this->pivotZ = pivotZ;
    this->body = body;
    this->limb = limb;
    visible = true;
}

void Limb::step()
{

}

void Limb::update(float time)
{
    time = fmaxf(time, 0);
    bool inRange = false;
    Keyframe* key1 = nullptr;
    Keyframe* key2 = nullptr;


    float angleX, angleY, angleZ, angleS;
    if (body != nullptr)
    {
        angleX = Maths::toRadians(body->rotX);
        angleY = Maths::toRadians(body->rotY);
        angleZ = Maths::toRadians(body->rotZ);
        angleS = Maths::toRadians(body->rotRoll);
    }
    else
    {
        angleX = Maths::toRadians(limb->rotX);
        angleY = Maths::toRadians(limb->rotY);
        angleZ = Maths::toRadians(limb->rotZ);
        angleS = Maths::toRadians(limb->rotRoll);
    }


    float newX = pivotX, newY = pivotY, newZ = pivotZ;

    float newPivotX = newX;
    float newPivotY = newY;
    float newPivotZ = newZ;

    //rotation around z axis
    newX =  (newPivotX * cosf(angleS) - newPivotY * sinf(angleS));
    newY = -(newPivotY * cosf(angleS) + newPivotX * sinf(angleS));
    newPivotX = newX;
    newPivotY = newY;
    newPivotZ = newZ;


    //rotation around x axis: this one might not be right
    newZ =  (newPivotZ * cosf(angleX) - newPivotY * sinf(angleX));
    newY = -(newPivotY * cosf(angleX) + newPivotZ * sinf(angleX));
    newPivotX = newX;
    newPivotY = newY;
    newPivotZ = newZ;


    //rotation around z axis
    newX = (newPivotX * cosf(angleZ) - newPivotY * sinf(angleZ));
    newY = (newPivotX * sinf(angleZ) + newPivotY * cosf(angleZ));
    newPivotX = newX;
    newPivotY = newY;
    newPivotZ = newZ;


    //rotation around y axis
    newX = (newPivotX * cosf(angleY) + newPivotZ * sinf(angleY));
    newZ = (newPivotZ * cosf(angleY) - newPivotX * sinf(angleY));


    float newXRot = 0;
    float newYRot = 0;
    float newZRot = 0;
    float newSRot = 0;
    float newScale = 0;

    for (unsigned int i = 0; i < animations->at(animationIndex).keyframes.size() - 1; i++)
    {
        key1 = &(animations->at(animationIndex).keyframes[i]);
        key2 = &(animations->at(animationIndex).keyframes[i + 1]);

        if (time >= key1->time && time <= key2->time)
        {
            inRange = true;
            break;
        }
    }

    if (inRange)
    {
        //linear interpolation
        //view key1 as the "base"
        //and add a certain ratio of the difference between
        //key2 and key1 to key1 for the interpolated value
        float t = (time - key1->time) / (key2->time - key1->time);

        //sinusoidal interpolation
        t = 1 - ((cosf(Maths::PI*t) + 1)*0.5f);

        float ratio = t;

        newXRot = key1->xRot + ratio*(key2->xRot - key1->xRot);
        newYRot = key1->yRot + ratio*(key2->yRot - key1->yRot);
        newZRot = key1->zRot + ratio*(key2->zRot - key1->zRot);
        newSRot = key1->sRot + ratio*(key2->sRot - key1->sRot);
        newScale = key1->scale + ratio*(key2->scale - key1->scale);
    }
    else
    {
        newXRot = key2->xRot;
        newYRot = key2->yRot;
        newZRot = key2->zRot;
        newSRot = key2->sRot;
        newScale = key2->scale;
    }

    if (body != nullptr)
    {
        newX*=body->scale;
        newY*=body->scale;
        newZ*=body->scale;
        position.x = (body->position.x + newX);
        position.y = (body->position.y + newY);
        position.z = (body->position.z + newZ);
        rotX = (body->rotX + newXRot); //twist
        rotY = (body->rotY + newYRot);
        rotZ = (body->rotZ + newZRot);
        rotRoll = (body->rotRoll + newSRot); //spin
        scale = (body->scale*newScale);
    }
    else
    {
        newX*=limb->scale;
        newY*=limb->scale;
        newZ*=limb->scale;
        position.x = (limb->position.x + newX);
        position.y = (limb->position.y + newY);
        position.z = (limb->position.z + newZ);
        rotX = (limb->rotX + newXRot); //twist
        rotY = (limb->rotY + newYRot);
        rotZ = (limb->rotZ + newZRot);
        rotRoll = (limb->rotRoll + newSRot); //spin
        scale = (limb->scale*newScale);
    }
}

std::vector<TexturedModel*>* Limb::getModels()
{
    return myModels;
}
