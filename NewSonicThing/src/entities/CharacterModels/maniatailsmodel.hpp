#ifndef MANIATAILSMODEL_H
#define MANIATAILSMODEL_H

class TexturedModel;
class Limb;
class Body;
class ParticleTexture;
class Dummy;

#include <list>
#include <vector>
#include "../entity.hpp"
#include "playermodel.hpp"


class ManiaTailsModel : public PlayerModel
{
private:
    std::vector<TexturedModel*>* models;

    const float displayHeightOffset = -0.5f;
    const float displayBallOffset = 3.0f;

    const float limbsScale = 1.0f;

    Body* myBody         = nullptr;
    Limb* myHead         = nullptr;
    Limb* myLeftHumerus  = nullptr;
    Limb* myLeftForearm  = nullptr;
    Limb* myLeftHand     = nullptr;
    Limb* myLeftThigh    = nullptr;
    Limb* myLeftShin     = nullptr;
    Limb* myLeftFoot     = nullptr;
    Limb* myRightHumerus = nullptr;
    Limb* myRightForearm = nullptr;
    Limb* myRightHand    = nullptr;
    Limb* myRightThigh   = nullptr;
    Limb* myRightShin    = nullptr;
    Limb* myRightFoot    = nullptr;

    Dummy* flyTails = nullptr;

    static std::vector<TexturedModel*> modelBody;
    static std::vector<TexturedModel*> modelHead;
    static std::vector<TexturedModel*> modelLeftHumerus;
    static std::vector<TexturedModel*> modelLeftForearm;
    static std::vector<TexturedModel*> modelLeftHand;
    static std::vector<TexturedModel*> modelLeftThigh;
    static std::vector<TexturedModel*> modelLeftShin;
    static std::vector<TexturedModel*> modelLeftFoot;
    static std::vector<TexturedModel*> modelRightHumerus;
    static std::vector<TexturedModel*> modelRightForearm;
    static std::vector<TexturedModel*> modelRightHand;
    static std::vector<TexturedModel*> modelRightThigh;
    static std::vector<TexturedModel*> modelRightShin;
    static std::vector<TexturedModel*> modelRightFoot;

    static std::vector<TexturedModel*> modelDash0;
    static std::vector<TexturedModel*> modelDash1;
    static std::vector<TexturedModel*> modelDash2;
    static std::vector<TexturedModel*> modelDash3;
    static std::vector<TexturedModel*> modelDash4;
    static std::vector<TexturedModel*> modelDash5;
    static std::vector<TexturedModel*> modelDash6;
    static std::vector<TexturedModel*> modelDash7;
    static std::vector<TexturedModel*> modelDash8;
    static std::vector<TexturedModel*> modelDash9;
    static std::vector<TexturedModel*> modelDash10;
    static std::vector<TexturedModel*> modelDash11;
    static std::vector<TexturedModel*> modelDash12;
    static std::vector<TexturedModel*> modelDash13;
    static std::vector<TexturedModel*> modelDash14;
    static std::vector<TexturedModel*> modelDash15;
    static std::vector<TexturedModel*> modelDash16;
    static std::vector<TexturedModel*> modelDash17;
    static std::vector<TexturedModel*> modelJump;
    static std::vector<TexturedModel*> modelJog0;
    static std::vector<TexturedModel*> modelJog1;
    static std::vector<TexturedModel*> modelJog2;
    static std::vector<TexturedModel*> modelJog3;
    static std::vector<TexturedModel*> modelJog4;
    static std::vector<TexturedModel*> modelJog5;
    static std::vector<TexturedModel*> modelJog6;
    static std::vector<TexturedModel*> modelJog7;
    static std::vector<TexturedModel*> modelJog8;
    static std::vector<TexturedModel*> modelJog9;
    static std::vector<TexturedModel*> modelJog10;
    static std::vector<TexturedModel*> modelJog11;
    static std::vector<TexturedModel*> modelJog12;
    static std::vector<TexturedModel*> modelJog13;
    static std::vector<TexturedModel*> modelJog14;
    static std::vector<TexturedModel*> modelJog15;
    static std::vector<TexturedModel*> modelJog16;
    static std::vector<TexturedModel*> modelJog17;
    static std::vector<TexturedModel*> modelGrind;
    static std::vector<TexturedModel*> modelSkid;
    static std::vector<TexturedModel*> modelLightdash;
    static std::vector<TexturedModel*> modelFreefall;
    static std::vector<TexturedModel*> modelStomp;
    static std::vector<TexturedModel*> modelGrab;
    static std::vector<TexturedModel*> modelFlyBody;
    static std::vector<TexturedModel*> modelFlyBodyTired;
    static std::vector<TexturedModel*> modelFlyTails;

    void setLimbsVisibility(bool newVisible);

    void updateLimbs(int animIndex, float time);

    void updateLimbsMatrix();

    void setFlyVisibility(bool newVisible);

    void updateFlyMatrix();

public:
    ManiaTailsModel();

    void step();

    void animate(int animIndex, float time);

    void setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, const Vector3f* up);

    void setBaseColor(float r, float g, float b);

    void setBaseAlpha(float a);

    void setBaseVisibility(bool newVisible);

    void setRenderOrderOverride(char newOrder);

    float getDisplayBallOffset() const;

    ParticleTexture* getBallTexture();

    std::vector<TexturedModel*>* getModels();

    static void loadStaticModels();

    static void deleteStaticModels();
};
#endif
