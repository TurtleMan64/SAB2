#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "maniaknucklesmodel.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../entities/playersonic.h"
#include "../../toolbox/maths.h"
#include "../../animation/body.h"
#include "../../animation/limb.h"
#include "../../animation/animationresources.h"
#include "../../particles/particletexture.h"
#include "../../particles/particleresources.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> ManiaKnucklesModel::modelBody;
std::list<TexturedModel*> ManiaKnucklesModel::modelHead;
std::list<TexturedModel*> ManiaKnucklesModel::modelLeftHumerus;
std::list<TexturedModel*> ManiaKnucklesModel::modelLeftForearm;
std::list<TexturedModel*> ManiaKnucklesModel::modelLeftHand;
std::list<TexturedModel*> ManiaKnucklesModel::modelLeftThigh;
std::list<TexturedModel*> ManiaKnucklesModel::modelLeftShin;
std::list<TexturedModel*> ManiaKnucklesModel::modelLeftFoot;
std::list<TexturedModel*> ManiaKnucklesModel::modelRightHumerus;
std::list<TexturedModel*> ManiaKnucklesModel::modelRightForearm;
std::list<TexturedModel*> ManiaKnucklesModel::modelRightHand;
std::list<TexturedModel*> ManiaKnucklesModel::modelRightThigh;
std::list<TexturedModel*> ManiaKnucklesModel::modelRightShin;
std::list<TexturedModel*> ManiaKnucklesModel::modelRightFoot;

std::list<TexturedModel*> ManiaKnucklesModel::modelDash0;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash1;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash2;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash3;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash4;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash5;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash6;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash7;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash8;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash9;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash10;
std::list<TexturedModel*> ManiaKnucklesModel::modelDash11;
std::list<TexturedModel*> ManiaKnucklesModel::modelJump;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog0;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog1;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog2;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog3;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog4;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog5;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog6;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog7;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog8;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog9;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog10;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog11;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog12;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog13;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog14;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog15;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog16;
std::list<TexturedModel*> ManiaKnucklesModel::modelJog17;
std::list<TexturedModel*> ManiaKnucklesModel::modelGrind;
std::list<TexturedModel*> ManiaKnucklesModel::modelStomp;
std::list<TexturedModel*> ManiaKnucklesModel::modelGrab;

ManiaKnucklesModel::ManiaKnucklesModel()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 0.27f;
    visible = false;
    models = &ManiaKnucklesModel::modelDash0;

    const float s = 3.0f;

    myBody =         new Body(&modelBody); INCR_NEW("Entity");
    myHead =         new Limb(&modelHead,         0.97f*s,-0.5f*s,  0,      myBody,   nullptr);        INCR_NEW("Entity");
    myLeftHumerus =  new Limb(&modelLeftHumerus,  0.6f*s,  0,      -0.8f*s, myBody,   nullptr);        INCR_NEW("Entity");
    myLeftForearm =  new Limb(&modelLeftForearm,  0,      -1.5f*s,  0,      nullptr,  myLeftHumerus);  INCR_NEW("Entity");
    myLeftHand =     new Limb(&modelLeftHand,     0,      -1.3f*s,  0,      nullptr,  myLeftForearm);  INCR_NEW("Entity");
    myLeftThigh =    new Limb(&modelLeftThigh,   -0.9f*s,  0,      -0.3f*s, myBody,   nullptr);        INCR_NEW("Entity");
    myLeftShin =     new Limb(&modelLeftShin,     0,      -1.5f*s,  0,      nullptr,  myLeftThigh);    INCR_NEW("Entity");
    myLeftFoot =     new Limb(&modelLeftFoot,     0,      -1.5f*s,  0,      nullptr,  myLeftShin);     INCR_NEW("Entity");
    myRightHumerus = new Limb(&modelRightHumerus, 0.6f*s,  0,       0.8f*s, myBody,   nullptr);        INCR_NEW("Entity");
    myRightForearm = new Limb(&modelRightForearm, 0,      -1.5f*s,  0,      nullptr,  myRightHumerus); INCR_NEW("Entity");
    myRightHand =    new Limb(&modelRightHand,    0,      -1.3f*s,  0,      nullptr,  myRightForearm); INCR_NEW("Entity");
    myRightThigh =   new Limb(&modelRightThigh,  -0.9f*s,  0,       0.3f*s, myBody,   nullptr);        INCR_NEW("Entity");
    myRightShin =    new Limb(&modelRightShin,    0,      -1.5f*s,  0,      nullptr,  myRightThigh);   INCR_NEW("Entity");
    myRightFoot =    new Limb(&modelRightFoot,    0,      -1.5f*s,  0,      nullptr,  myRightShin);    INCR_NEW("Entity");

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

