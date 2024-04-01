#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "maniasonicmodel.hpp"
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

std::vector<TexturedModel*> ManiaSonicModel::modelBody;
std::vector<TexturedModel*> ManiaSonicModel::modelHead;
std::vector<TexturedModel*> ManiaSonicModel::modelLeftHumerus;
std::vector<TexturedModel*> ManiaSonicModel::modelLeftForearm;
std::vector<TexturedModel*> ManiaSonicModel::modelLeftHand;
std::vector<TexturedModel*> ManiaSonicModel::modelLeftThigh;
std::vector<TexturedModel*> ManiaSonicModel::modelLeftShin;
std::vector<TexturedModel*> ManiaSonicModel::modelLeftFoot;
std::vector<TexturedModel*> ManiaSonicModel::modelRightHumerus;
std::vector<TexturedModel*> ManiaSonicModel::modelRightForearm;
std::vector<TexturedModel*> ManiaSonicModel::modelRightHand;
std::vector<TexturedModel*> ManiaSonicModel::modelRightThigh;
std::vector<TexturedModel*> ManiaSonicModel::modelRightShin;
std::vector<TexturedModel*> ManiaSonicModel::modelRightFoot;

std::vector<TexturedModel*> ManiaSonicModel::modelDash0;
std::vector<TexturedModel*> ManiaSonicModel::modelDash1;
std::vector<TexturedModel*> ManiaSonicModel::modelDash2;
std::vector<TexturedModel*> ManiaSonicModel::modelDash3;
std::vector<TexturedModel*> ManiaSonicModel::modelDash4;
std::vector<TexturedModel*> ManiaSonicModel::modelDash5;
std::vector<TexturedModel*> ManiaSonicModel::modelDash6;
std::vector<TexturedModel*> ManiaSonicModel::modelDash7;
std::vector<TexturedModel*> ManiaSonicModel::modelDash8;
std::vector<TexturedModel*> ManiaSonicModel::modelDash9;
std::vector<TexturedModel*> ManiaSonicModel::modelDash10;
std::vector<TexturedModel*> ManiaSonicModel::modelDash11;
std::vector<TexturedModel*> ManiaSonicModel::modelJump;
std::vector<TexturedModel*> ManiaSonicModel::modelJog0;
std::vector<TexturedModel*> ManiaSonicModel::modelJog1;
std::vector<TexturedModel*> ManiaSonicModel::modelJog2;
std::vector<TexturedModel*> ManiaSonicModel::modelJog3;
std::vector<TexturedModel*> ManiaSonicModel::modelJog4;
std::vector<TexturedModel*> ManiaSonicModel::modelJog5;
std::vector<TexturedModel*> ManiaSonicModel::modelJog6;
std::vector<TexturedModel*> ManiaSonicModel::modelJog7;
std::vector<TexturedModel*> ManiaSonicModel::modelJog8;
std::vector<TexturedModel*> ManiaSonicModel::modelJog9;
std::vector<TexturedModel*> ManiaSonicModel::modelJog10;
std::vector<TexturedModel*> ManiaSonicModel::modelJog11;
std::vector<TexturedModel*> ManiaSonicModel::modelJog12;
std::vector<TexturedModel*> ManiaSonicModel::modelJog13;
std::vector<TexturedModel*> ManiaSonicModel::modelJog14;
std::vector<TexturedModel*> ManiaSonicModel::modelJog15;
std::vector<TexturedModel*> ManiaSonicModel::modelJog16;
std::vector<TexturedModel*> ManiaSonicModel::modelJog17;
std::vector<TexturedModel*> ManiaSonicModel::modelGrind;
std::vector<TexturedModel*> ManiaSonicModel::modelSkid;
std::vector<TexturedModel*> ManiaSonicModel::modelLightdash;
std::vector<TexturedModel*> ManiaSonicModel::modelFreefall;
std::vector<TexturedModel*> ManiaSonicModel::modelStomp;
std::vector<TexturedModel*> ManiaSonicModel::modelGrab;
std::vector<TexturedModel*> ManiaSonicModel::modelDrive;

ManiaSonicModel::ManiaSonicModel()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 0.27f;
    visible = false;
    models = &ManiaSonicModel::modelDash0;

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

void ManiaSonicModel::step()
{

}

float ManiaSonicModel::getDisplayBallOffset() const
{
    return displayBallOffset;
}

ParticleTexture* ManiaSonicModel::getBallTexture()
{
    return ParticleResources::textureLightBlueTrail;
}

