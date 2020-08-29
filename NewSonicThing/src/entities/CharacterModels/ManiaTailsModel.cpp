#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "maniatailsmodel.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../entities/playersonic.h"
#include "../../toolbox/maths.h"
#include "../../animation/body.h"
#include "../../animation/limb.h"
#include "../../animation/animationresources.h"
#include "../../particles/particletexture.h"
#include "../../particles/particleresources.h"
#include "../dummy.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> ManiaTailsModel::modelBody;
std::list<TexturedModel*> ManiaTailsModel::modelHead;
std::list<TexturedModel*> ManiaTailsModel::modelLeftHumerus;
std::list<TexturedModel*> ManiaTailsModel::modelLeftForearm;
std::list<TexturedModel*> ManiaTailsModel::modelLeftHand;
std::list<TexturedModel*> ManiaTailsModel::modelLeftThigh;
std::list<TexturedModel*> ManiaTailsModel::modelLeftShin;
std::list<TexturedModel*> ManiaTailsModel::modelLeftFoot;
std::list<TexturedModel*> ManiaTailsModel::modelRightHumerus;
std::list<TexturedModel*> ManiaTailsModel::modelRightForearm;
std::list<TexturedModel*> ManiaTailsModel::modelRightHand;
std::list<TexturedModel*> ManiaTailsModel::modelRightThigh;
std::list<TexturedModel*> ManiaTailsModel::modelRightShin;
std::list<TexturedModel*> ManiaTailsModel::modelRightFoot;

std::list<TexturedModel*> ManiaTailsModel::modelDash0;
std::list<TexturedModel*> ManiaTailsModel::modelDash1;
std::list<TexturedModel*> ManiaTailsModel::modelDash2;
std::list<TexturedModel*> ManiaTailsModel::modelDash3;
std::list<TexturedModel*> ManiaTailsModel::modelDash4;
std::list<TexturedModel*> ManiaTailsModel::modelDash5;
std::list<TexturedModel*> ManiaTailsModel::modelDash6;
std::list<TexturedModel*> ManiaTailsModel::modelDash7;
std::list<TexturedModel*> ManiaTailsModel::modelDash8;
std::list<TexturedModel*> ManiaTailsModel::modelDash9;
std::list<TexturedModel*> ManiaTailsModel::modelDash10;
std::list<TexturedModel*> ManiaTailsModel::modelDash11;
std::list<TexturedModel*> ManiaTailsModel::modelDash12;
std::list<TexturedModel*> ManiaTailsModel::modelDash13;
std::list<TexturedModel*> ManiaTailsModel::modelDash14;
std::list<TexturedModel*> ManiaTailsModel::modelDash15;
std::list<TexturedModel*> ManiaTailsModel::modelDash16;
std::list<TexturedModel*> ManiaTailsModel::modelDash17;
std::list<TexturedModel*> ManiaTailsModel::modelJump;
std::list<TexturedModel*> ManiaTailsModel::modelJog0;
std::list<TexturedModel*> ManiaTailsModel::modelJog1;
std::list<TexturedModel*> ManiaTailsModel::modelJog2;
std::list<TexturedModel*> ManiaTailsModel::modelJog3;
std::list<TexturedModel*> ManiaTailsModel::modelJog4;
std::list<TexturedModel*> ManiaTailsModel::modelJog5;
std::list<TexturedModel*> ManiaTailsModel::modelJog6;
std::list<TexturedModel*> ManiaTailsModel::modelJog7;
std::list<TexturedModel*> ManiaTailsModel::modelJog8;
std::list<TexturedModel*> ManiaTailsModel::modelJog9;
std::list<TexturedModel*> ManiaTailsModel::modelJog10;
std::list<TexturedModel*> ManiaTailsModel::modelJog11;
std::list<TexturedModel*> ManiaTailsModel::modelJog12;
std::list<TexturedModel*> ManiaTailsModel::modelJog13;
std::list<TexturedModel*> ManiaTailsModel::modelJog14;
std::list<TexturedModel*> ManiaTailsModel::modelJog15;
std::list<TexturedModel*> ManiaTailsModel::modelJog16;
std::list<TexturedModel*> ManiaTailsModel::modelJog17;
std::list<TexturedModel*> ManiaTailsModel::modelGrind;
std::list<TexturedModel*> ManiaTailsModel::modelSkid;
std::list<TexturedModel*> ManiaTailsModel::modelLightdash;
std::list<TexturedModel*> ManiaTailsModel::modelFreefall;
std::list<TexturedModel*> ManiaTailsModel::modelStomp;
std::list<TexturedModel*> ManiaTailsModel::modelGrab;
std::list<TexturedModel*> ManiaTailsModel::modelFlyBody;
std::list<TexturedModel*> ManiaTailsModel::modelFlyBodyTired;
std::list<TexturedModel*> ManiaTailsModel::modelFlyTails;

