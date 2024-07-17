#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "supersonicmodel.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../entities/playersonic.hpp"
#include "../../toolbox/maths.hpp"
#include "../../animation/body.hpp"
#include "../../animation/limb.hpp"
#include "../../animation/animationresources.hpp"
#include "../../particles/particletexture.hpp"
#include "../../particles/particleresources.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> SuperSonicModel::modelBody;
std::vector<TexturedModel*> SuperSonicModel::modelHead;
std::vector<TexturedModel*> SuperSonicModel::modelLeftHumerus;
std::vector<TexturedModel*> SuperSonicModel::modelLeftForearm;
std::vector<TexturedModel*> SuperSonicModel::modelLeftHand;
std::vector<TexturedModel*> SuperSonicModel::modelLeftThigh;
std::vector<TexturedModel*> SuperSonicModel::modelLeftShin;
std::vector<TexturedModel*> SuperSonicModel::modelLeftFoot;
std::vector<TexturedModel*> SuperSonicModel::modelRightHumerus;
std::vector<TexturedModel*> SuperSonicModel::modelRightForearm;
std::vector<TexturedModel*> SuperSonicModel::modelRightHand;
std::vector<TexturedModel*> SuperSonicModel::modelRightThigh;
std::vector<TexturedModel*> SuperSonicModel::modelRightShin;
std::vector<TexturedModel*> SuperSonicModel::modelRightFoot;

std::vector<TexturedModel*> SuperSonicModel::modelDash0;
std::vector<TexturedModel*> SuperSonicModel::modelDash1;
std::vector<TexturedModel*> SuperSonicModel::modelDash2;
std::vector<TexturedModel*> SuperSonicModel::modelDash3;
std::vector<TexturedModel*> SuperSonicModel::modelDash4;
std::vector<TexturedModel*> SuperSonicModel::modelDash5;
std::vector<TexturedModel*> SuperSonicModel::modelDash6;
std::vector<TexturedModel*> SuperSonicModel::modelDash7;
std::vector<TexturedModel*> SuperSonicModel::modelDash8;
std::vector<TexturedModel*> SuperSonicModel::modelDash9;
std::vector<TexturedModel*> SuperSonicModel::modelDash10;
std::vector<TexturedModel*> SuperSonicModel::modelDash11;
std::vector<TexturedModel*> SuperSonicModel::modelJump;
std::vector<TexturedModel*> SuperSonicModel::modelJog0;
std::vector<TexturedModel*> SuperSonicModel::modelJog1;
std::vector<TexturedModel*> SuperSonicModel::modelJog2;
std::vector<TexturedModel*> SuperSonicModel::modelJog3;
std::vector<TexturedModel*> SuperSonicModel::modelJog4;
std::vector<TexturedModel*> SuperSonicModel::modelJog5;
std::vector<TexturedModel*> SuperSonicModel::modelJog6;
std::vector<TexturedModel*> SuperSonicModel::modelJog7;
std::vector<TexturedModel*> SuperSonicModel::modelJog8;
std::vector<TexturedModel*> SuperSonicModel::modelJog9;
std::vector<TexturedModel*> SuperSonicModel::modelJog10;
std::vector<TexturedModel*> SuperSonicModel::modelJog11;
std::vector<TexturedModel*> SuperSonicModel::modelJog12;
std::vector<TexturedModel*> SuperSonicModel::modelJog13;
std::vector<TexturedModel*> SuperSonicModel::modelJog14;
std::vector<TexturedModel*> SuperSonicModel::modelJog15;
std::vector<TexturedModel*> SuperSonicModel::modelJog16;
std::vector<TexturedModel*> SuperSonicModel::modelJog17;
std::vector<TexturedModel*> SuperSonicModel::modelGrind;
std::vector<TexturedModel*> SuperSonicModel::modelStomp;
std::vector<TexturedModel*> SuperSonicModel::modelGrab;

