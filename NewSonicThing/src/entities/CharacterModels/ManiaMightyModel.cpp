#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "maniamightymodel.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../../entities/playersonic.hpp"
#include "../../toolbox/maths.hpp"
#include "../../animation/body.hpp"
#include "../../animation/limb.hpp"
#include "../../animation/animationresources.hpp"
#include "../../particles/particletexture.hpp"
#include "../../particles/particleresources.hpp"
#include "../dummy.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> ManiaMightyModel::modelBody;
std::vector<TexturedModel*> ManiaMightyModel::modelHead;
std::vector<TexturedModel*> ManiaMightyModel::modelLeftHumerus;
std::vector<TexturedModel*> ManiaMightyModel::modelLeftForearm;
std::vector<TexturedModel*> ManiaMightyModel::modelLeftHand;
std::vector<TexturedModel*> ManiaMightyModel::modelLeftThigh;
std::vector<TexturedModel*> ManiaMightyModel::modelLeftShin;
std::vector<TexturedModel*> ManiaMightyModel::modelLeftFoot;
std::vector<TexturedModel*> ManiaMightyModel::modelRightHumerus;
std::vector<TexturedModel*> ManiaMightyModel::modelRightForearm;
std::vector<TexturedModel*> ManiaMightyModel::modelRightHand;
std::vector<TexturedModel*> ManiaMightyModel::modelRightThigh;
std::vector<TexturedModel*> ManiaMightyModel::modelRightShin;
std::vector<TexturedModel*> ManiaMightyModel::modelRightFoot;

std::vector<TexturedModel*> ManiaMightyModel::modelDash0;
std::vector<TexturedModel*> ManiaMightyModel::modelDash1;
std::vector<TexturedModel*> ManiaMightyModel::modelDash2;
std::vector<TexturedModel*> ManiaMightyModel::modelDash3;
std::vector<TexturedModel*> ManiaMightyModel::modelDash4;
std::vector<TexturedModel*> ManiaMightyModel::modelDash5;
std::vector<TexturedModel*> ManiaMightyModel::modelDash6;
std::vector<TexturedModel*> ManiaMightyModel::modelDash7;
std::vector<TexturedModel*> ManiaMightyModel::modelDash8;
std::vector<TexturedModel*> ManiaMightyModel::modelDash9;
std::vector<TexturedModel*> ManiaMightyModel::modelDash10;
std::vector<TexturedModel*> ManiaMightyModel::modelDash11;
std::vector<TexturedModel*> ManiaMightyModel::modelJump;
std::vector<TexturedModel*> ManiaMightyModel::modelJog0;
std::vector<TexturedModel*> ManiaMightyModel::modelJog1;
std::vector<TexturedModel*> ManiaMightyModel::modelJog2;
std::vector<TexturedModel*> ManiaMightyModel::modelJog3;
std::vector<TexturedModel*> ManiaMightyModel::modelJog4;
std::vector<TexturedModel*> ManiaMightyModel::modelJog5;
std::vector<TexturedModel*> ManiaMightyModel::modelJog6;
std::vector<TexturedModel*> ManiaMightyModel::modelJog7;
std::vector<TexturedModel*> ManiaMightyModel::modelJog8;
std::vector<TexturedModel*> ManiaMightyModel::modelJog9;
std::vector<TexturedModel*> ManiaMightyModel::modelJog10;
std::vector<TexturedModel*> ManiaMightyModel::modelJog11;
std::vector<TexturedModel*> ManiaMightyModel::modelJog12;
std::vector<TexturedModel*> ManiaMightyModel::modelJog13;
std::vector<TexturedModel*> ManiaMightyModel::modelJog14;
std::vector<TexturedModel*> ManiaMightyModel::modelJog15;
std::vector<TexturedModel*> ManiaMightyModel::modelJog16;
std::vector<TexturedModel*> ManiaMightyModel::modelJog17;
std::vector<TexturedModel*> ManiaMightyModel::modelGrind;
std::vector<TexturedModel*> ManiaMightyModel::modelSkid;
std::vector<TexturedModel*> ManiaMightyModel::modelLightdash;
std::vector<TexturedModel*> ManiaMightyModel::modelFreefall;
std::vector<TexturedModel*> ManiaMightyModel::modelStomp;
std::vector<TexturedModel*> ManiaMightyModel::modelGrab;
std::vector<TexturedModel*> ManiaMightyModel::modelDrive;
std::vector<TexturedModel*> ManiaMightyModel::modelFlyBody;
std::vector<TexturedModel*> ManiaMightyModel::modelFlyBodyTired;
std::vector<TexturedModel*> ManiaMightyModel::modelFlyPropeller;