ManiaTailsModel::ManiaTailsModel()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 0.30175f;
    visible = false;
    models = &ManiaTailsModel::modelDash0;

    flyTails = new Dummy(&ManiaTailsModel::modelFlyTails); INCR_NEW("Entity");
    Global::addEntity(flyTails);

    myBody =         new Body(&modelBody);                                                                   INCR_NEW("Entity");
    myHead =         new Limb(&modelHead,         0.93717f, -0.20826f,  0,        myBody,   nullptr);        INCR_NEW("Entity");
    myLeftHumerus =  new Limb(&modelLeftHumerus,  0.62478f,  0,        -0.62478f, myBody,   nullptr);        INCR_NEW("Entity");
    myLeftForearm =  new Limb(&modelLeftForearm,  0,        -0.74592f,  0,        nullptr,  myLeftHumerus);  INCR_NEW("Entity");
    myLeftHand =     new Limb(&modelLeftHand,     0,        -0.90246f,  0,        nullptr,  myLeftForearm);  INCR_NEW("Entity");
    myLeftThigh =    new Limb(&modelLeftThigh,   -0.62478f,  0,        -0.20826f, myBody,   nullptr);        INCR_NEW("Entity");
    myLeftShin =     new Limb(&modelLeftShin,     0,        -0.64592f,  0,        nullptr,  myLeftThigh);    INCR_NEW("Entity");
    myLeftFoot =     new Limb(&modelLeftFoot,     0,        -1.0413f,   0,        nullptr,  myLeftShin);     INCR_NEW("Entity");
    myRightHumerus = new Limb(&modelRightHumerus, 0.62478f,  0,         0.62478f, myBody,   nullptr);        INCR_NEW("Entity");
    myRightForearm = new Limb(&modelRightForearm, 0,        -0.74592f,  0,        nullptr,  myRightHumerus); INCR_NEW("Entity");
    myRightHand =    new Limb(&modelRightHand,    0,        -0.90246f,  0,        nullptr,  myRightForearm); INCR_NEW("Entity");
    myRightThigh =   new Limb(&modelRightThigh,  -0.62478f,  0,         0.20826f, myBody,   nullptr);        INCR_NEW("Entity");
    myRightShin =    new Limb(&modelRightShin,    0,        -0.62592f,  0,        nullptr,  myRightThigh);   INCR_NEW("Entity");
    myRightFoot =    new Limb(&modelRightFoot,    0,        -1.0413f,   0,        nullptr,  myRightShin);    INCR_NEW("Entity");

    AnimationResources::assignAnimationsHuman(myBody, myHead,
        myLeftHumerus, myLeftForearm, myLeftHand,
        myRightHumerus, myRightForearm, myRightHand,
        myLeftThigh, myLeftShin, myLeftFoot,
        myRightThigh, myRightShin, myRightFoot);

    Global::addEntity(myBody);
    Global::addEntity(myHead);
    Global::addEntity(myLeftHumerus);
    Global::addEntity(myLeftForearm);
    Global::addEntity(myLeftHand);
    Global::addEntity(myLeftThigh);
    Global::addEntity(myLeftShin);
    Global::addEntity(myLeftFoot);
    Global::addEntity(myRightHumerus);
    Global::addEntity(myRightForearm);
    Global::addEntity(myRightHand);
    Global::addEntity(myRightThigh);
    Global::addEntity(myRightShin);
    Global::addEntity(myRightFoot);
}

