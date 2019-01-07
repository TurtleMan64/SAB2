#ifndef ANIMATIONRESOURCES_H
#define ANIMATIONRESOURCES_H

class Body;
class Limb;

#include <vector>
#include "animation.h"

class AnimationResources
{
private:
	static std::vector<Animation> animsHumanBody;
	static std::vector<Animation> animsHumanHead;
	static std::vector<Animation> animsHumanLeftHumerus;
	static std::vector<Animation> animsHumanLeftForearm;
	static std::vector<Animation> animsHumanLeftHand;
	static std::vector<Animation> animsHumanRightHumerus;
	static std::vector<Animation> animsHumanRightForearm;
	static std::vector<Animation> animsHumanRightHand;
	static std::vector<Animation> animsHumanLeftThigh;
	static std::vector<Animation> animsHumanLeftShin;
	static std::vector<Animation> animsHumanLeftFoot;
	static std::vector<Animation> animsHumanRightThigh;
	static std::vector<Animation> animsHumanRightShin;
	static std::vector<Animation> animsHumanRightFoot;


public:
	static void createAnimations();

	static void assignAnimationsHuman(Body* body, Limb* head,
		Limb* leftHumerus, Limb* leftForearm, Limb* leftHand,
		Limb* rightHumerus, Limb* rightForearm, Limb* rightHand,
		Limb* leftThigh, Limb* leftShin, Limb* leftFoot,
		Limb* rightThigh, Limb* rightShin, Limb* rightFoot);

};

#endif