void ManiaSonicModel::animate(int animIndex, float time)
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
            scale = (0.27f);
            int index = (int)(time/8.3333333f);
            switch (index % 12)
            {
                case  0: models = &ManiaSonicModel::modelDash0;  break;
                case  1: models = &ManiaSonicModel::modelDash1;  break;
                case  2: models = &ManiaSonicModel::modelDash2;  break;
                case  3: models = &ManiaSonicModel::modelDash3;  break;
                case  4: models = &ManiaSonicModel::modelDash4;  break;
                case  5: models = &ManiaSonicModel::modelDash5;  break;
                case  6: models = &ManiaSonicModel::modelDash6;  break;
                case  7: models = &ManiaSonicModel::modelDash7;  break;
                case  8: models = &ManiaSonicModel::modelDash8;  break;
                case  9: models = &ManiaSonicModel::modelDash9;  break;
                case 10: models = &ManiaSonicModel::modelDash10; break;
                case 11: models = &ManiaSonicModel::modelDash11; break;
                default: printf("dash animation index out of bounds"); break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 3: //stomp
        {
            scale = (0.27f);
            models = &ManiaSonicModel::modelStomp;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 8: //skid
        {
            scale = (0.27f);
            models = &ManiaSonicModel::modelSkid;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 11: //hitstun
        {
            updateLimbs(11, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 12: //jump
        {
            scale = (0.32f);
            models = &ManiaSonicModel::modelJump;
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
            scale = (0.27f);
            int index = (int)(time / 5.55555555f);
            switch (index % 18)
            {
                case 0:  models = &ManiaSonicModel::modelJog0;  break;
                case 1:  models = &ManiaSonicModel::modelJog1;  break;
                case 2:  models = &ManiaSonicModel::modelJog2;  break;
                case 3:  models = &ManiaSonicModel::modelJog3;  break;
                case 4:  models = &ManiaSonicModel::modelJog4;  break;
                case 5:  models = &ManiaSonicModel::modelJog5;  break;
                case 6:  models = &ManiaSonicModel::modelJog6;  break;
                case 7:  models = &ManiaSonicModel::modelJog7;  break;
                case 8:  models = &ManiaSonicModel::modelJog8;  break;
                case 9:  models = &ManiaSonicModel::modelJog9;  break;
                case 10: models = &ManiaSonicModel::modelJog10; break;
                case 11: models = &ManiaSonicModel::modelJog11; break;
                case 12: models = &ManiaSonicModel::modelJog12; break;
                case 13: models = &ManiaSonicModel::modelJog13; break;
                case 14: models = &ManiaSonicModel::modelJog14; break;
                case 15: models = &ManiaSonicModel::modelJog15; break;
                case 16: models = &ManiaSonicModel::modelJog16; break;
                case 17: models = &ManiaSonicModel::modelJog17; break;
                default: printf("warning: jog animation index out of bounds\n"); break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 18: //lightdash
        {
            scale = (0.27f);
            models = &ManiaSonicModel::modelLightdash;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 19: //die
        {
            updateLimbs(19, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            visible = false;
            break;
        }

        case 21: //freefall
        {
            scale = (0.27f);
            models = &ManiaSonicModel::modelFreefall;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 25: //grab
        {
            scale = (0.27f);
            models = &ManiaSonicModel::modelGrab;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 26: //grind
        {
            scale = (0.27f);
            models = &ManiaSonicModel::modelGrind;
            updateTransformationMatrixYXZY();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        case 27: //drive
        {
            scale = (1.0f);
            models = &ManiaSonicModel::modelDrive;
            updateTransformationMatrixYXZY();
            setLimbsVisibility(false);
            visible = baseVisible;
            break;
        }

        default:
            break;
    }
}

void ManiaSonicModel::setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, const Vector3f* newUp)
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
}

void ManiaSonicModel::setBaseColor(float r, float g, float b)
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

void ManiaSonicModel::setBaseAlpha(float a)
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

void ManiaSonicModel::setRenderOrderOverride(char newOrder)
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

void ManiaSonicModel::setBaseVisibility(bool newVisible)
{
    baseVisible = newVisible;
}

std::vector<TexturedModel*>* ManiaSonicModel::getModels()
{
    return models;
}

void ManiaSonicModel::loadStaticModels()
{
    if (ManiaSonicModel::modelDash0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading ManiaSonicModel static models...\n");
    #endif

    ModelLoader::loadModel(&ManiaSonicModel::modelBody        , "res/Models/Characters/ManiaSonic/", "Body");
    ModelLoader::loadModel(&ManiaSonicModel::modelHead        , "res/Models/Characters/ManiaSonic/", "Head");
    ModelLoader::loadModel(&ManiaSonicModel::modelLeftHumerus , "res/Models/Characters/ManiaSonic/", "Humerus");
    ModelLoader::loadModel(&ManiaSonicModel::modelLeftForearm , "res/Models/Characters/ManiaSonic/", "Forearm");
    ModelLoader::loadModel(&ManiaSonicModel::modelLeftHand    , "res/Models/Characters/ManiaSonic/", "HandLeft");
    ModelLoader::loadModel(&ManiaSonicModel::modelLeftThigh   , "res/Models/Characters/ManiaSonic/", "Thigh");
    ModelLoader::loadModel(&ManiaSonicModel::modelLeftShin    , "res/Models/Characters/ManiaSonic/", "Shin");
    ModelLoader::loadModel(&ManiaSonicModel::modelLeftFoot    , "res/Models/Characters/ManiaSonic/", "ShoeLeft");
    ModelLoader::loadModel(&ManiaSonicModel::modelRightHumerus, "res/Models/Characters/ManiaSonic/", "Humerus");
    ModelLoader::loadModel(&ManiaSonicModel::modelRightForearm, "res/Models/Characters/ManiaSonic/", "Forearm");
    ModelLoader::loadModel(&ManiaSonicModel::modelRightHand   , "res/Models/Characters/ManiaSonic/", "HandRight");
    ModelLoader::loadModel(&ManiaSonicModel::modelRightThigh  , "res/Models/Characters/ManiaSonic/", "Thigh");
    ModelLoader::loadModel(&ManiaSonicModel::modelRightShin   , "res/Models/Characters/ManiaSonic/", "Shin");
    ModelLoader::loadModel(&ManiaSonicModel::modelRightFoot   , "res/Models/Characters/ManiaSonic/", "ShoeRight");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash0       , "res/Models/Characters/ManiaSonic/", "Dash0");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash1       , "res/Models/Characters/ManiaSonic/", "Dash1");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash2       , "res/Models/Characters/ManiaSonic/", "Dash2");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash3       , "res/Models/Characters/ManiaSonic/", "Dash3");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash4       , "res/Models/Characters/ManiaSonic/", "Dash4");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash5       , "res/Models/Characters/ManiaSonic/", "Dash5");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash6       , "res/Models/Characters/ManiaSonic/", "Dash6");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash7       , "res/Models/Characters/ManiaSonic/", "Dash7");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash8       , "res/Models/Characters/ManiaSonic/", "Dash8");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash9       , "res/Models/Characters/ManiaSonic/", "Dash9");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash10      , "res/Models/Characters/ManiaSonic/", "Dash10");
    ModelLoader::loadModel(&ManiaSonicModel::modelDash11      , "res/Models/Characters/ManiaSonic/", "Dash11");
    ModelLoader::loadModel(&ManiaSonicModel::modelJump        , "res/Models/Characters/ManiaSonic/", "Jump");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog0        , "res/Models/Characters/ManiaSonic/", "Jog0");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog1        , "res/Models/Characters/ManiaSonic/", "Jog1");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog2        , "res/Models/Characters/ManiaSonic/", "Jog2");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog3        , "res/Models/Characters/ManiaSonic/", "Jog3");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog4        , "res/Models/Characters/ManiaSonic/", "Jog4");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog5        , "res/Models/Characters/ManiaSonic/", "Jog5");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog6        , "res/Models/Characters/ManiaSonic/", "Jog6");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog7        , "res/Models/Characters/ManiaSonic/", "Jog7");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog8        , "res/Models/Characters/ManiaSonic/", "Jog8");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog9        , "res/Models/Characters/ManiaSonic/", "Jog9");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog10       , "res/Models/Characters/ManiaSonic/", "Jog10");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog11       , "res/Models/Characters/ManiaSonic/", "Jog11");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog12       , "res/Models/Characters/ManiaSonic/", "Jog12");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog13       , "res/Models/Characters/ManiaSonic/", "Jog13");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog14       , "res/Models/Characters/ManiaSonic/", "Jog14");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog15       , "res/Models/Characters/ManiaSonic/", "Jog15");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog16       , "res/Models/Characters/ManiaSonic/", "Jog16");
    ModelLoader::loadModel(&ManiaSonicModel::modelJog17       , "res/Models/Characters/ManiaSonic/", "Jog17");
    ModelLoader::loadModel(&ManiaSonicModel::modelGrind       , "res/Models/Characters/ManiaSonic/", "Grind");
    ModelLoader::loadModel(&ManiaSonicModel::modelSkid        , "res/Models/Characters/ManiaSonic/", "Skid");
    ModelLoader::loadModel(&ManiaSonicModel::modelLightdash   , "res/Models/Characters/ManiaSonic/", "Lightdash");
    ModelLoader::loadModel(&ManiaSonicModel::modelFreefall    , "res/Models/Characters/ManiaSonic/", "Freefall");
    ModelLoader::loadModel(&ManiaSonicModel::modelStomp       , "res/Models/Characters/ManiaSonic/", "Stomp");
    ModelLoader::loadModel(&ManiaSonicModel::modelGrab        , "res/Models/Characters/ManiaSonic/", "Grab");
    ModelLoader::loadModel(&ManiaSonicModel::modelDrive       , "res/Models/Characters/ManiaSonic/", "Drive");
}

