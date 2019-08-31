#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "maniamightymodel.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/playersonic.h"
#include "../toolbox/maths.h"
#include "../animation/body.h"
#include "../animation/limb.h"
#include "../animation/animationresources.h"
#include "../particles/particletexture.h"
#include "../particles/particleresources.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

std::list<TexturedModel*> ManiaMightyModel::modelBody;
std::list<TexturedModel*> ManiaMightyModel::modelHead;
std::list<TexturedModel*> ManiaMightyModel::modelLeftHumerus;
std::list<TexturedModel*> ManiaMightyModel::modelLeftForearm;
std::list<TexturedModel*> ManiaMightyModel::modelLeftHand;
std::list<TexturedModel*> ManiaMightyModel::modelLeftThigh;
std::list<TexturedModel*> ManiaMightyModel::modelLeftShin;
std::list<TexturedModel*> ManiaMightyModel::modelLeftFoot;
std::list<TexturedModel*> ManiaMightyModel::modelRightHumerus;
std::list<TexturedModel*> ManiaMightyModel::modelRightForearm;
std::list<TexturedModel*> ManiaMightyModel::modelRightHand;
std::list<TexturedModel*> ManiaMightyModel::modelRightThigh;
std::list<TexturedModel*> ManiaMightyModel::modelRightShin;
std::list<TexturedModel*> ManiaMightyModel::modelRightFoot;