void ManiaTailsModel::step()
{

}

const float ManiaTailsModel::getDisplayBallOffset()
{
    return displayBallOffset;
}

ParticleTexture* ManiaTailsModel::getBallTexture()
{
    return ParticleResources::textureOrangeTrail;
}

void ManiaTailsModel::animate(int animIndex, float time)
{
    currentAnimIndex = animIndex;
    currentAnimTime = time;
    switch (animIndex)
    {
        case 0: //stand
        {
            Vector3f off = currentUpDirection.scaleCopy(limbsScale*displayHeightOffset);
            Vector3f pos = position + off;
            myBody->setBaseOrientation(pos.x, pos.y, pos.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(0, fmodf(time, 100.0f));
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 1: //run
        {
            scale = 0.30175f;
            int index = (int)(time / 13.0f);
            switch (index % 18)
            {
                case  0: models = &ManiaTailsModel::modelDash0;  break;
                case  1: models = &ManiaTailsModel::modelDash1;  break;
                case  2: models = &ManiaTailsModel::modelDash2;  break;
                case  3: models = &ManiaTailsModel::modelDash3;  break;
                case  4: models = &ManiaTailsModel::modelDash4;  break;
                case  5: models = &ManiaTailsModel::modelDash5;  break;
                case  6: models = &ManiaTailsModel::modelDash6;  break;
                case  7: models = &ManiaTailsModel::modelDash7;  break;
                case  8: models = &ManiaTailsModel::modelDash8;  break;
                case  9: models = &ManiaTailsModel::modelDash9;  break;
                case 10: models = &ManiaTailsModel::modelDash10; break;
                case 11: models = &ManiaTailsModel::modelDash11; break;
                case 12: models = &ManiaTailsModel::modelDash12; break;
                case 13: models = &ManiaTailsModel::modelDash13; break;
                case 14: models = &ManiaTailsModel::modelDash14; break;
                case 15: models = &ManiaTailsModel::modelDash15; break;
                case 16: models = &ManiaTailsModel::modelDash16; break;
                case 17: models = &ManiaTailsModel::modelDash17; break;
                default: models = &ManiaTailsModel::modelDash17; break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 3: //stomp
        {
            scale = 1.0f;
            models = &ManiaTailsModel::modelStomp;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 8: //skid
        {
            scale = 1.0f;
            models = &ManiaTailsModel::modelSkid;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 11: //hitstun
        {
            updateLimbs(11, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            setFlyVisibility(false);
            break;
        }

        case 12: //jump
        {
            scale = 0.32f;
            models = &ManiaTailsModel::modelJump;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 14: //dab
        {
            Vector3f off = currentUpDirection.scaleCopy(limbsScale*displayHeightOffset);
            Vector3f pos = position + off;
            myBody->setBaseOrientation(pos.x, pos.y, pos.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(14, time);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 15: //jog
        {
            scale = 0.30175f;
            int index = (int)(time / 5.55555555f);
            switch (index % 18)
            {
                case 0:  models = &ManiaTailsModel::modelJog0;  break;
                case 1:  models = &ManiaTailsModel::modelJog1;  break;
                case 2:  models = &ManiaTailsModel::modelJog2;  break;
                case 3:  models = &ManiaTailsModel::modelJog3;  break;
                case 4:  models = &ManiaTailsModel::modelJog4;  break;
                case 5:  models = &ManiaTailsModel::modelJog5;  break;
                case 6:  models = &ManiaTailsModel::modelJog6;  break;
                case 7:  models = &ManiaTailsModel::modelJog7;  break;
                case 8:  models = &ManiaTailsModel::modelJog8;  break;
                case 9:  models = &ManiaTailsModel::modelJog9;  break;
                case 10: models = &ManiaTailsModel::modelJog10; break;
                case 11: models = &ManiaTailsModel::modelJog11; break;
                case 12: models = &ManiaTailsModel::modelJog12; break;
                case 13: models = &ManiaTailsModel::modelJog13; break;
                case 14: models = &ManiaTailsModel::modelJog14; break;
                case 15: models = &ManiaTailsModel::modelJog15; break;
                case 16: models = &ManiaTailsModel::modelJog16; break;
                case 17: models = &ManiaTailsModel::modelJog17; break;
                default: models = &ManiaTailsModel::modelJog17; break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 18: //lightdash
        {
            scale = 1.0f;
            models = &ManiaTailsModel::modelLightdash;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 19: //die
        {
            updateLimbs(19, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 20: //fly
        {
            scale = 1.0f;
            setLimbsVisibility(false);
            visible = baseVisible;
            setFlyVisibility(baseVisible);
            models = &ManiaTailsModel::modelFlyBody;
            flyTails->rotY += time*0.6f;
            updateFlyMatrix();
            updateTransformationMatrix();
            break;
        }

        case 21: //freefall
        {
            scale = 1.0f;
            models = &ManiaTailsModel::modelFreefall;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 25: //grab
        {
            scale = 1.0f;
            models = &ManiaTailsModel::modelGrab;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 26: //grind
        {
            scale = 1.0f;
            models = &ManiaTailsModel::modelGrind;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 27: //fly tired
        {
            scale = 1.0f;
            setLimbsVisibility(false);
            visible = baseVisible;
            setFlyVisibility(baseVisible);
            models = &ManiaTailsModel::modelFlyBodyTired;
            flyTails->rotY += time*0.2f;
            updateFlyMatrix();
            updateTransformationMatrix();
            break;
        }

        default:
            break;
    }
}

void ManiaTailsModel::setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, Vector3f* newUp)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX       = xRot;
    rotY       = yRot;
    rotZ       = zRot;
    rotRoll    = spinRot;
    currentUpDirection.set(newUp);
    myBody->setBaseOrientation(x, y, z, rotX, rotY, rotZ, rotRoll, limbsScale);
    flyTails->position.set(x, y, z);
    flyTails->setRotation(xRot, yRot, zRot, spinRot);
}

void ManiaTailsModel::setBaseColor(float r, float g, float b)
{
                    baseColour.set(r, g, b);
    myBody        ->baseColour.set(r, g, b);
    myHead        ->baseColour.set(r, g, b);
    myLeftHumerus ->baseColour.set(r, g, b);
    myLeftForearm ->baseColour.set(r, g, b);
    myLeftHand    ->baseColour.set(r, g, b);
    myLeftThigh   ->baseColour.set(r, g, b);
    myLeftShin    ->baseColour.set(r, g, b);
    myLeftFoot    ->baseColour.set(r, g, b);
    myRightHumerus->baseColour.set(r, g, b);
    myRightForearm->baseColour.set(r, g, b);
    myRightHand   ->baseColour.set(r, g, b);
    myRightThigh  ->baseColour.set(r, g, b);
    myRightShin   ->baseColour.set(r, g, b);
    myRightFoot   ->baseColour.set(r, g, b);
    flyTails      ->baseColour.set(r, g, b);
}

void ManiaTailsModel::setBaseAlpha(float a)
{
                    baseAlpha = a;
    myBody        ->baseAlpha = a;
    myHead        ->baseAlpha = a;
    myLeftHumerus ->baseAlpha = a;
    myLeftForearm ->baseAlpha = a;
    myLeftHand    ->baseAlpha = a;
    myLeftThigh   ->baseAlpha = a;
    myLeftShin    ->baseAlpha = a;
    myLeftFoot    ->baseAlpha = a;
    myRightHumerus->baseAlpha = a;
    myRightForearm->baseAlpha = a;
    myRightHand   ->baseAlpha = a;
    myRightThigh  ->baseAlpha = a;
    myRightShin   ->baseAlpha = a;
    myRightFoot   ->baseAlpha = a;
    flyTails      ->baseAlpha = a;
}

void ManiaTailsModel::setRenderOrderOverride(char newOrder)
{
                    renderOrderOverride = newOrder;
    myBody        ->renderOrderOverride = newOrder;
    myHead        ->renderOrderOverride = newOrder;
    myLeftHumerus ->renderOrderOverride = newOrder;
    myLeftForearm ->renderOrderOverride = newOrder;
    myLeftHand    ->renderOrderOverride = newOrder;
    myLeftThigh   ->renderOrderOverride = newOrder;
    myLeftShin    ->renderOrderOverride = newOrder;
    myLeftFoot    ->renderOrderOverride = newOrder;
    myRightHumerus->renderOrderOverride = newOrder;
    myRightForearm->renderOrderOverride = newOrder;
    myRightHand   ->renderOrderOverride = newOrder;
    myRightThigh  ->renderOrderOverride = newOrder;
    myRightShin   ->renderOrderOverride = newOrder;
    myRightFoot   ->renderOrderOverride = newOrder;
    flyTails      ->renderOrderOverride = newOrder;
}

void ManiaTailsModel::setBaseVisibility(bool newVisible)
{
    baseVisible = newVisible;
}

std::list<TexturedModel*>* ManiaTailsModel::getModels()
{
    return models;
}

void ManiaTailsModel::loadStaticModels()
{
    if (ManiaTailsModel::modelDash0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading mania tails static models...\n");
    #endif

    ObjLoader::loadModel(&ManiaTailsModel::modelBody,         "res/Models/Characters/ManiaTails/", "Body");
    ObjLoader::loadModel(&ManiaTailsModel::modelHead,         "res/Models/Characters/ManiaTails/", "Head");
    ObjLoader::loadModel(&ManiaTailsModel::modelLeftHumerus,  "res/Models/Characters/ManiaTails/", "Humerus");
    ObjLoader::loadModel(&ManiaTailsModel::modelLeftForearm,  "res/Models/Characters/ManiaTails/", "Forearm");
    ObjLoader::loadModel(&ManiaTailsModel::modelLeftHand,     "res/Models/Characters/ManiaTails/", "HandLeft");
    ObjLoader::loadModel(&ManiaTailsModel::modelLeftThigh,    "res/Models/Characters/ManiaTails/", "Humerus");
    ObjLoader::loadModel(&ManiaTailsModel::modelLeftShin,     "res/Models/Characters/ManiaTails/", "Forearm");
    ObjLoader::loadModel(&ManiaTailsModel::modelLeftFoot,     "res/Models/Characters/ManiaTails/", "ShoeLeft");
    ObjLoader::loadModel(&ManiaTailsModel::modelRightHumerus, "res/Models/Characters/ManiaTails/", "Humerus");
    ObjLoader::loadModel(&ManiaTailsModel::modelRightForearm, "res/Models/Characters/ManiaTails/", "Forearm");
    ObjLoader::loadModel(&ManiaTailsModel::modelRightHand,    "res/Models/Characters/ManiaTails/", "HandRight");
    ObjLoader::loadModel(&ManiaTailsModel::modelRightThigh,   "res/Models/Characters/ManiaTails/", "Humerus");
    ObjLoader::loadModel(&ManiaTailsModel::modelRightShin,    "res/Models/Characters/ManiaTails/", "Forearm");
    ObjLoader::loadModel(&ManiaTailsModel::modelRightFoot,    "res/Models/Characters/ManiaTails/", "ShoeRight");

    ObjLoader::loadModel(&ManiaTailsModel::modelDash0       , "res/Models/Characters/ManiaTails/", "Dash0");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash1       , "res/Models/Characters/ManiaTails/", "Dash1");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash2       , "res/Models/Characters/ManiaTails/", "Dash2");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash3       , "res/Models/Characters/ManiaTails/", "Dash3");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash4       , "res/Models/Characters/ManiaTails/", "Dash4");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash5       , "res/Models/Characters/ManiaTails/", "Dash5");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash6       , "res/Models/Characters/ManiaTails/", "Dash6");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash7       , "res/Models/Characters/ManiaTails/", "Dash7");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash8       , "res/Models/Characters/ManiaTails/", "Dash8");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash9       , "res/Models/Characters/ManiaTails/", "Dash9");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash10      , "res/Models/Characters/ManiaTails/", "Dash10");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash11      , "res/Models/Characters/ManiaTails/", "Dash11");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash12      , "res/Models/Characters/ManiaTails/", "Dash12");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash13      , "res/Models/Characters/ManiaTails/", "Dash13");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash14      , "res/Models/Characters/ManiaTails/", "Dash14");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash15      , "res/Models/Characters/ManiaTails/", "Dash15");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash16      , "res/Models/Characters/ManiaTails/", "Dash16");
    ObjLoader::loadModel(&ManiaTailsModel::modelDash17      , "res/Models/Characters/ManiaTails/", "Dash17");
    ObjLoader::loadModel(&ManiaTailsModel::modelJump        , "res/Models/Characters/ManiaTails/", "Jump");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog0        , "res/Models/Characters/ManiaTails/", "Jog0");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog1        , "res/Models/Characters/ManiaTails/", "Jog1");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog2        , "res/Models/Characters/ManiaTails/", "Jog2");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog3        , "res/Models/Characters/ManiaTails/", "Jog3");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog4        , "res/Models/Characters/ManiaTails/", "Jog4");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog5        , "res/Models/Characters/ManiaTails/", "Jog5");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog6        , "res/Models/Characters/ManiaTails/", "Jog6");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog7        , "res/Models/Characters/ManiaTails/", "Jog7");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog8        , "res/Models/Characters/ManiaTails/", "Jog8");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog9        , "res/Models/Characters/ManiaTails/", "Jog9");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog10       , "res/Models/Characters/ManiaTails/", "Jog10");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog11       , "res/Models/Characters/ManiaTails/", "Jog11");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog12       , "res/Models/Characters/ManiaTails/", "Jog12");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog13       , "res/Models/Characters/ManiaTails/", "Jog13");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog14       , "res/Models/Characters/ManiaTails/", "Jog14");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog15       , "res/Models/Characters/ManiaTails/", "Jog15");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog16       , "res/Models/Characters/ManiaTails/", "Jog16");
    ObjLoader::loadModel(&ManiaTailsModel::modelJog17       , "res/Models/Characters/ManiaTails/", "Jog17");
    ObjLoader::loadModel(&ManiaTailsModel::modelGrind       , "res/Models/Characters/ManiaTails/", "Grind");
    ObjLoader::loadModel(&ManiaTailsModel::modelSkid        , "res/Models/Characters/ManiaTails/", "Skid");
    ObjLoader::loadModel(&ManiaTailsModel::modelLightdash   , "res/Models/Characters/ManiaTails/", "Lightdash");
    ObjLoader::loadModel(&ManiaTailsModel::modelFreefall    , "res/Models/Characters/ManiaTails/", "Freefall");
    ObjLoader::loadModel(&ManiaTailsModel::modelStomp       , "res/Models/Characters/ManiaTails/", "Freefall");
    ObjLoader::loadModel(&ManiaTailsModel::modelGrab        , "res/Models/Characters/ManiaTails/", "Grab");
    ObjLoader::loadModel(&ManiaTailsModel::modelFlyBody     , "res/Models/Characters/ManiaTails/", "FlyBody");
    ObjLoader::loadModel(&ManiaTailsModel::modelFlyBodyTired, "res/Models/Characters/ManiaTails/", "FlyBodyTired");
    ObjLoader::loadModel(&ManiaTailsModel::modelFlyTails    , "res/Models/Characters/ManiaTails/", "FlyTails");
}