SuperSonicModel::SuperSonicModel()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 0.27f;
    visible = false;
    models = &SuperSonicModel::modelDash0;

    myBody =         new Body(&modelBody); INCR_NEW("Entity")
    myHead =         new Limb(&modelHead,         1.2f,  -0.3f,   0,     myBody,   nullptr);        INCR_NEW("Entity")
    myLeftHumerus =  new Limb(&modelLeftHumerus,  0.9f,   0,     -0.9f,  myBody,   nullptr);        INCR_NEW("Entity")
    myLeftForearm =  new Limb(&modelLeftForearm,  0,     -1.3f,   0,     nullptr,  myLeftHumerus);  INCR_NEW("Entity")
    myLeftHand =     new Limb(&modelLeftHand,     0,     -1.3f,   0,     nullptr,  myLeftForearm);  INCR_NEW("Entity")
    myLeftThigh =    new Limb(&modelLeftThigh,   -0.9f,   0,     -0.3f,  myBody,   nullptr);        INCR_NEW("Entity")
    myLeftShin =     new Limb(&modelLeftShin,     0,     -1.3f,   0,     nullptr,  myLeftThigh);    INCR_NEW("Entity")
    myLeftFoot =     new Limb(&modelLeftFoot,     0,     -1.1f,   0,     nullptr,  myLeftShin);     INCR_NEW("Entity")
    myRightHumerus = new Limb(&modelRightHumerus, 0.9f,   0,      0.9f,  myBody,   nullptr);        INCR_NEW("Entity")
    myRightForearm = new Limb(&modelRightForearm, 0,     -1.3f,   0,     nullptr,  myRightHumerus); INCR_NEW("Entity")
    myRightHand =    new Limb(&modelRightHand,    0,     -1.3f,   0,     nullptr,  myRightForearm); INCR_NEW("Entity")
    myRightThigh =   new Limb(&modelRightThigh,  -0.9f,   0,      0.3f,  myBody,   nullptr);        INCR_NEW("Entity")
    myRightShin =    new Limb(&modelRightShin,    0,     -1.3f,   0,     nullptr,  myRightThigh);   INCR_NEW("Entity")
    myRightFoot =    new Limb(&modelRightFoot,    0,     -1.1f,   0,     nullptr,  myRightShin);    INCR_NEW("Entity")

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

void SuperSonicModel::step()
{

}

float SuperSonicModel::getDisplayBallOffset() const
{
    return displayBallOffset;
}

ParticleTexture* SuperSonicModel::getBallTexture()
{
    return ParticleResources::textureYellowTrail;
}

void SuperSonicModel::animate(int animIndex, float time)
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
            visible = false;
            break;
        }

        case 1: //run
        {
            scale = 0.27f;
            int index = (int)(time / 8.3333333f);
            switch (index % 12)
            {
                case  0: models = &SuperSonicModel::modelDash0;  break;
                case  1: models = &SuperSonicModel::modelDash1;  break;
                case  2: models = &SuperSonicModel::modelDash2;  break;
                case  3: models = &SuperSonicModel::modelDash3;  break;
                case  4: models = &SuperSonicModel::modelDash4;  break;
                case  5: models = &SuperSonicModel::modelDash5;  break;
                case  6: models = &SuperSonicModel::modelDash6;  break;
                case  7: models = &SuperSonicModel::modelDash7;  break;
                case  8: models = &SuperSonicModel::modelDash8;  break;
                case  9: models = &SuperSonicModel::modelDash9;  break;
                case 10: models = &SuperSonicModel::modelDash10; break;
                case 11: models = &SuperSonicModel::modelDash11; break;
                default: printf("dash animation index out of bounds"); break;
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
            models = &SuperSonicModel::modelJump;
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
                case 0:  models = &SuperSonicModel::modelJog0;  break;
                case 1:  models = &SuperSonicModel::modelJog1;  break;
                case 2:  models = &SuperSonicModel::modelJog2;  break;
                case 3:  models = &SuperSonicModel::modelJog3;  break;
                case 4:  models = &SuperSonicModel::modelJog4;  break;
                case 5:  models = &SuperSonicModel::modelJog5;  break;
                case 6:  models = &SuperSonicModel::modelJog6;  break;
                case 7:  models = &SuperSonicModel::modelJog7;  break;
                case 8:  models = &SuperSonicModel::modelJog8;  break;
                case 9:  models = &SuperSonicModel::modelJog9;  break;
                case 10: models = &SuperSonicModel::modelJog10; break;
                case 11: models = &SuperSonicModel::modelJog11; break;
                case 12: models = &SuperSonicModel::modelJog12; break;
                case 13: models = &SuperSonicModel::modelJog13; break;
                case 14: models = &SuperSonicModel::modelJog14; break;
                case 15: models = &SuperSonicModel::modelJog15; break;
                case 16: models = &SuperSonicModel::modelJog16; break;
                case 17: models = &SuperSonicModel::modelJog17; break;
                default: printf("warning: jog animation index out of bounds\n"); break;
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
            models = &SuperSonicModel::modelGrab;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 26: //grind
        {
            scale = 0.27f;
            models = &SuperSonicModel::modelGrind;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        default:
            break;
    }
}

void SuperSonicModel::setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, const Vector3f* newUp)
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