void ManiaKnucklesModel::step()
{

}

float ManiaKnucklesModel::getDisplayBallOffset() const
{
    return displayBallOffset;
}

ParticleTexture* ManiaKnucklesModel::getBallTexture()
{
    return ParticleResources::textureRedTrail;
}

void ManiaKnucklesModel::animate(int animIndex, float time)
{
    const float limbsScale = 0.30f;

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
            visible = false;
            break;
        }

        case 1: //run
        {
            scale = 0.27f;
            int index = (int)(time / 8.3333333f);
            switch (index % 12)
            {
                case  0: models = &ManiaKnucklesModel::modelDash0;  break;
                case  1: models = &ManiaKnucklesModel::modelDash1;  break;
                case  2: models = &ManiaKnucklesModel::modelDash2;  break;
                case  3: models = &ManiaKnucklesModel::modelDash3;  break;
                case  4: models = &ManiaKnucklesModel::modelDash4;  break;
                case  5: models = &ManiaKnucklesModel::modelDash5;  break;
                case  6: models = &ManiaKnucklesModel::modelDash6;  break;
                case  7: models = &ManiaKnucklesModel::modelDash7;  break;
                case  8: models = &ManiaKnucklesModel::modelDash8;  break;
                case  9: models = &ManiaKnucklesModel::modelDash9;  break;
                case 10: models = &ManiaKnucklesModel::modelDash10; break;
                case 11: models = &ManiaKnucklesModel::modelDash11; break;
                default: std::fprintf(stdout, "dash animation index out of bounds"); break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 3: //stomp
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ+90, rotRoll-90, limbsScale);
            updateLimbs(3, 100);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 8: //skid
        {
            Vector3f off = currentUpDirection.scaleCopy(limbsScale*displayHeightOffset);
            Vector3f pos = position + off;
            myBody->setBaseOrientation(pos.x, pos.y, pos.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(8, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 11: //hitstun
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(11, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 12: //jump
        {
            scale = 0.27f;
            models = &ManiaKnucklesModel::modelJump;
            updateTransformationMatrix();
            setLimbsVisibility(false);
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
            visible = false;
            break;
        }

        case 15: //jog
        {
            scale = 0.27f;
            int index = (int)(time / 5.55555555f);
            switch (index % 18)
            {
                case 0:  models = &ManiaKnucklesModel::modelJog0;  break;
                case 1:  models = &ManiaKnucklesModel::modelJog1;  break;
                case 2:  models = &ManiaKnucklesModel::modelJog2;  break;
                case 3:  models = &ManiaKnucklesModel::modelJog3;  break;
                case 4:  models = &ManiaKnucklesModel::modelJog4;  break;
                case 5:  models = &ManiaKnucklesModel::modelJog5;  break;
                case 6:  models = &ManiaKnucklesModel::modelJog6;  break;
                case 7:  models = &ManiaKnucklesModel::modelJog7;  break;
                case 8:  models = &ManiaKnucklesModel::modelJog8;  break;
                case 9:  models = &ManiaKnucklesModel::modelJog9;  break;
                case 10: models = &ManiaKnucklesModel::modelJog10; break;
                case 11: models = &ManiaKnucklesModel::modelJog11; break;
                case 12: models = &ManiaKnucklesModel::modelJog12; break;
                case 13: models = &ManiaKnucklesModel::modelJog13; break;
                case 14: models = &ManiaKnucklesModel::modelJog14; break;
                case 15: models = &ManiaKnucklesModel::modelJog15; break;
                case 16: models = &ManiaKnucklesModel::modelJog16; break;
                case 17: models = &ManiaKnucklesModel::modelJog17; break;
                default: std::fprintf(stdout, "warning: jog animation index out of bounds\n"); break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 16: //swim animation
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(15, time);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 17: //swim stroke animation
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(16, time);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 18: //lightdash
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ-90, rotRoll, limbsScale);
            updateLimbs(18, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 19: //die
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(19, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 20: //fly
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(20, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 21: //freefall
        {
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(21, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 22: //punch
        {
            Vector3f off = currentUpDirection.scaleCopy(limbsScale*displayHeightOffset*0.95f);
            Vector3f pos = position + off;
            myBody->setBaseOrientation(pos.x, pos.y, pos.z, rotX, rotY, rotZ, rotRoll, limbsScale*0.95f);
            updateLimbs(22, time);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 23: //climb
        {
            Vector3f off = currentUpDirection.scaleCopy(limbsScale*displayHeightOffset*0.95f);
            Vector3f pos = position + off;
            myBody->setBaseOrientation(pos.x, pos.y, pos.z, rotX, rotY, rotZ, rotRoll, limbsScale*0.95f);
            updateLimbs(23, time);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 25: //grab
        {
            scale = 0.27f;
            models = &ManiaKnucklesModel::modelGrab;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 26: //grind
        {
            scale = 0.27f;
            models = &ManiaKnucklesModel::modelGrind;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        default:
            break;
    }
}

void ManiaKnucklesModel::setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, Vector3f* newUp)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX       = xRot;
    rotY       = yRot;
    rotZ       = zRot;
    rotRoll    = spinRot;
    currentUpDirection.set(newUp);
}

void ManiaKnucklesModel::setBaseColor(float r, float g, float b)
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
}

void ManiaKnucklesModel::setBaseAlpha(float a)
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
}

void ManiaKnucklesModel::setRenderOrderOverride(char newOrder)
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
}

void ManiaKnucklesModel::setBaseVisibility(bool newVisible)
{
    baseVisible = newVisible;
}

std::list<TexturedModel*>* ManiaKnucklesModel::getModels()
{
    return models;
}

void ManiaKnucklesModel::loadStaticModels()
{
    if (ManiaKnucklesModel::modelDash0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading mania knuckles static models...\n");
    #endif

    ObjLoader::ObjLoader::loadModel(&ManiaKnucklesModel::modelBody,         "res/Models/Characters/ManiaKnuckles/", "Body");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelHead,         "res/Models/Characters/ManiaKnuckles/", "Head");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelLeftHumerus,  "res/Models/Characters/ManiaKnuckles/", "Thigh");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelLeftForearm,  "res/Models/Characters/ManiaKnuckles/", "Forearm");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelLeftHand,     "res/Models/Characters/ManiaKnuckles/", "HandLeft");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelLeftThigh,    "res/Models/Characters/ManiaKnuckles/", "Thigh");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelLeftShin,     "res/Models/Characters/ManiaKnuckles/", "Shin");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelLeftFoot,     "res/Models/Characters/ManiaKnuckles/", "FootLeft");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelRightHumerus, "res/Models/Characters/ManiaKnuckles/", "Thigh");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelRightForearm, "res/Models/Characters/ManiaKnuckles/", "Forearm");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelRightHand,    "res/Models/Characters/ManiaKnuckles/", "HandRight");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelRightThigh,   "res/Models/Characters/ManiaKnuckles/", "Thigh");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelRightShin,    "res/Models/Characters/ManiaKnuckles/", "Shin");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelRightFoot,    "res/Models/Characters/ManiaKnuckles/", "FootRight");

    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash0    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash0");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash1    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash1");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash2    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash2");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash3    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash3");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash4    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash4");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash5    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash5");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash6    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash6");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash7    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash7");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash8    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash8");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash9    , "res/Models/Characters/ManiaKnuckles/", "KnuxDash9");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash10   , "res/Models/Characters/ManiaKnuckles/", "KnuxDash10");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelDash11   , "res/Models/Characters/ManiaKnuckles/", "KnuxDash11");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJump     , "res/Models/Characters/ManiaKnuckles/", "KnuxJump");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog0     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog0");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog1     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog1");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog2     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog2");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog3     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog3");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog4     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog4");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog5     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog5");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog6     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog6");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog7     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog7");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog8     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog8");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog9     , "res/Models/Characters/ManiaKnuckles/", "KnuxJog9");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog10    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog10");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog11    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog11");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog12    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog12");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog13    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog13");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog14    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog14");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog15    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog15");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog16    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog16");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelJog17    , "res/Models/Characters/ManiaKnuckles/", "KnuxJog17");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelGrind    , "res/Models/Characters/ManiaSonic/", "Grind");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelStomp    , "res/Models/Characters/ManiaSonic/", "Stomp");
    ObjLoader::loadModel(&ManiaKnucklesModel::modelGrab     , "res/Models/Characters/ManiaKnuckles/", "Grab");
}