std::list<TexturedModel*> ManiaMightyModel::modelDash0;
std::list<TexturedModel*> ManiaMightyModel::modelDash1;
std::list<TexturedModel*> ManiaMightyModel::modelDash2;
std::list<TexturedModel*> ManiaMightyModel::modelDash3;
std::list<TexturedModel*> ManiaMightyModel::modelDash4;
std::list<TexturedModel*> ManiaMightyModel::modelDash5;
std::list<TexturedModel*> ManiaMightyModel::modelDash6;
std::list<TexturedModel*> ManiaMightyModel::modelDash7;
std::list<TexturedModel*> ManiaMightyModel::modelDash8;
std::list<TexturedModel*> ManiaMightyModel::modelDash9;
std::list<TexturedModel*> ManiaMightyModel::modelDash10;
std::list<TexturedModel*> ManiaMightyModel::modelDash11;
std::list<TexturedModel*> ManiaMightyModel::modelJump;
std::list<TexturedModel*> ManiaMightyModel::modelJog0;
std::list<TexturedModel*> ManiaMightyModel::modelJog1;
std::list<TexturedModel*> ManiaMightyModel::modelJog2;
std::list<TexturedModel*> ManiaMightyModel::modelJog3;
std::list<TexturedModel*> ManiaMightyModel::modelJog4;
std::list<TexturedModel*> ManiaMightyModel::modelJog5;
std::list<TexturedModel*> ManiaMightyModel::modelJog6;
std::list<TexturedModel*> ManiaMightyModel::modelJog7;
std::list<TexturedModel*> ManiaMightyModel::modelJog8;
std::list<TexturedModel*> ManiaMightyModel::modelJog9;
std::list<TexturedModel*> ManiaMightyModel::modelJog10;
std::list<TexturedModel*> ManiaMightyModel::modelJog11;
std::list<TexturedModel*> ManiaMightyModel::modelJog12;
std::list<TexturedModel*> ManiaMightyModel::modelJog13;
std::list<TexturedModel*> ManiaMightyModel::modelJog14;
std::list<TexturedModel*> ManiaMightyModel::modelJog15;
std::list<TexturedModel*> ManiaMightyModel::modelJog16;
std::list<TexturedModel*> ManiaMightyModel::modelJog17;
std::list<TexturedModel*> ManiaMightyModel::modelGrind;
std::list<TexturedModel*> ManiaMightyModel::modelSkid;
std::list<TexturedModel*> ManiaMightyModel::modelLightdash;
std::list<TexturedModel*> ManiaMightyModel::modelFreefall;
std::list<TexturedModel*> ManiaMightyModel::modelStomp;
std::list<TexturedModel*> ManiaMightyModel::modelGrab;

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

	myBody =         new Body(&modelBody);
	myHead =         new Limb(&modelHead,         1.2f,  -0.3f,   0,     myBody,   nullptr);        INCR_NEW("Entity")
	myLeftHumerus =  new Limb(&modelLeftHumerus,  0.9f,	  0,     -0.9f,  myBody,   nullptr);        INCR_NEW("Entity")
	myLeftForearm =  new Limb(&modelLeftForearm,  0,     -1.3f,   0,     nullptr,  myLeftHumerus);  INCR_NEW("Entity")
	myLeftHand =     new Limb(&modelLeftHand,     0,	 -1.3f,   0,     nullptr,  myLeftForearm);  INCR_NEW("Entity")
	myLeftThigh =    new Limb(&modelLeftThigh,   -0.9f,   0,     -0.3f,  myBody,   nullptr);        INCR_NEW("Entity")
	myLeftShin =     new Limb(&modelLeftShin,     0,	 -1.3f,   0,     nullptr,  myLeftThigh);    INCR_NEW("Entity")
	myLeftFoot =     new Limb(&modelLeftFoot,     0,	 -1.1f,   0,     nullptr,  myLeftShin);     INCR_NEW("Entity")
	myRightHumerus = new Limb(&modelRightHumerus, 0.9f,   0,      0.9f,  myBody,   nullptr);        INCR_NEW("Entity")
	myRightForearm = new Limb(&modelRightForearm, 0,	 -1.3f,   0,     nullptr,  myRightHumerus); INCR_NEW("Entity")
	myRightHand =    new Limb(&modelRightHand,    0,	 -1.3f,   0,     nullptr,  myRightForearm); INCR_NEW("Entity")
	myRightThigh =   new Limb(&modelRightThigh,  -0.9f,   0,      0.3f,  myBody,   nullptr);        INCR_NEW("Entity")
	myRightShin =    new Limb(&modelRightShin,    0,	 -1.3f,   0,     nullptr,  myRightThigh);   INCR_NEW("Entity")
	myRightFoot =    new Limb(&modelRightFoot,    0,	 -1.1f,   0,     nullptr,  myRightShin);    INCR_NEW("Entity")

    AnimationResources::assignAnimationsHuman(myBody, myHead,
		myLeftHumerus, myLeftForearm, myLeftHand,
		myRightHumerus, myRightForearm, myRightHand,
		myLeftThigh, myLeftShin, myLeftFoot,
		myRightThigh, myRightShin, myRightFoot);

    Main_addEntity(myBody);
	Main_addEntity(myHead);
	Main_addEntity(myLeftHumerus);
	Main_addEntity(myLeftForearm);
	Main_addEntity(myLeftHand);
	Main_addEntity(myLeftThigh);
	Main_addEntity(myLeftShin);
	Main_addEntity(myLeftFoot);
	Main_addEntity(myRightHumerus);
	Main_addEntity(myRightForearm);
	Main_addEntity(myRightHand);
	Main_addEntity(myRightThigh);
	Main_addEntity(myRightShin);
	Main_addEntity(myRightFoot);
}

void ManiaMightyModel::step()
{

}