void ManiaTailsModel::setLimbsVisibility(bool newVisible)
{
    myBody->setVisible(newVisible);
    myHead->setVisible(newVisible);
    myLeftHumerus->setVisible(newVisible);
    myLeftForearm->setVisible(newVisible);
    myLeftHand->setVisible(newVisible);
    myLeftThigh->setVisible(newVisible);
    myLeftShin->setVisible(newVisible);
    myLeftFoot->setVisible(newVisible);
    myRightHumerus->setVisible(newVisible);
    myRightForearm->setVisible(newVisible);
    myRightHand->setVisible(newVisible);
    myRightThigh->setVisible(newVisible);
    myRightShin->setVisible(newVisible);
    myRightFoot->setVisible(newVisible);
}

void ManiaTailsModel::updateLimbs(int animIndex, float time)
{
    myBody->animationIndex = animIndex;
    myHead->animationIndex = animIndex;
    myLeftHumerus->animationIndex = animIndex;
    myLeftForearm->animationIndex = animIndex;
    myLeftHand->animationIndex = animIndex;
    myLeftThigh->animationIndex = animIndex;
    myLeftShin->animationIndex = animIndex;
    myLeftFoot->animationIndex = animIndex;
    myRightHumerus->animationIndex = animIndex;
    myRightForearm->animationIndex = animIndex;
    myRightHand->animationIndex = animIndex;
    myRightThigh->animationIndex = animIndex;
    myRightShin->animationIndex = animIndex;
    myRightFoot->animationIndex = animIndex;
    myBody->update(time);
    myHead->update(time);
    myLeftHumerus->update(time);
    myLeftForearm->update(time);
    myLeftHand->update(time);
    myLeftThigh->update(time);
    myLeftShin->update(time);
    myLeftFoot->update(time);
    myRightHumerus->update(time);
    myRightForearm->update(time);
    myRightHand->update(time);
    myRightThigh->update(time);
    myRightShin->update(time);
    myRightFoot->update(time);
}