ManiaMightyModel::ManiaMightyModel()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 0.27f;
    visible = false;
    models = &ManiaMightyModel::modelDash0;

    flyPropeller = new Dummy(&ManiaMightyModel::modelFlyPropeller); INCR_NEW("Entity");
    Global::addEntity(flyPropeller);

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

void ManiaMightyModel::step()
{

}

float ManiaMightyModel::getDisplayBallOffset() const
{
    return displayBallOffset;
}

ParticleTexture* ManiaMightyModel::getBallTexture()
{
    return ParticleResources::textureRedTrail;
}

void ManiaMightyModel::animate(int animIndex, float time)
{
    currentAnimIndex = animIndex;
    currentAnimTime = time;
    switch (animIndex)
    {
        case 0: //stand
        {
            const float limbsScale = 0.85f;
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
            scale = (0.27f);
            int index = (int)(time / 8.3333333f);
            switch (index % 12)
            {
                case  0: models = &ManiaMightyModel::modelDash0;  break;
                case  1: models = &ManiaMightyModel::modelDash1;  break;
                case  2: models = &ManiaMightyModel::modelDash2;  break;
                case  3: models = &ManiaMightyModel::modelDash3;  break;
                case  4: models = &ManiaMightyModel::modelDash4;  break;
                case  5: models = &ManiaMightyModel::modelDash5;  break;
                case  6: models = &ManiaMightyModel::modelDash6;  break;
                case  7: models = &ManiaMightyModel::modelDash7;  break;
                case  8: models = &ManiaMightyModel::modelDash8;  break;
                case  9: models = &ManiaMightyModel::modelDash9;  break;
                case 10: models = &ManiaMightyModel::modelDash10; break;
                case 11: models = &ManiaMightyModel::modelDash11; break;
                default: printf("dash animation index out of bounds"); break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 3: //stomp
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ+90, rotRoll-90, limbsScale);
            updateLimbs(3, 100);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 8: //skid
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(8, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 11: //hitstun
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(11, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 12: //jump
        {
            scale = (0.27f);
            models = &ManiaMightyModel::modelJump;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 14: //dab
        {
            Vector3f off = currentUpDirection.scaleCopy(displayHeightOffset);
            Vector3f pos = position + off;
            myBody->setBaseOrientation(pos.x, pos.y, pos.z, rotX, rotY, rotZ, rotRoll, 1.0f);
            updateLimbs(14, time);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 15: //jog
        {
            scale = (0.27f);
            int index = (int)(time / 5.55555555f);
            switch (index % 18)
            {
                case 0:  models = &ManiaMightyModel::modelJog0;  break;
                case 1:  models = &ManiaMightyModel::modelJog1;  break;
                case 2:  models = &ManiaMightyModel::modelJog2;  break;
                case 3:  models = &ManiaMightyModel::modelJog3;  break;
                case 4:  models = &ManiaMightyModel::modelJog4;  break;
                case 5:  models = &ManiaMightyModel::modelJog5;  break;
                case 6:  models = &ManiaMightyModel::modelJog6;  break;
                case 7:  models = &ManiaMightyModel::modelJog7;  break;
                case 8:  models = &ManiaMightyModel::modelJog8;  break;
                case 9:  models = &ManiaMightyModel::modelJog9;  break;
                case 10: models = &ManiaMightyModel::modelJog10; break;
                case 11: models = &ManiaMightyModel::modelJog11; break;
                case 12: models = &ManiaMightyModel::modelJog12; break;
                case 13: models = &ManiaMightyModel::modelJog13; break;
                case 14: models = &ManiaMightyModel::modelJog14; break;
                case 15: models = &ManiaMightyModel::modelJog15; break;
                case 16: models = &ManiaMightyModel::modelJog16; break;
                case 17: models = &ManiaMightyModel::modelJog17; break;
                default: printf("warning: jog animation index out of bounds\n"); break;
            }
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 18: //lightdash
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ-90, rotRoll, limbsScale);
            updateLimbs(18, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 19: //die
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(19, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 20: //fly
        {
            scale = 0.84f;
            setLimbsVisibility(false);
            setFlyVisibility(true);
            visible = baseVisible;
            setFlyVisibility(baseVisible);
            models = &ManiaMightyModel::modelFlyBody;
            flyPropeller->rotY += time*0.6f;
            updateFlyMatrix();
            updateTransformationMatrix();
            break;
        }

        case 21: //freefall
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(21, 0);
            updateLimbsMatrix();
            setLimbsVisibility(baseVisible);
            setFlyVisibility(false);
            visible = false;
            break;
        }

        case 25: //grab
        {
            scale = (0.84f);
            models = &ManiaMightyModel::modelGrab;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 26: //grind
        {
            scale = (0.84f);
            models = &ManiaMightyModel::modelGrind;
            updateTransformationMatrixYXZY();
            setLimbsVisibility(false);
            setFlyVisibility(false);
            visible = baseVisible;
            break;
        }

        case 27: //fly tired
        {
            if (Global::levelId == LVL_TWINKLE_CIRCUIT)
            {
                //scale = (3.84f);
                //models = &ManiaMightyModel::modelLightdash;
                //Vector3f pos = position + currentUpDirection.scaleCopy(20.0f);
                //Maths::createTransformationMatrixYXZY(&transformationMatrix, &pos, scale, rotX, rotY, rotZ, rotRoll);
                //setLimbsVisibility(false);
                //setFlyVisibility(false);
                //visible = true;

                scale = (1.0f);
                models = &ManiaMightyModel::modelDrive;
                updateTransformationMatrixYXZY();
                setLimbsVisibility(false);
                visible = baseVisible;
                break;

                //scale = 0.84f;
                //setLimbsVisibility(false);
                //setFlyVisibility(true);
                //visible = baseVisible;
                //setFlyVisibility(baseVisible);
                //models = &ManiaMightyModel::modelDrive;
                //flyPropeller->rotY += time*0.2f;
                //updateFlyMatrix();
                //updateTransformationMatrix();
            }
            else
            {
                scale = 0.84f;
                setLimbsVisibility(false);
                setFlyVisibility(true);
                visible = baseVisible;
                setFlyVisibility(baseVisible);
                models = &ManiaMightyModel::modelFlyBodyTired;
                flyPropeller->rotY += time*0.2f;
                updateFlyMatrix();
                updateTransformationMatrix();
            }
            break;
        }

        default:
            break;
    }
}

void ManiaMightyModel::setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, const Vector3f* newUp)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX       = xRot;
    rotY       = yRot;
    rotZ       = zRot;
    rotRoll    = spinRot;
    currentUpDirection.set(newUp);
    flyPropeller->position.set(x, y, z);
    flyPropeller->setRotation(xRot, yRot, zRot, spinRot);
}

void ManiaMightyModel::setBaseColor(float r, float g, float b)
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

void ManiaMightyModel::setBaseAlpha(float a)
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

void ManiaMightyModel::setRenderOrderOverride(char newOrder)
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

void ManiaMightyModel::setBaseVisibility(bool newVisible)
{
    baseVisible = newVisible;
}

std::vector<TexturedModel*>* ManiaMightyModel::getModels()
{
    return models;
}

void ManiaMightyModel::loadStaticModels()
{
    if (ManiaMightyModel::modelDash0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading ManiaMightyModel static models...\n");
    #endif

    ModelLoader::loadModel(&ManiaMightyModel::modelBody,         "res/Models/Characters/ManiaMighty/", "LimbBody");
    ModelLoader::loadModel(&ManiaMightyModel::modelHead,         "res/Models/Characters/ManiaMighty/", "LimbHead");
    ModelLoader::loadModel(&ManiaMightyModel::modelLeftHumerus,  "res/Models/Characters/ManiaMighty/", "LimbHumerus");
    ModelLoader::loadModel(&ManiaMightyModel::modelLeftForearm,  "res/Models/Characters/ManiaMighty/", "LimbForearm");
    ModelLoader::loadModel(&ManiaMightyModel::modelLeftHand,     "res/Models/Characters/ManiaMighty/", "LimbHandLeft");
    ModelLoader::loadModel(&ManiaMightyModel::modelLeftThigh,    "res/Models/Characters/ManiaMighty/", "LimbThigh");
    ModelLoader::loadModel(&ManiaMightyModel::modelLeftShin,     "res/Models/Characters/ManiaMighty/", "LimbShin");
    ModelLoader::loadModel(&ManiaMightyModel::modelLeftFoot,     "res/Models/Characters/ManiaMighty/", "LimbShoeLeft");
    ModelLoader::loadModel(&ManiaMightyModel::modelRightHumerus, "res/Models/Characters/ManiaMighty/", "LimbHumerus");
    ModelLoader::loadModel(&ManiaMightyModel::modelRightForearm, "res/Models/Characters/ManiaMighty/", "LimbForearm");
    ModelLoader::loadModel(&ManiaMightyModel::modelRightHand,    "res/Models/Characters/ManiaMighty/", "LimbHandRight");
    ModelLoader::loadModel(&ManiaMightyModel::modelRightThigh,   "res/Models/Characters/ManiaMighty/", "LimbThigh");
    ModelLoader::loadModel(&ManiaMightyModel::modelRightShin,    "res/Models/Characters/ManiaMighty/", "LimbShin");
    ModelLoader::loadModel(&ManiaMightyModel::modelRightFoot,    "res/Models/Characters/ManiaMighty/", "LimbShoeRight");

    ModelLoader::loadModel(&ManiaMightyModel::modelDash0       , "res/Models/Characters/ManiaMighty/", "MightyDash0");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash1       , "res/Models/Characters/ManiaMighty/", "MightyDash1");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash2       , "res/Models/Characters/ManiaMighty/", "MightyDash2");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash3       , "res/Models/Characters/ManiaMighty/", "MightyDash3");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash4       , "res/Models/Characters/ManiaMighty/", "MightyDash4");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash5       , "res/Models/Characters/ManiaMighty/", "MightyDash5");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash6       , "res/Models/Characters/ManiaMighty/", "MightyDash6");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash7       , "res/Models/Characters/ManiaMighty/", "MightyDash7");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash8       , "res/Models/Characters/ManiaMighty/", "MightyDash8");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash9       , "res/Models/Characters/ManiaMighty/", "MightyDash9");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash10      , "res/Models/Characters/ManiaMighty/", "MightyDash10");
    ModelLoader::loadModel(&ManiaMightyModel::modelDash11      , "res/Models/Characters/ManiaMighty/", "MightyDash11");
    ModelLoader::loadModel(&ManiaMightyModel::modelJump        , "res/Models/Characters/ManiaMighty/", "MightyJump");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog0        , "res/Models/Characters/ManiaMighty/", "MightyJog0");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog1        , "res/Models/Characters/ManiaMighty/", "MightyJog1");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog2        , "res/Models/Characters/ManiaMighty/", "MightyJog2");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog3        , "res/Models/Characters/ManiaMighty/", "MightyJog3");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog4        , "res/Models/Characters/ManiaMighty/", "MightyJog4");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog5        , "res/Models/Characters/ManiaMighty/", "MightyJog5");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog6        , "res/Models/Characters/ManiaMighty/", "MightyJog6");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog7        , "res/Models/Characters/ManiaMighty/", "MightyJog7");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog8        , "res/Models/Characters/ManiaMighty/", "MightyJog8");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog9        , "res/Models/Characters/ManiaMighty/", "MightyJog9");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog10       , "res/Models/Characters/ManiaMighty/", "MightyJog10");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog11       , "res/Models/Characters/ManiaMighty/", "MightyJog11");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog12       , "res/Models/Characters/ManiaMighty/", "MightyJog12");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog13       , "res/Models/Characters/ManiaMighty/", "MightyJog13");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog14       , "res/Models/Characters/ManiaMighty/", "MightyJog14");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog15       , "res/Models/Characters/ManiaMighty/", "MightyJog15");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog16       , "res/Models/Characters/ManiaMighty/", "MightyJog16");
    ModelLoader::loadModel(&ManiaMightyModel::modelJog17       , "res/Models/Characters/ManiaMighty/", "MightyJog17");
    ModelLoader::loadModel(&ManiaMightyModel::modelGrind       , "res/Models/Characters/ManiaMighty/", "MightyGrind");
    ModelLoader::loadModel(&ManiaMightyModel::modelSkid        , "res/Models/Characters/ManiaMighty/", "MightySkid");
    ModelLoader::loadModel(&ManiaMightyModel::modelLightdash   , "res/Models/Characters/ManiaMighty/", "MightyLightdash");
    ModelLoader::loadModel(&ManiaMightyModel::modelFreefall    , "res/Models/Characters/ManiaMighty/", "MightyFreefall");
    ModelLoader::loadModel(&ManiaMightyModel::modelStomp       , "res/Models/Characters/ManiaMighty/", "MightyStomp");
    ModelLoader::loadModel(&ManiaMightyModel::modelGrab        , "res/Models/Characters/ManiaMighty/", "MightyGrab");
    ModelLoader::loadModel(&ManiaMightyModel::modelDrive       , "res/Models/Characters/ManiaMighty/", "MightyDrive");
    ModelLoader::loadModel(&ManiaMightyModel::modelFlyBody     , "res/Models/Characters/ManiaMighty/", "MightyFlyBody");
    ModelLoader::loadModel(&ManiaMightyModel::modelFlyBodyTired, "res/Models/Characters/ManiaMighty/", "MightyFlyBodyTired");
    ModelLoader::loadModel(&ManiaMightyModel::modelFlyPropeller, "res/Models/Characters/ManiaMighty/", "MightyFlyPropeller");
}