void ManiaKnucklesModel::setLimbsVisibility(bool newVisible)
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

void ManiaKnucklesModel::updateLimbs(int animIndex, float time)
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

void ManiaKnucklesModel::updateLimbsMatrix()
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

void ManiaKnucklesModel::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting mania knuckles static models...\n");
    #endif

    Entity::deleteModels(&ManiaKnucklesModel::modelBody);
    Entity::deleteModels(&ManiaKnucklesModel::modelHead);
    Entity::deleteModels(&ManiaKnucklesModel::modelLeftHumerus);
    Entity::deleteModels(&ManiaKnucklesModel::modelLeftForearm);
    Entity::deleteModels(&ManiaKnucklesModel::modelLeftHand);
    Entity::deleteModels(&ManiaKnucklesModel::modelLeftThigh);
    Entity::deleteModels(&ManiaKnucklesModel::modelLeftShin);
    Entity::deleteModels(&ManiaKnucklesModel::modelLeftFoot);
    Entity::deleteModels(&ManiaKnucklesModel::modelRightHumerus);
    Entity::deleteModels(&ManiaKnucklesModel::modelRightForearm);
    Entity::deleteModels(&ManiaKnucklesModel::modelRightHand);
    Entity::deleteModels(&ManiaKnucklesModel::modelRightThigh);
    Entity::deleteModels(&ManiaKnucklesModel::modelRightShin);
    Entity::deleteModels(&ManiaKnucklesModel::modelRightFoot);

    Entity::deleteModels(&ManiaKnucklesModel::modelDash0);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash1);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash2);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash3);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash4);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash5);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash6);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash7);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash8);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash9);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash10);
    Entity::deleteModels(&ManiaKnucklesModel::modelDash11);
    Entity::deleteModels(&ManiaKnucklesModel::modelJump);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog0);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog1);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog2);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog3);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog4);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog5);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog6);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog7);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog8);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog9);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog10);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog11);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog12);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog13);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog14);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog15);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog16);
    Entity::deleteModels(&ManiaKnucklesModel::modelJog17);
    Entity::deleteModels(&ManiaKnucklesModel::modelGrind);
    Entity::deleteModels(&ManiaKnucklesModel::modelStomp);
    Entity::deleteModels(&ManiaKnucklesModel::modelGrab);
}