void ManiaTailsModel::updateLimbsMatrix()
{
    myBody->updateTransformationMatrix();
    myHead->updateTransformationMatrix();
    myLeftHumerus->updateTransformationMatrix();
    myLeftForearm->updateTransformationMatrix();
    myLeftHand->updateTransformationMatrix();
    myLeftThigh->updateTransformationMatrix();
    myLeftShin->updateTransformationMatrix();
    myLeftFoot->updateTransformationMatrix();
    myRightHumerus->updateTransformationMatrix();
    myRightForearm->updateTransformationMatrix();
    myRightHand->updateTransformationMatrix();
    myRightThigh->updateTransformationMatrix();
    myRightShin->updateTransformationMatrix();
    myRightFoot->updateTransformationMatrix();
}

void ManiaTailsModel::setFlyVisibility(bool newVisible)
{
    flyTails->visible = newVisible;
}

void ManiaTailsModel::updateFlyMatrix()
{
    flyTails->updateTransformationMatrix();
}

void ManiaTailsModel::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting mania tails static models...\n");
    #endif

    Entity::deleteModels(&ManiaTailsModel::modelBody);
    Entity::deleteModels(&ManiaTailsModel::modelHead);
    Entity::deleteModels(&ManiaTailsModel::modelLeftHumerus);
    Entity::deleteModels(&ManiaTailsModel::modelLeftForearm);
    Entity::deleteModels(&ManiaTailsModel::modelLeftHand);
    Entity::deleteModels(&ManiaTailsModel::modelLeftThigh);
    Entity::deleteModels(&ManiaTailsModel::modelLeftShin);
    Entity::deleteModels(&ManiaTailsModel::modelLeftFoot);
    Entity::deleteModels(&ManiaTailsModel::modelRightHumerus);
    Entity::deleteModels(&ManiaTailsModel::modelRightForearm);
    Entity::deleteModels(&ManiaTailsModel::modelRightHand);
    Entity::deleteModels(&ManiaTailsModel::modelRightThigh);
    Entity::deleteModels(&ManiaTailsModel::modelRightShin);
    Entity::deleteModels(&ManiaTailsModel::modelRightFoot);

    Entity::deleteModels(&ManiaTailsModel::modelDash0);
    Entity::deleteModels(&ManiaTailsModel::modelDash1);
    Entity::deleteModels(&ManiaTailsModel::modelDash2);
    Entity::deleteModels(&ManiaTailsModel::modelDash3);
    Entity::deleteModels(&ManiaTailsModel::modelDash4);
    Entity::deleteModels(&ManiaTailsModel::modelDash5);
    Entity::deleteModels(&ManiaTailsModel::modelDash6);
    Entity::deleteModels(&ManiaTailsModel::modelDash7);
    Entity::deleteModels(&ManiaTailsModel::modelDash8);
    Entity::deleteModels(&ManiaTailsModel::modelDash9);
    Entity::deleteModels(&ManiaTailsModel::modelDash10);
    Entity::deleteModels(&ManiaTailsModel::modelDash11);
    Entity::deleteModels(&ManiaTailsModel::modelDash12);
    Entity::deleteModels(&ManiaTailsModel::modelDash13);
    Entity::deleteModels(&ManiaTailsModel::modelDash14);
    Entity::deleteModels(&ManiaTailsModel::modelDash15);
    Entity::deleteModels(&ManiaTailsModel::modelDash16);
    Entity::deleteModels(&ManiaTailsModel::modelDash17);
    Entity::deleteModels(&ManiaTailsModel::modelJump);
    Entity::deleteModels(&ManiaTailsModel::modelJog0);
    Entity::deleteModels(&ManiaTailsModel::modelJog1);
    Entity::deleteModels(&ManiaTailsModel::modelJog2);
    Entity::deleteModels(&ManiaTailsModel::modelJog3);
    Entity::deleteModels(&ManiaTailsModel::modelJog4);
    Entity::deleteModels(&ManiaTailsModel::modelJog5);
    Entity::deleteModels(&ManiaTailsModel::modelJog6);
    Entity::deleteModels(&ManiaTailsModel::modelJog7);
    Entity::deleteModels(&ManiaTailsModel::modelJog8);
    Entity::deleteModels(&ManiaTailsModel::modelJog9);
    Entity::deleteModels(&ManiaTailsModel::modelJog10);
    Entity::deleteModels(&ManiaTailsModel::modelJog11);
    Entity::deleteModels(&ManiaTailsModel::modelJog12);
    Entity::deleteModels(&ManiaTailsModel::modelJog13);
    Entity::deleteModels(&ManiaTailsModel::modelJog14);
    Entity::deleteModels(&ManiaTailsModel::modelJog15);
    Entity::deleteModels(&ManiaTailsModel::modelJog16);
    Entity::deleteModels(&ManiaTailsModel::modelJog17);
    Entity::deleteModels(&ManiaTailsModel::modelGrind);
    Entity::deleteModels(&ManiaTailsModel::modelSkid);
    Entity::deleteModels(&ManiaTailsModel::modelLightdash);
    Entity::deleteModels(&ManiaTailsModel::modelFreefall);
    Entity::deleteModels(&ManiaTailsModel::modelStomp);
    Entity::deleteModels(&ManiaTailsModel::modelGrab);
    Entity::deleteModels(&ManiaTailsModel::modelFlyBody);
    Entity::deleteModels(&ManiaTailsModel::modelFlyBodyTired);
    Entity::deleteModels(&ManiaTailsModel::modelFlyTails);
}