void ManiaSonicModel::setLimbsVisibility(bool newVisible)
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

void ManiaSonicModel::updateLimbs(int animIndex, float time)
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

void ManiaSonicModel::updateLimbsMatrix()
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

void ManiaSonicModel::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting ManiaSonicModel static models...\n");
    #endif

    Entity::deleteModels(&ManiaSonicModel::modelBody);
    Entity::deleteModels(&ManiaSonicModel::modelHead);
    Entity::deleteModels(&ManiaSonicModel::modelLeftHumerus);
    Entity::deleteModels(&ManiaSonicModel::modelLeftForearm);
    Entity::deleteModels(&ManiaSonicModel::modelLeftHand);
    Entity::deleteModels(&ManiaSonicModel::modelLeftThigh);
    Entity::deleteModels(&ManiaSonicModel::modelLeftShin);
    Entity::deleteModels(&ManiaSonicModel::modelLeftFoot);
    Entity::deleteModels(&ManiaSonicModel::modelRightHumerus);
    Entity::deleteModels(&ManiaSonicModel::modelRightForearm);
    Entity::deleteModels(&ManiaSonicModel::modelRightHand);
    Entity::deleteModels(&ManiaSonicModel::modelRightThigh);
    Entity::deleteModels(&ManiaSonicModel::modelRightShin);
    Entity::deleteModels(&ManiaSonicModel::modelRightFoot);

    Entity::deleteModels(&ManiaSonicModel::modelDash0);
    Entity::deleteModels(&ManiaSonicModel::modelDash1);
    Entity::deleteModels(&ManiaSonicModel::modelDash2);
    Entity::deleteModels(&ManiaSonicModel::modelDash3);
    Entity::deleteModels(&ManiaSonicModel::modelDash4);
    Entity::deleteModels(&ManiaSonicModel::modelDash5);
    Entity::deleteModels(&ManiaSonicModel::modelDash6);
    Entity::deleteModels(&ManiaSonicModel::modelDash7);
    Entity::deleteModels(&ManiaSonicModel::modelDash8);
    Entity::deleteModels(&ManiaSonicModel::modelDash9);
    Entity::deleteModels(&ManiaSonicModel::modelDash10);
    Entity::deleteModels(&ManiaSonicModel::modelDash11);
    Entity::deleteModels(&ManiaSonicModel::modelJump);
    Entity::deleteModels(&ManiaSonicModel::modelJog0);
    Entity::deleteModels(&ManiaSonicModel::modelJog1);
    Entity::deleteModels(&ManiaSonicModel::modelJog2);
    Entity::deleteModels(&ManiaSonicModel::modelJog3);
    Entity::deleteModels(&ManiaSonicModel::modelJog4);
    Entity::deleteModels(&ManiaSonicModel::modelJog5);
    Entity::deleteModels(&ManiaSonicModel::modelJog6);
    Entity::deleteModels(&ManiaSonicModel::modelJog7);
    Entity::deleteModels(&ManiaSonicModel::modelJog8);
    Entity::deleteModels(&ManiaSonicModel::modelJog9);
    Entity::deleteModels(&ManiaSonicModel::modelJog10);
    Entity::deleteModels(&ManiaSonicModel::modelJog11);
    Entity::deleteModels(&ManiaSonicModel::modelJog12);
    Entity::deleteModels(&ManiaSonicModel::modelJog13);
    Entity::deleteModels(&ManiaSonicModel::modelJog14);
    Entity::deleteModels(&ManiaSonicModel::modelJog15);
    Entity::deleteModels(&ManiaSonicModel::modelJog16);
    Entity::deleteModels(&ManiaSonicModel::modelJog17);
    Entity::deleteModels(&ManiaSonicModel::modelGrind);
    Entity::deleteModels(&ManiaSonicModel::modelSkid);
    Entity::deleteModels(&ManiaSonicModel::modelLightdash);
    Entity::deleteModels(&ManiaSonicModel::modelFreefall);
    Entity::deleteModels(&ManiaSonicModel::modelStomp);
    Entity::deleteModels(&ManiaSonicModel::modelGrab);
    Entity::deleteModels(&ManiaSonicModel::modelDrive);
}