void SuperSonicModel::setBaseColor(float r, float g, float b)
{
                    baseColor.set(r, g, b);
    myBody        ->baseColor.set(r, g, b);
    myHead        ->baseColor.set(r, g, b);
    myLeftHumerus ->baseColor.set(r, g, b);
    myLeftForearm ->baseColor.set(r, g, b);
    myLeftHand    ->baseColor.set(r, g, b);
    myLeftThigh   ->baseColor.set(r, g, b);
    myLeftShin    ->baseColor.set(r, g, b);
    myLeftFoot    ->baseColor.set(r, g, b);
    myRightHumerus->baseColor.set(r, g, b);
    myRightForearm->baseColor.set(r, g, b);
    myRightHand   ->baseColor.set(r, g, b);
    myRightThigh  ->baseColor.set(r, g, b);
    myRightShin   ->baseColor.set(r, g, b);
    myRightFoot   ->baseColor.set(r, g, b);
}

void SuperSonicModel::setBaseAlpha(float a)
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

void SuperSonicModel::setRenderOrderOverride(char newOrder)
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

void SuperSonicModel::setBaseVisibility(bool newVisible)
{
    baseVisible = newVisible;
}

std::vector<TexturedModel*>* SuperSonicModel::getModels()
{
    return models;
}

void SuperSonicModel::loadStaticModels()
{
    if (SuperSonicModel::modelDash0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SuperSonicModel static models...\n");
    #endif

    ModelLoader::loadModel(&SuperSonicModel::modelBody,         "res/Models/Characters/SuperSonic/", "Body");
    ModelLoader::loadModel(&SuperSonicModel::modelHead,         "res/Models/Characters/SuperSonic/", "Head");
    ModelLoader::loadModel(&SuperSonicModel::modelLeftHumerus,  "res/Models/Characters/SuperSonic/", "Humerus");
    ModelLoader::loadModel(&SuperSonicModel::modelLeftForearm,  "res/Models/Characters/SuperSonic/", "Forearm");
    ModelLoader::loadModel(&SuperSonicModel::modelLeftHand,     "res/Models/Characters/SuperSonic/", "HandLeft");
    ModelLoader::loadModel(&SuperSonicModel::modelLeftThigh,    "res/Models/Characters/SuperSonic/", "Thigh");
    ModelLoader::loadModel(&SuperSonicModel::modelLeftShin,     "res/Models/Characters/SuperSonic/", "Shin");
    ModelLoader::loadModel(&SuperSonicModel::modelLeftFoot,     "res/Models/Characters/SuperSonic/", "ShoeLeft");
    ModelLoader::loadModel(&SuperSonicModel::modelRightHumerus, "res/Models/Characters/SuperSonic/", "Humerus");
    ModelLoader::loadModel(&SuperSonicModel::modelRightForearm, "res/Models/Characters/SuperSonic/", "Forearm");
    ModelLoader::loadModel(&SuperSonicModel::modelRightHand,    "res/Models/Characters/SuperSonic/", "HandRight");
    ModelLoader::loadModel(&SuperSonicModel::modelRightThigh,   "res/Models/Characters/SuperSonic/", "Thigh");
    ModelLoader::loadModel(&SuperSonicModel::modelRightShin,    "res/Models/Characters/SuperSonic/", "Shin");
    ModelLoader::loadModel(&SuperSonicModel::modelRightFoot,    "res/Models/Characters/SuperSonic/", "ShoeRight");

    ModelLoader::loadModel(&SuperSonicModel::modelDash0    , "res/Models/Characters/SuperSonic/", "Dash0");
    ModelLoader::loadModel(&SuperSonicModel::modelDash1    , "res/Models/Characters/SuperSonic/", "Dash1");
    ModelLoader::loadModel(&SuperSonicModel::modelDash2    , "res/Models/Characters/SuperSonic/", "Dash2");
    ModelLoader::loadModel(&SuperSonicModel::modelDash3    , "res/Models/Characters/SuperSonic/", "Dash3");
    ModelLoader::loadModel(&SuperSonicModel::modelDash4    , "res/Models/Characters/SuperSonic/", "Dash4");
    ModelLoader::loadModel(&SuperSonicModel::modelDash5    , "res/Models/Characters/SuperSonic/", "Dash5");
    ModelLoader::loadModel(&SuperSonicModel::modelDash6    , "res/Models/Characters/SuperSonic/", "Dash6");
    ModelLoader::loadModel(&SuperSonicModel::modelDash7    , "res/Models/Characters/SuperSonic/", "Dash7");
    ModelLoader::loadModel(&SuperSonicModel::modelDash8    , "res/Models/Characters/SuperSonic/", "Dash8");
    ModelLoader::loadModel(&SuperSonicModel::modelDash9    , "res/Models/Characters/SuperSonic/", "Dash9");
    ModelLoader::loadModel(&SuperSonicModel::modelDash10   , "res/Models/Characters/SuperSonic/", "Dash10");
    ModelLoader::loadModel(&SuperSonicModel::modelDash11   , "res/Models/Characters/SuperSonic/", "Dash11");
    ModelLoader::loadModel(&SuperSonicModel::modelJump     , "res/Models/Characters/SuperSonic/", "Jump");
    ModelLoader::loadModel(&SuperSonicModel::modelJog0     , "res/Models/Characters/SuperSonic/", "Jog0");
    ModelLoader::loadModel(&SuperSonicModel::modelJog1     , "res/Models/Characters/SuperSonic/", "Jog1");
    ModelLoader::loadModel(&SuperSonicModel::modelJog2     , "res/Models/Characters/SuperSonic/", "Jog2");
    ModelLoader::loadModel(&SuperSonicModel::modelJog3     , "res/Models/Characters/SuperSonic/", "Jog3");
    ModelLoader::loadModel(&SuperSonicModel::modelJog4     , "res/Models/Characters/SuperSonic/", "Jog4");
    ModelLoader::loadModel(&SuperSonicModel::modelJog5     , "res/Models/Characters/SuperSonic/", "Jog5");
    ModelLoader::loadModel(&SuperSonicModel::modelJog6     , "res/Models/Characters/SuperSonic/", "Jog6");
    ModelLoader::loadModel(&SuperSonicModel::modelJog7     , "res/Models/Characters/SuperSonic/", "Jog7");
    ModelLoader::loadModel(&SuperSonicModel::modelJog8     , "res/Models/Characters/SuperSonic/", "Jog8");
    ModelLoader::loadModel(&SuperSonicModel::modelJog9     , "res/Models/Characters/SuperSonic/", "Jog9");
    ModelLoader::loadModel(&SuperSonicModel::modelJog10    , "res/Models/Characters/SuperSonic/", "Jog10");
    ModelLoader::loadModel(&SuperSonicModel::modelJog11    , "res/Models/Characters/SuperSonic/", "Jog11");
    ModelLoader::loadModel(&SuperSonicModel::modelJog12    , "res/Models/Characters/SuperSonic/", "Jog12");
    ModelLoader::loadModel(&SuperSonicModel::modelJog13    , "res/Models/Characters/SuperSonic/", "Jog13");
    ModelLoader::loadModel(&SuperSonicModel::modelJog14    , "res/Models/Characters/SuperSonic/", "Jog14");
    ModelLoader::loadModel(&SuperSonicModel::modelJog15    , "res/Models/Characters/SuperSonic/", "Jog15");
    ModelLoader::loadModel(&SuperSonicModel::modelJog16    , "res/Models/Characters/SuperSonic/", "Jog16");
    ModelLoader::loadModel(&SuperSonicModel::modelJog17    , "res/Models/Characters/SuperSonic/", "Jog17");
    ModelLoader::loadModel(&SuperSonicModel::modelGrind    , "res/Models/Characters/SuperSonic/", "Grind");
    ModelLoader::loadModel(&SuperSonicModel::modelStomp    , "res/Models/Characters/SuperSonic/", "Stomp");
    ModelLoader::loadModel(&SuperSonicModel::modelGrab     , "res/Models/Characters/SuperSonic/", "Grab");
}

void SuperSonicModel::setLimbsVisibility(bool newVisible)
{
    myBody->visible = (newVisible);
    myHead->visible = (newVisible);
    myLeftHumerus->visible = (newVisible);
    myLeftForearm->visible = (newVisible);
    myLeftHand->visible = (newVisible);
    myLeftThigh->visible = (newVisible);
    myLeftShin->visible = (newVisible);
    myLeftFoot->visible = (newVisible);
    myRightHumerus->visible = (newVisible);
    myRightForearm->visible = (newVisible);
    myRightHand->visible = (newVisible);
    myRightThigh->visible = (newVisible);
    myRightShin->visible = (newVisible);
    myRightFoot->visible = (newVisible);
}

void SuperSonicModel::updateLimbs(int animIndex, float time)
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

void SuperSonicModel::updateLimbsMatrix()
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

void SuperSonicModel::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SuperSonicModel static models...\n");
    #endif

    Entity::deleteModels(&SuperSonicModel::modelBody);
    Entity::deleteModels(&SuperSonicModel::modelHead);
    Entity::deleteModels(&SuperSonicModel::modelLeftHumerus);
    Entity::deleteModels(&SuperSonicModel::modelLeftForearm);
    Entity::deleteModels(&SuperSonicModel::modelLeftHand);
    Entity::deleteModels(&SuperSonicModel::modelLeftThigh);
    Entity::deleteModels(&SuperSonicModel::modelLeftShin);
    Entity::deleteModels(&SuperSonicModel::modelLeftFoot);
    Entity::deleteModels(&SuperSonicModel::modelRightHumerus);
    Entity::deleteModels(&SuperSonicModel::modelRightForearm);
    Entity::deleteModels(&SuperSonicModel::modelRightHand);
    Entity::deleteModels(&SuperSonicModel::modelRightThigh);
    Entity::deleteModels(&SuperSonicModel::modelRightShin);
    Entity::deleteModels(&SuperSonicModel::modelRightFoot);

    Entity::deleteModels(&SuperSonicModel::modelDash0);
    Entity::deleteModels(&SuperSonicModel::modelDash1);
    Entity::deleteModels(&SuperSonicModel::modelDash2);
    Entity::deleteModels(&SuperSonicModel::modelDash3);
    Entity::deleteModels(&SuperSonicModel::modelDash4);
    Entity::deleteModels(&SuperSonicModel::modelDash5);
    Entity::deleteModels(&SuperSonicModel::modelDash6);
    Entity::deleteModels(&SuperSonicModel::modelDash7);
    Entity::deleteModels(&SuperSonicModel::modelDash8);
    Entity::deleteModels(&SuperSonicModel::modelDash9);
    Entity::deleteModels(&SuperSonicModel::modelDash10);
    Entity::deleteModels(&SuperSonicModel::modelDash11);
    Entity::deleteModels(&SuperSonicModel::modelJump);
    Entity::deleteModels(&SuperSonicModel::modelJog0);
    Entity::deleteModels(&SuperSonicModel::modelJog1);
    Entity::deleteModels(&SuperSonicModel::modelJog2);
    Entity::deleteModels(&SuperSonicModel::modelJog3);
    Entity::deleteModels(&SuperSonicModel::modelJog4);
    Entity::deleteModels(&SuperSonicModel::modelJog5);
    Entity::deleteModels(&SuperSonicModel::modelJog6);
    Entity::deleteModels(&SuperSonicModel::modelJog7);
    Entity::deleteModels(&SuperSonicModel::modelJog8);
    Entity::deleteModels(&SuperSonicModel::modelJog9);
    Entity::deleteModels(&SuperSonicModel::modelJog10);
    Entity::deleteModels(&SuperSonicModel::modelJog11);
    Entity::deleteModels(&SuperSonicModel::modelJog12);
    Entity::deleteModels(&SuperSonicModel::modelJog13);
    Entity::deleteModels(&SuperSonicModel::modelJog14);
    Entity::deleteModels(&SuperSonicModel::modelJog15);
    Entity::deleteModels(&SuperSonicModel::modelJog16);
    Entity::deleteModels(&SuperSonicModel::modelJog17);
    Entity::deleteModels(&SuperSonicModel::modelGrind);
    Entity::deleteModels(&SuperSonicModel::modelStomp);
    Entity::deleteModels(&SuperSonicModel::modelGrab);
}