const float ManiaMightyModel::getDisplayBallOffset()
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
            updateLimbs(0, time);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
            break;
        }

		case 1: //run
		{
            setScale(0.27f);
			int index = (int)(time / 8.3333333f);
			switch (index)
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
				default: std::fprintf(stdout, "dash animation index out of bounds"); break;
			}
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = true;
			break;
		}

        case 3: //stomp
		{
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ+90, rotRoll-90, limbsScale);
            updateLimbs(3, 100);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
            break;
		}

        case 8: //skid
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(8, 0);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
            break;
        }

        case 11: //hitstun
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(11, 0);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
            break;
        }

		case 12: //jump
		{
            setScale(0.27f);
			models = &ManiaMightyModel::modelJump;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = true;
			break;
		}

		case 15: //jog
		{
            setScale(0.27f);
			int index = (int)(time / 5.55555555f);
			switch (index)
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
                default: std::fprintf(stdout, "warning: jog animation index out of bounds\n"); break;
			}
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = true;
			break;
		}

        case 18: //lightdash
		{
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ-90, rotRoll, limbsScale);
            updateLimbs(18, 0);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
            break;
		}

        case 19: //die
        {
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(19, 0);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
            break;
        }

        case 21: //freefall
		{
            const float limbsScale = 0.85f;
            myBody->setBaseOrientation(position.x, position.y, position.z, rotX, rotY, rotZ, rotRoll, limbsScale);
            updateLimbs(21, 0);
            updateLimbsMatrix();
            setLimbsVisibility(true);
            visible = false;
			break;
		}

        case 25: //grab
		{
            setScale(0.27f);
			models = &ManiaMightyModel::modelGrab;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = true;
			break;
		}

		case 26: //grind
		{
            setScale(0.27f);
			models = &ManiaMightyModel::modelGrind;
            updateTransformationMatrix();
            setLimbsVisibility(false);
            visible = true;
			break;
		}

		default:
			break;
	}
}

void ManiaMightyModel::setOrientation(float x, float y, float z, float xRot, float yRot, float zRot, float spinRot, Vector3f* newUp)
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