void ManiaMightyModel::setLimbsVisibility(bool newVisible)
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

void ManiaMightyModel::updateLimbs(int animIndex, float time)
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

void ManiaMightyModel::updateLimbsMatrix()
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

void ManiaMightyModel::setFlyVisibility(bool newVisible)
{
    flyPropeller->visible = newVisible;
}

void ManiaMightyModel::updateFlyMatrix()
{
    flyPropeller->updateTransformationMatrix();
}

void ManiaMightyModel::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting ManiaMightyModel static models...\n");
    #endif

    Entity::deleteModels(&ManiaMightyModel::modelBody);
    Entity::deleteModels(&ManiaMightyModel::modelHead);
    Entity::deleteModels(&ManiaMightyModel::modelLeftHumerus);
    Entity::deleteModels(&ManiaMightyModel::modelLeftForearm);
    Entity::deleteModels(&ManiaMightyModel::modelLeftHand);
    Entity::deleteModels(&ManiaMightyModel::modelLeftThigh);
    Entity::deleteModels(&ManiaMightyModel::modelLeftShin);
    Entity::deleteModels(&ManiaMightyModel::modelLeftFoot);
    Entity::deleteModels(&ManiaMightyModel::modelRightHumerus);
    Entity::deleteModels(&ManiaMightyModel::modelRightForearm);
    Entity::deleteModels(&ManiaMightyModel::modelRightHand);
    Entity::deleteModels(&ManiaMightyModel::modelRightThigh);
    Entity::deleteModels(&ManiaMightyModel::modelRightShin);
    Entity::deleteModels(&ManiaMightyModel::modelRightFoot);

    Entity::deleteModels(&ManiaMightyModel::modelDash0);
    Entity::deleteModels(&ManiaMightyModel::modelDash1);
    Entity::deleteModels(&ManiaMightyModel::modelDash2);
    Entity::deleteModels(&ManiaMightyModel::modelDash3);
    Entity::deleteModels(&ManiaMightyModel::modelDash4);
    Entity::deleteModels(&ManiaMightyModel::modelDash5);
    Entity::deleteModels(&ManiaMightyModel::modelDash6);
    Entity::deleteModels(&ManiaMightyModel::modelDash7);
    Entity::deleteModels(&ManiaMightyModel::modelDash8);
    Entity::deleteModels(&ManiaMightyModel::modelDash9);
    Entity::deleteModels(&ManiaMightyModel::modelDash10);
    Entity::deleteModels(&ManiaMightyModel::modelDash11);
    Entity::deleteModels(&ManiaMightyModel::modelJump);
    Entity::deleteModels(&ManiaMightyModel::modelJog0);
    Entity::deleteModels(&ManiaMightyModel::modelJog1);
    Entity::deleteModels(&ManiaMightyModel::modelJog2);
    Entity::deleteModels(&ManiaMightyModel::modelJog3);
    Entity::deleteModels(&ManiaMightyModel::modelJog4);
    Entity::deleteModels(&ManiaMightyModel::modelJog5);
    Entity::deleteModels(&ManiaMightyModel::modelJog6);
    Entity::deleteModels(&ManiaMightyModel::modelJog7);
    Entity::deleteModels(&ManiaMightyModel::modelJog8);
    Entity::deleteModels(&ManiaMightyModel::modelJog9);
    Entity::deleteModels(&ManiaMightyModel::modelJog10);
    Entity::deleteModels(&ManiaMightyModel::modelJog11);
    Entity::deleteModels(&ManiaMightyModel::modelJog12);
    Entity::deleteModels(&ManiaMightyModel::modelJog13);
    Entity::deleteModels(&ManiaMightyModel::modelJog14);
    Entity::deleteModels(&ManiaMightyModel::modelJog15);
    Entity::deleteModels(&ManiaMightyModel::modelJog16);
    Entity::deleteModels(&ManiaMightyModel::modelJog17);
    Entity::deleteModels(&ManiaMightyModel::modelGrind);
    Entity::deleteModels(&ManiaMightyModel::modelSkid);
    Entity::deleteModels(&ManiaMightyModel::modelLightdash);
    Entity::deleteModels(&ManiaMightyModel::modelFreefall);
    Entity::deleteModels(&ManiaMightyModel::modelStomp);
    Entity::deleteModels(&ManiaMightyModel::modelGrab);
    Entity::deleteModels(&ManiaMightyModel::modelDrive);
    Entity::deleteModels(&ManiaMightyModel::modelFlyBody);
    Entity::deleteModels(&ManiaMightyModel::modelFlyBodyTired);
    Entity::deleteModels(&ManiaMightyModel::modelFlyPropeller);
}