std::list<TexturedModel*>* ManiaMightyModel::getModels()
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
	std::fprintf(stdout, "Loading mania sonic static models...\n");
	#endif

    loadModel(&ManiaMightyModel::modelBody,         "res/Models/Characters/ManiaMighty/", "LimbBody");
	loadModel(&ManiaMightyModel::modelHead,         "res/Models/Characters/ManiaMighty/", "LimbHead");
	loadModel(&ManiaMightyModel::modelLeftHumerus,  "res/Models/Characters/ManiaMighty/", "LimbHumerus");
	loadModel(&ManiaMightyModel::modelLeftForearm,  "res/Models/Characters/ManiaMighty/", "LimbForearm");
	loadModel(&ManiaMightyModel::modelLeftHand,     "res/Models/Characters/ManiaMighty/", "LimbHandLeft");
	loadModel(&ManiaMightyModel::modelLeftThigh,    "res/Models/Characters/ManiaMighty/", "LimbThigh");
	loadModel(&ManiaMightyModel::modelLeftShin,     "res/Models/Characters/ManiaMighty/", "LimbShin");
	loadModel(&ManiaMightyModel::modelLeftFoot,     "res/Models/Characters/ManiaMighty/", "LimbShoeLeft");
	loadModel(&ManiaMightyModel::modelRightHumerus, "res/Models/Characters/ManiaMighty/", "LimbHumerus");
	loadModel(&ManiaMightyModel::modelRightForearm, "res/Models/Characters/ManiaMighty/", "LimbForearm");
	loadModel(&ManiaMightyModel::modelRightHand,    "res/Models/Characters/ManiaMighty/", "LimbHandRight");
	loadModel(&ManiaMightyModel::modelRightThigh,   "res/Models/Characters/ManiaMighty/", "LimbThigh");
	loadModel(&ManiaMightyModel::modelRightShin,    "res/Models/Characters/ManiaMighty/", "LimbShin");
	loadModel(&ManiaMightyModel::modelRightFoot,    "res/Models/Characters/ManiaMighty/", "LimbShoeRight");

	loadModel(&ManiaMightyModel::modelDash0    , "res/Models/Characters/ManiaMighty/", "MightyDash0");
	loadModel(&ManiaMightyModel::modelDash1    , "res/Models/Characters/ManiaMighty/", "MightyDash1");
	loadModel(&ManiaMightyModel::modelDash2    , "res/Models/Characters/ManiaMighty/", "MightyDash2");
	loadModel(&ManiaMightyModel::modelDash3    , "res/Models/Characters/ManiaMighty/", "MightyDash3");
	loadModel(&ManiaMightyModel::modelDash4    , "res/Models/Characters/ManiaMighty/", "MightyDash4");
	loadModel(&ManiaMightyModel::modelDash5    , "res/Models/Characters/ManiaMighty/", "MightyDash5");
	loadModel(&ManiaMightyModel::modelDash6    , "res/Models/Characters/ManiaMighty/", "MightyDash6");
	loadModel(&ManiaMightyModel::modelDash7    , "res/Models/Characters/ManiaMighty/", "MightyDash7");
	loadModel(&ManiaMightyModel::modelDash8    , "res/Models/Characters/ManiaMighty/", "MightyDash8");
	loadModel(&ManiaMightyModel::modelDash9    , "res/Models/Characters/ManiaMighty/", "MightyDash9");
	loadModel(&ManiaMightyModel::modelDash10   , "res/Models/Characters/ManiaMighty/", "MightyDash10");
	loadModel(&ManiaMightyModel::modelDash11   , "res/Models/Characters/ManiaMighty/", "MightyDash11");
	loadModel(&ManiaMightyModel::modelJump     , "res/Models/Characters/ManiaMighty/", "MightyJump");
	loadModel(&ManiaMightyModel::modelJog0     , "res/Models/Characters/ManiaMighty/", "MightyJog0");
	loadModel(&ManiaMightyModel::modelJog1     , "res/Models/Characters/ManiaMighty/", "MightyJog1");
	loadModel(&ManiaMightyModel::modelJog2     , "res/Models/Characters/ManiaMighty/", "MightyJog2");
	loadModel(&ManiaMightyModel::modelJog3     , "res/Models/Characters/ManiaMighty/", "MightyJog3");
	loadModel(&ManiaMightyModel::modelJog4     , "res/Models/Characters/ManiaMighty/", "MightyJog4");
	loadModel(&ManiaMightyModel::modelJog5     , "res/Models/Characters/ManiaMighty/", "MightyJog5");
	loadModel(&ManiaMightyModel::modelJog6     , "res/Models/Characters/ManiaMighty/", "MightyJog6");
	loadModel(&ManiaMightyModel::modelJog7     , "res/Models/Characters/ManiaMighty/", "MightyJog7");
	loadModel(&ManiaMightyModel::modelJog8     , "res/Models/Characters/ManiaMighty/", "MightyJog8");
	loadModel(&ManiaMightyModel::modelJog9     , "res/Models/Characters/ManiaMighty/", "MightyJog9");
	loadModel(&ManiaMightyModel::modelJog10    , "res/Models/Characters/ManiaMighty/", "MightyJog10");
	loadModel(&ManiaMightyModel::modelJog11    , "res/Models/Characters/ManiaMighty/", "MightyJog11");
	loadModel(&ManiaMightyModel::modelJog12    , "res/Models/Characters/ManiaMighty/", "MightyJog12");
	loadModel(&ManiaMightyModel::modelJog13    , "res/Models/Characters/ManiaMighty/", "MightyJog13");
	loadModel(&ManiaMightyModel::modelJog14    , "res/Models/Characters/ManiaMighty/", "MightyJog14");
	loadModel(&ManiaMightyModel::modelJog15    , "res/Models/Characters/ManiaMighty/", "MightyJog15");
	loadModel(&ManiaMightyModel::modelJog16    , "res/Models/Characters/ManiaMighty/", "MightyJog16");
	loadModel(&ManiaMightyModel::modelJog17    , "res/Models/Characters/ManiaMighty/", "MightyJog17");
	loadModel(&ManiaMightyModel::modelGrind    , "res/Models/Characters/ManiaSonic/", "Grind");
    loadModel(&ManiaMightyModel::modelSkid     , "res/Models/Characters/ManiaSonic/", "Skid");
    loadModel(&ManiaMightyModel::modelLightdash, "res/Models/Characters/ManiaSonic/", "Lightdash");
    loadModel(&ManiaMightyModel::modelFreefall , "res/Models/Characters/ManiaSonic/", "Freefall");
    loadModel(&ManiaMightyModel::modelStomp    , "res/Models/Characters/ManiaSonic/", "Stomp");
    loadModel(&ManiaMightyModel::modelGrab     , "res/Models/Characters/ManiaSonic/", "Grab");
}

void ManiaMightyModel::setLimbsVisibility(bool newVisible)
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

void ManiaMightyModel::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting mania mighty static models...\n");
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
}
