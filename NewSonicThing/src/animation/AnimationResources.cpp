#include "animationresources.h"
#include "animation.h"
#include "body.h"
#include "limb.h"

std::vector<Animation> AnimationResources::animsHumanBody;
std::vector<Animation> AnimationResources::animsHumanHead;
std::vector<Animation> AnimationResources::animsHumanLeftHumerus;
std::vector<Animation> AnimationResources::animsHumanLeftForearm;
std::vector<Animation> AnimationResources::animsHumanLeftHand;
std::vector<Animation> AnimationResources::animsHumanRightHumerus;
std::vector<Animation> AnimationResources::animsHumanRightForearm;
std::vector<Animation> AnimationResources::animsHumanRightHand;
std::vector<Animation> AnimationResources::animsHumanLeftThigh;
std::vector<Animation> AnimationResources::animsHumanLeftShin;
std::vector<Animation> AnimationResources::animsHumanLeftFoot;
std::vector<Animation> AnimationResources::animsHumanRightThigh;
std::vector<Animation> AnimationResources::animsHumanRightShin;
std::vector<Animation> AnimationResources::animsHumanRightFoot;


void AnimationResources::createAnimations()
{
	unsigned int animCount = 26;
	while (animsHumanBody.size()         < animCount) { Animation filler; animsHumanBody.push_back(filler); }
	while (animsHumanHead.size()         < animCount) { Animation filler; animsHumanHead.push_back(filler); }
	while (animsHumanLeftHumerus.size()  < animCount) { Animation filler; animsHumanLeftHumerus.push_back(filler); }
	while (animsHumanLeftForearm.size()  < animCount) { Animation filler; animsHumanLeftForearm.push_back(filler); }
	while (animsHumanLeftHand.size()     < animCount) { Animation filler; animsHumanLeftHand.push_back(filler); }
	while (animsHumanRightHumerus.size() < animCount) { Animation filler; animsHumanRightHumerus.push_back(filler); }
	while (animsHumanRightForearm.size() < animCount) { Animation filler; animsHumanRightForearm.push_back(filler); }
	while (animsHumanRightHand.size()    < animCount) { Animation filler; animsHumanRightHand.push_back(filler); }
	while (animsHumanLeftThigh.size()    < animCount) { Animation filler; animsHumanLeftThigh.push_back(filler); }
	while (animsHumanLeftShin.size()     < animCount) { Animation filler; animsHumanLeftShin.push_back(filler); }
	while (animsHumanLeftFoot.size()     < animCount) { Animation filler; animsHumanLeftFoot.push_back(filler); }
	while (animsHumanRightThigh.size()   < animCount) { Animation filler; animsHumanRightThigh.push_back(filler); }
	while (animsHumanRightShin.size()    < animCount) { Animation filler; animsHumanRightShin.push_back(filler); }
	while (animsHumanRightFoot.size()    < animCount) { Animation filler; animsHumanRightFoot.push_back(filler); }

	//stand animation
	animsHumanBody[0].addKeyframe(0, 3.35f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[0].addKeyframe(50, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[0].addKeyframe(100, 3.35f, 0, 0, 0, 0, 0, 0, 1);

	//run animation
	animsHumanBody[1].addKeyframe(0, 3.7f, -1, 0, 0, 0, 0, -15, 1);
	animsHumanBody[1].addKeyframe(25, 3.3f, 0, 0, 10, 0, 0, -15, 1);
	animsHumanBody[1].addKeyframe(50, 3.7f, -1, 0, 0, 0, 0, -15, 1);
	animsHumanBody[1].addKeyframe(75, 3.3f, 0, 0, -10, 0, 0, -15, 1);
	animsHumanBody[1].addKeyframe(100, 3.7f, -1, 0, 0, 0, 0, -15, 1);

	//jump animation
	animsHumanBody[2].addKeyframe(0, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[2].addKeyframe(100, 3.7f, 0, 0, 45, 0, 0, 0, 1);

	//double jump , now a stomp at time == 100
	animsHumanBody[3].addKeyframe(0, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[3].addKeyframe(100, 3.7f, 0, 0, 0, 0, 0, 0, 1);

	//triple jump animation
	animsHumanBody[4].addKeyframe(0, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[4].addKeyframe(100, 3.7f, 0, 0, 0, 0, 0, -720, 1);

	//side flip animation
	animsHumanBody[5].addKeyframe(0, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[5].addKeyframe(100, 3.7f, 0, 0, 0, 0, 0, -360, 1);

	//dive animation
	animsHumanBody[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);

	//rollout animation
	animsHumanBody[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanBody[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 45, 1);

	//skid animation
	animsHumanBody[8].addKeyframe(0, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[8].addKeyframe(100, 3, 0, 0, 0, 0, 0, 0, 1);

	//ground pound animation
	animsHumanBody[9].addKeyframe(0, 1, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[9].addKeyframe(100, 1, 0, 0, 0, 0, 0, 0, 1);
	
	//long jump animation
	animsHumanBody[10].addKeyframe(0, 3.7f, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[10].addKeyframe(100, 3.7f, 0, 0, 0, 0, 0, 0, 1);

	//getting hit animation
	animsHumanBody[11].addKeyframe(0, 1, 0, 0, 180, 0, 0, 0, 1);
	animsHumanBody[11].addKeyframe(100, 1, 0, 0, 180, 0, 0, 0, 1);

	//crouch animation
	animsHumanBody[12].addKeyframe(0, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanBody[12].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);

	//high jump animation
	animsHumanBody[13].addKeyframe(0, 2, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[13].addKeyframe(100, 2, 0, 0, 0, 0, 0, 0, 1);

	//dab animation
	animsHumanBody[14].addKeyframe(0,   3.54f,   0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[14].addKeyframe(100, 3.0385f, 0, 0, 0, 0, 0, 0, 1);

	//swim animation
	animsHumanBody[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanBody[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);

	//swim stroke animation
	animsHumanBody[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanBody[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);

	//t pose animation
	animsHumanBody[17].addKeyframe(0, 4, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[17].addKeyframe(100, 4, 0, 0, 0, 0, 0, 0, 1);

	//lightdash pose
	animsHumanBody[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, 3.3f, 1);
	animsHumanBody[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, 3.3f, 1);

	//dead pose
	animsHumanBody[19].addKeyframe(0,   0, 0, 0.5f, 180, 0, 0, 0, 1);
	animsHumanBody[19].addKeyframe(100, 0, 0, 0.5f, 180, 0, 0, 0, 1);

	//fly pose
	animsHumanBody[20].addKeyframe(0,   0, 1, 0, 0, 0, 0, 3.3f, 1);
	animsHumanBody[20].addKeyframe(100, 0, 1, 0, 0, 0, 0, 3.3f, 1);

	//free fall pose
	animsHumanBody[21].addKeyframe(0,   4, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[21].addKeyframe(100, 4, 0, 0, 0, 0, 0, 0, 1);

	//punch
	animsHumanBody[22].addKeyframe(0,   4, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[22].addKeyframe(100, 4, 0, 0, 0, 0, 0, 0, 1);

	//climb
	animsHumanBody[23].addKeyframe(0,   0, 2, 0, 0, 0, 0, 0, 1);
	animsHumanBody[23].addKeyframe(100, 0, 2, 0, 0, 0, 0, 0, 1);

	//metal sonic fly
	animsHumanBody[24].addKeyframe(0,   2.5f, 2.5f, 0, 0, 0, 0, -45, 1);
	animsHumanBody[24].addKeyframe(50,  3,    3,    0, 0, 0, 0, -50, 1);
	animsHumanBody[24].addKeyframe(100, 2.5f, 2.5f, 0, 0, 0, 0, -45, 1);

	//grab rocket
	animsHumanBody[25].addKeyframe(0,   4, 0, 0, 0, 0, 0, 0, 1);
	animsHumanBody[25].addKeyframe(100, 4, 0, 0, 0, 0, 0, 0, 1);




	animsHumanHead[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanHead[0].addKeyframe(33, 0, 0, 0, 60, 0, 0, 10, 1);
	animsHumanHead[0].addKeyframe(66, 0, 0, 0, -60, 0, 0, 10, 1);
	animsHumanHead[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanHead[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanHead[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, 15, 1);
	animsHumanHead[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanHead[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, 15, 1);
	animsHumanHead[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanHead[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanHead[2].addKeyframe(100, 0, 0, 0, -40, 0, 0, 20, 1);
	animsHumanHead[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanHead[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, 40, 1);
	animsHumanHead[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanHead[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanHead[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanHead[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanHead[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanHead[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanHead[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanHead[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanHead[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanHead[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanHead[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanHead[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanHead[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanHead[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanHead[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanHead[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, -40, 1);
	animsHumanHead[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, -40, 1);
	animsHumanHead[12].addKeyframe(0, 0, 0, 0, 0, 0, 0, 5, 1);
	animsHumanHead[12].addKeyframe(100, 0, 0, 0, 0, 0, 0, 5, 1);
	animsHumanHead[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanHead[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanHead[14].addKeyframe(0,   0, 0, 0,  0,  0, 0, 0,   1);
	animsHumanHead[14].addKeyframe(100, 0, 0, 0, -34.1f, 0, 0, -20.9f, 1);
	animsHumanHead[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanHead[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanHead[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanHead[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanHead[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanHead[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanHead[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, 57, 1);
	animsHumanHead[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, 57, 1);
	animsHumanHead[19].addKeyframe(0,   0, 0, 0, 65.7f, 0, 0, 0, 1);
	animsHumanHead[19].addKeyframe(100, 0, 0, 0, 65.7f, 0, 0, 0, 1);
	animsHumanHead[20].addKeyframe(0,   0, 0, 0, 0, 0, 0, 73, 1);
	animsHumanHead[20].addKeyframe(100, 0, 0, 0, 0, 0, 0, 73, 1);
	animsHumanHead[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, -33, 1);
	animsHumanHead[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, -33, 1);
	animsHumanHead[22].addKeyframe(0,   0, 0, 0, 0, 0, 0, -7, 1);
	animsHumanHead[22].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanHead[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, 40, 1);
	animsHumanHead[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanHead[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, 40, 1);
	animsHumanHead[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, 28.3f, 1);
	animsHumanHead[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, 28.3f, 1);
	animsHumanHead[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, 25.3f, 1);
	animsHumanHead[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, 25.3f, 1);

	animsHumanLeftHumerus[0].addKeyframe(0, 0, 0, 0, 110, 0, 0, -25, 1);
	animsHumanLeftHumerus[0].addKeyframe(50, 0, 0, 0, 100, 0, 0, -35, 1);
	animsHumanLeftHumerus[0].addKeyframe(100, 0, 0, 0, 110, 0, 0, -25, 1);
	animsHumanLeftHumerus[1].addKeyframe(0, 0, 0, 0, 165, 0, 0, -25, 1);
	animsHumanLeftHumerus[1].addKeyframe(50, 0, 0, 0, 80, 0, 0, -55, 1);
	animsHumanLeftHumerus[1].addKeyframe(100, 0, 0, 0, 165, 0, 0, -25, 1);
	animsHumanLeftHumerus[2].addKeyframe(0, 0, 0, 0, 90, 0, 0, -50, 1);
	animsHumanLeftHumerus[2].addKeyframe(100, 0, 0, 0, 90, 0, 0, -80, 1);
	animsHumanLeftHumerus[3].addKeyframe(0, 0, 0, 0, 90, 0, 0, -75, 1);
	animsHumanLeftHumerus[3].addKeyframe(75, 0, 0, 0, 90, 0, 0, -75, 1);
	animsHumanLeftHumerus[3].addKeyframe(100, 0, 0, 0, 140, 0, 0, 0, 1);
	animsHumanLeftHumerus[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftHumerus[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanLeftHumerus[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftHumerus[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanLeftHumerus[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftHumerus[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftHumerus[7].addKeyframe(0, 0, 0, 0, -20, 0, 0, 135, 1);
	animsHumanLeftHumerus[7].addKeyframe(100, 0, 0, 0, -20, 0, 0, 135, 1);
	animsHumanLeftHumerus[8].addKeyframe(0, 0, 0, 0, 170, 0, 0, 5, 1);
	animsHumanLeftHumerus[8].addKeyframe(100, 0, 0, 0, 155, 0, 0, 15, 1);
	animsHumanLeftHumerus[9].addKeyframe(0, 0, 0, 0, 135, 0, 0, 45, 1);
	animsHumanLeftHumerus[9].addKeyframe(100, 0, 0, 0, 135, 0, 0, 45, 1);
	animsHumanLeftHumerus[10].addKeyframe(0, 0, 0, 0, 135, 0, 0, -10, 1);
	animsHumanLeftHumerus[10].addKeyframe(100, 0, 0, 0, 20, 0, 0, -20, 1);
	animsHumanLeftHumerus[11].addKeyframe(0, 0, 0, 0, 10, 0, 0, -10, 1);
	animsHumanLeftHumerus[11].addKeyframe(100, 0, 0, 0, 10, 0, 0, -10, 1);
	animsHumanLeftHumerus[12].addKeyframe(0, 0, 0, 0, 45, 0, 0, 35, 1);
	animsHumanLeftHumerus[12].addKeyframe(100, 0, 0, 0, 45, 0, 0, 35, 1);
	animsHumanLeftHumerus[13].addKeyframe(0, 0, 0, 0, 5, 0, 0, -130, 1);
	animsHumanLeftHumerus[13].addKeyframe(100, 0, 0, 0, 5, 0, 0, -130, 1);
	animsHumanLeftHumerus[14].addKeyframe(0,   0, 0, 0, 110, 0, 0, -25, 1);
	animsHumanLeftHumerus[14].addKeyframe(100, 0, 0, 0, 100, 0, 0, 25.9f,  1);
	animsHumanLeftHumerus[15].addKeyframe(0, 0, 0, 0, -30, 0, 0, -90, 1);
	animsHumanLeftHumerus[15].addKeyframe(100, 0, 0, 0, -30, 0, 0, -90, 1);
	animsHumanLeftHumerus[16].addKeyframe(0, 0, 0, 0, -150, 0, 0, -90, 1);
	animsHumanLeftHumerus[16].addKeyframe(50, 0, 0, 0, -30, 0, 0, -90, 1);
	animsHumanLeftHumerus[16].addKeyframe(100, 0, 0, 0, -30, 0, 0, -90, 1);
	animsHumanLeftHumerus[17].addKeyframe(0, 0, 0, 0, 90, 0, 0, 0, 1);
	animsHumanLeftHumerus[17].addKeyframe(100, 0, 0, 0, 90, 0, 0, 0, 1);
	animsHumanLeftHumerus[18].addKeyframe(0,   0, 0, 0, 84, 0, 0, 32.4f, 1);
	animsHumanLeftHumerus[18].addKeyframe(100, 0, 0, 0, 84, 0, 0, 32.4f, 1);
	animsHumanLeftHumerus[19].addKeyframe(0,   0, 0, 0, 88, 0, 0, 33.6f, 1);
	animsHumanLeftHumerus[19].addKeyframe(100, 0, 0, 0, 88, 0, 0, 33.6f, 1);
	animsHumanLeftHumerus[20].addKeyframe(0,   0, 0, 0, 84, 0, 0, 32.4f, 1);
	animsHumanLeftHumerus[20].addKeyframe(100, 0, 0, 0, 84, 0, 0, 32.4f, 1);
	animsHumanLeftHumerus[21].addKeyframe(0,   0, 0, 0, 148, 0, 0, 23, 1);
	animsHumanLeftHumerus[21].addKeyframe(100, 0, 0, 0, 148, 0, 0, 23, 1);
	animsHumanLeftHumerus[22].addKeyframe(0,   0, 0, 0, 63, 0, 0, -8.1f, 1);
	animsHumanLeftHumerus[22].addKeyframe(100, 0, 0, 0, 146, 0, 0, -8.1f, 1);
	animsHumanLeftHumerus[23].addKeyframe(0,   0, 0, 0, 49.6f, 0, 0, -30.6f, 1);
	animsHumanLeftHumerus[23].addKeyframe(50,  0, 0, 0, 57.3f, 0, 0,  21.1f, 1); //right humerus, -xrot
	animsHumanLeftHumerus[23].addKeyframe(100, 0, 0, 0, 49.6f, 0, 0, -30.6f, 1);
	animsHumanLeftHumerus[24].addKeyframe(0,   0, 0, 0, 160, 0, 0, 64, 1);
	animsHumanLeftHumerus[24].addKeyframe(100, 0, 0, 0, 160, 0, 0, 64, 1);
	animsHumanLeftHumerus[25].addKeyframe(0,   0, 0, 0, 31, 0, 0, 25, 1);
	animsHumanLeftHumerus[25].addKeyframe(100, 0, 0, 0, 31, 0, 0, 25, 1);

	animsHumanLeftForearm[0].addKeyframe(0, 0, 0, 0, -120, 0, 0, -25, 1);
	animsHumanLeftForearm[0].addKeyframe(50, 0, 0, 0, -120, 0, 0, 0, 1);
	animsHumanLeftForearm[0].addKeyframe(100, 0, 0, 0, -120, 0, 0, -25, 1);
	animsHumanLeftForearm[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftForearm[1].addKeyframe(50, 0, 0, 0, -80, 0, 0, 90, 1);
	animsHumanLeftForearm[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftForearm[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftForearm[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[9].addKeyframe(0, 0, 0, 0, -90, 0, 0, -90, 1);
	animsHumanLeftForearm[9].addKeyframe(100, 0, 0, 0, -90, 0, 0, -90, 1);
	animsHumanLeftForearm[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[12].addKeyframe(0, 0, 0, 0, -20, 0, 0, 20, 1);
	animsHumanLeftForearm[12].addKeyframe(100, 0, 0, 0, -20, 0, 0, 20, 1);
	animsHumanLeftForearm[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[14].addKeyframe(0,   0, 0, 0, -120, 0, 0, -25,   1);
	animsHumanLeftForearm[14].addKeyframe(100, 0, 0, 0, 0,    0, 0, -2.9f, 1);
	animsHumanLeftForearm[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftForearm[18].addKeyframe(0,   0, 0, 0, -45.3f, 0, 0, 59.8f, 1);
	animsHumanLeftForearm[18].addKeyframe(100, 0, 0, 0, -45.3f, 0, 0, 59.8f, 1);
	animsHumanLeftForearm[19].addKeyframe(0,   0, 0, 0, 0.9f, 0, 0, -44.7f, 1);
	animsHumanLeftForearm[19].addKeyframe(100, 0, 0, 0, 0.9f, 0, 0, -44.7f, 1);
	animsHumanLeftForearm[20].addKeyframe(0,   0, 0, 0, -45.3f, 0, 0, 59.8f, 1);
	animsHumanLeftForearm[20].addKeyframe(100, 0, 0, 0, -45.3f, 0, 0, 59.8f, 1);
	animsHumanLeftForearm[21].addKeyframe(0,   0, 0, 0, -36, 0, 0, 0, 1);
	animsHumanLeftForearm[21].addKeyframe(100, 0, 0, 0, -36, 0, 0, 0, 1);
	animsHumanLeftForearm[22].addKeyframe(0,   0, 0, 0, 116, 0, 0, 230, 1);
	animsHumanLeftForearm[22].addKeyframe(100, 0, 0, 0, 173, 0, 0, 212, 1);
	animsHumanLeftForearm[23].addKeyframe(0,   0, 0, 0, -23.8f, 0, 0, -19.7f, 1);
	animsHumanLeftForearm[23].addKeyframe(50,  0, 0, 0, -48.5f, 0, 0,  30.1f, 1); //right forearm, -xrot
	animsHumanLeftForearm[23].addKeyframe(100, 0, 0, 0, -23.8f, 0, 0, -19.7f, 1);
	animsHumanLeftForearm[24].addKeyframe(0,   0, 0, 0, -1.6f, 0, 0, -13.8f, 1);
	animsHumanLeftForearm[24].addKeyframe(100, 0, 0, 0, -1.6f, 0, 0, -13.8f, 1);
	animsHumanLeftForearm[25].addKeyframe(0,   0, 0, 0, -27, 0, 0, 24, 1);
	animsHumanLeftForearm[25].addKeyframe(100, 0, 0, 0, -27, 0, 0, 24, 1);

	animsHumanLeftHand[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[12].addKeyframe(0, 0, 0, 0, 10, 0, 0, 30, 1);
	animsHumanLeftHand[12].addKeyframe(100, 0, 0, 0, 10, 0, 0, 30, 1);
	animsHumanLeftHand[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[14].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[14].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[19].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[19].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[20].addKeyframe(0,   0, 0, 0, 0, 43, 0, 19, 1);
	animsHumanLeftHand[20].addKeyframe(100, 0, 0, 0, 0, 43, 0, 19, 1);
	animsHumanLeftHand[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftHand[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftHand[22].addKeyframe(0,   0, 0, 0, 180, 0, 0, 180, 1);
	animsHumanLeftHand[22].addKeyframe(100, 0, 0, 0, 180, 0, 0, 180, 1);
	animsHumanLeftHand[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftHand[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);

	animsHumanRightHumerus[0].addKeyframe(0, 0, 0, 0, -110, 0, 0, -25, 1);
	animsHumanRightHumerus[0].addKeyframe(50, 0, 0, 0, -100, 0, 0, -35, 1);
	animsHumanRightHumerus[0].addKeyframe(100, 0, 0, 0, -110, 0, 0, -25, 1);
	animsHumanRightHumerus[1].addKeyframe(0, 0, 0, 0, -80, 0, 0, -55, 1);
	animsHumanRightHumerus[1].addKeyframe(50, 0, 0, 0, -165, 0, 0, -25, 1);
	animsHumanRightHumerus[1].addKeyframe(100, 0, 0, 0, -80, 0, 0, -55, 1);
	animsHumanRightHumerus[2].addKeyframe(0, 0, 0, 0, -90, 0, 0, 50, 1);
	animsHumanRightHumerus[2].addKeyframe(100, 0, 0, 0, -90, 0, 0, 80, 1);
	animsHumanRightHumerus[3].addKeyframe(0, 0, 0, 0, -90, 0, 0, -75, 1);
	animsHumanRightHumerus[3].addKeyframe(75, 0, 0, 0, -90, 0, 0, -75, 1);
	animsHumanRightHumerus[3].addKeyframe(100, 0, 0, 0, -140, 0, 0, 0, 1);
	animsHumanRightHumerus[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightHumerus[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanRightHumerus[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightHumerus[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanRightHumerus[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightHumerus[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightHumerus[7].addKeyframe(0, 0, 0, 0, 20, 0, 0, 135, 1);
	animsHumanRightHumerus[7].addKeyframe(100, 0, 0, 0, 20, 0, 0, 135, 1);
	animsHumanRightHumerus[8].addKeyframe(0, 0, 0, 0, -170, 0, 0, 5, 1);
	animsHumanRightHumerus[8].addKeyframe(100, 0, 0, 0, -155, 0, 0, 15, 1);
	animsHumanRightHumerus[9].addKeyframe(0, 0, 0, 0, -135, 0, 0, 45, 1);
	animsHumanRightHumerus[9].addKeyframe(100, 0, 0, 0, -135, 0, 0, 45, 1);
	animsHumanRightHumerus[10].addKeyframe(0, 0, 0, 0, -135, 0, 0, -10, 1);
	animsHumanRightHumerus[10].addKeyframe(100, 0, 0, 0, -20, 0, 0, -20, 1);
	animsHumanRightHumerus[11].addKeyframe(0, 0, 0, 0, -10, 0, 0, -10, 1);
	animsHumanRightHumerus[11].addKeyframe(100, 0, 0, 0, -10, 0, 0, -10, 1);
	animsHumanRightHumerus[12].addKeyframe(0, 0, 0, 0, -45, 0, 0, 35, 1);
	animsHumanRightHumerus[12].addKeyframe(100, 0, 0, 0, -45, 0, 0, 35, 1);
	animsHumanRightHumerus[13].addKeyframe(0, 0, 0, 0, -5, 0, 0, -130, 1);
	animsHumanRightHumerus[13].addKeyframe(100, 0, 0, 0, -5, 0, 0, -130, 1);
	animsHumanRightHumerus[14].addKeyframe(0,   0, 0, 0, -110,   0, 0, -25,  1);
	animsHumanRightHumerus[14].addKeyframe(100, 0, 0, 0, -65.3f, 0, 0, 4.5f, 1);
	animsHumanRightHumerus[15].addKeyframe(0, 0, 0, 0, 30, 0, 0, -90, 1);
	animsHumanRightHumerus[15].addKeyframe(100, 0, 0, 0, 30, 0, 0, -90, 1);
	animsHumanRightHumerus[16].addKeyframe(0, 0, 0, 0, 150, 0, 0, -90, 1);
	animsHumanRightHumerus[16].addKeyframe(50, 0, 0, 0, 30, 0, 0, -90, 1);
	animsHumanRightHumerus[16].addKeyframe(100, 0, 0, 0, 30, 0, 0, -90, 1);
	animsHumanRightHumerus[17].addKeyframe(0, 0, 0, 0, -90, 0, 0, 0, 1);
	animsHumanRightHumerus[17].addKeyframe(100, 0, 0, 0, -90, 0, 0, 0, 1);
	animsHumanRightHumerus[18].addKeyframe(0,   0, 0, 0, -110, 0, 0, -25, 1);
	animsHumanRightHumerus[18].addKeyframe(100, 0, 0, 0, -110, 0, 0, -25, 1);
	animsHumanRightHumerus[19].addKeyframe(0,   0, 0, 0, -84.6f, 0, 0, -25, 1);
	animsHumanRightHumerus[19].addKeyframe(100, 0, 0, 0, -84.6f, 0, 0, -25, 1);
	animsHumanRightHumerus[20].addKeyframe(0,   0, 0, 0, -84, 0, 0, 32.4f, 1);
	animsHumanRightHumerus[20].addKeyframe(100, 0, 0, 0, -84, 0, 0, 32.4f, 1);
	animsHumanRightHumerus[21].addKeyframe(0,   0, 0, 0, -148, 0, 0, 23, 1);
	animsHumanRightHumerus[21].addKeyframe(100, 0, 0, 0, -148, 0, 0, 23, 1);
	animsHumanRightHumerus[22].addKeyframe(0,   0, 0, 0, -153, 0, 0, 2.4f, 1);
	animsHumanRightHumerus[22].addKeyframe(100, 0, 0, 0, -46.2f, 0, 0, 2.4f, 1);
	animsHumanRightHumerus[23].addKeyframe(0,   0, 0, 0, -57.3f, 0, 0,  21.2f, 1);
	animsHumanRightHumerus[23].addKeyframe(50,  0, 0, 0, -49.6f, 0, 0, -30.6f, 1); //left humerus, -xrot
	animsHumanRightHumerus[23].addKeyframe(100, 0, 0, 0, -57.3f, 0, 0,  21.2f, 1);
	animsHumanRightHumerus[24].addKeyframe(0,   0, 0, 0, -160, 0, 0, 64, 1);
	animsHumanRightHumerus[24].addKeyframe(100, 0, 0, 0, -160, 0, 0, 64, 1);
	animsHumanRightHumerus[25].addKeyframe(0,   0, 0, 0, -31, 0, 0, 25, 1);
	animsHumanRightHumerus[25].addKeyframe(100, 0, 0, 0, -31, 0, 0, 25, 1);

	animsHumanRightForearm[0].addKeyframe(0, 0, 0, 0, 120, 0, 0, -25, 1);
	animsHumanRightForearm[0].addKeyframe(50, 0, 0, 0, 120, 0, 0, 0, 1);
	animsHumanRightForearm[0].addKeyframe(100, 0, 0, 0, 120, 0, 0, -25, 1);
	animsHumanRightForearm[1].addKeyframe(0, 0, 0, 0, 80, 0, 0, 90, 1);
	animsHumanRightForearm[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanRightForearm[1].addKeyframe(100, 0, 0, 0, 80, 0, 0, 90, 1);
	animsHumanRightForearm[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightForearm[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[9].addKeyframe(0, 0, 0, 0, 90, 0, 0, -90, 1);
	animsHumanRightForearm[9].addKeyframe(100, 0, 0, 0, 90, 0, 0, -90, 1);
	animsHumanRightForearm[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[12].addKeyframe(0, 0, 0, 0, 20, 0, 0, 20, 1);
	animsHumanRightForearm[12].addKeyframe(100, 0, 0, 0, 20, 0, 0, 20, 1);
	animsHumanRightForearm[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[14].addKeyframe(0,   0, 0, 0, 120,   0, 0, -25,   1);
	animsHumanRightForearm[14].addKeyframe(100, 0, 0, 0, 88.5f, 0, 0, 20.8f, 1);
	animsHumanRightForearm[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightForearm[18].addKeyframe(0,   0, 0, 0, 50.5f, 0, 0, 60.4f, 1);
	animsHumanRightForearm[18].addKeyframe(100, 0, 0, 0, 50.5f, 0, 0, 60.4f, 1);
	animsHumanRightForearm[19].addKeyframe(0,   0, 0, 0, 2.9f, 0, 0, -32, 1);
	animsHumanRightForearm[19].addKeyframe(100, 0, 0, 0, 2.9f, 0, 0, -32, 1);
	animsHumanRightForearm[20].addKeyframe(0,   0, 0, 0, 45.3f, 0, 0, 59.8f, 1);
	animsHumanRightForearm[20].addKeyframe(100, 0, 0, 0, 45.3f, 0, 0, 59.8f, 1);
	animsHumanRightForearm[21].addKeyframe(0,   0, 0, 0, 36, 0, 0, 0, 1);
	animsHumanRightForearm[21].addKeyframe(100, 0, 0, 0, 36, 0, 0, 0, 1);
	animsHumanRightForearm[22].addKeyframe(0,   0, 0, 0, -99.4f, 0, 0, 200, 1);
	animsHumanRightForearm[22].addKeyframe(100, 0, 0, 0, -146, 0, 0, 179, 1);
	animsHumanRightForearm[23].addKeyframe(0,   0, 0, 0, 48.5f, 0, 0,  30.1f, 1);
	animsHumanRightForearm[23].addKeyframe(50,  0, 0, 0, 23.8f, 0, 0, -19.7f, 1); //left forearm, -xrot
	animsHumanRightForearm[23].addKeyframe(100, 0, 0, 0, 48.5f, 0, 0,  30.1f, 1);
	animsHumanRightForearm[24].addKeyframe(0,   0, 0, 0, 1.6f, 0, 0, -13.8f, 1);
	animsHumanRightForearm[24].addKeyframe(100, 0, 0, 0, 1.6f, 0, 0, -13.8f, 1);
	animsHumanRightForearm[25].addKeyframe(0,   0, 0, 0, 27, 0, 0, 24, 1);
	animsHumanRightForearm[25].addKeyframe(100, 0, 0, 0, 27, 0, 0, 24, 1);

	animsHumanRightHand[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[12].addKeyframe(0, 0, 0, 0, 10, 0, 0, 30, 1);
	animsHumanRightHand[12].addKeyframe(100, 0, 0, 0, 10, 0, 0, 30, 1);
	animsHumanRightHand[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[14].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[14].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[19].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[19].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[20].addKeyframe(0,   0, 0, 0, 0, -43, 0, 19, 1);
	animsHumanRightHand[20].addKeyframe(100, 0, 0, 0, 0, -43, 0, 19, 1);
	animsHumanRightHand[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightHand[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightHand[22].addKeyframe(0,   0, 0, 0, 180, 0, 0, 180, 1);
	animsHumanRightHand[22].addKeyframe(100, 0, 0, 0, 180, 0, 0, 180, 2);
	animsHumanRightHand[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightHand[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);

	animsHumanLeftThigh[0].addKeyframe(0, 0, 0, 0, 50, 0, 0, -55, 1);
	animsHumanLeftThigh[0].addKeyframe(50, 0, 0, 0, 50, 0, 0, -70, 1);
	animsHumanLeftThigh[0].addKeyframe(100, 0, 0, 0, 50, 0, 0, -55, 1);
	animsHumanLeftThigh[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, 15, 1);
	animsHumanLeftThigh[1].addKeyframe(5, 0, 0, 0, 0, 0, 0, 5, 1);
	animsHumanLeftThigh[1].addKeyframe(10, 0, 0, 0, 0, 0, 0, -50, 1);
	animsHumanLeftThigh[1].addKeyframe(15, 0, 0, 0, 0, 0, 0, -55, 1);
	animsHumanLeftThigh[1].addKeyframe(20, 0, 0, 0, 0, 0, 0, -60, 1);
	animsHumanLeftThigh[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[1].addKeyframe(30, 0, 0, 0, 0, 0, 0, -100, 1);
	animsHumanLeftThigh[1].addKeyframe(35, 0, 0, 0, 0, 0, 0, -120, 1);
	animsHumanLeftThigh[1].addKeyframe(40, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftThigh[1].addKeyframe(45, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftThigh[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -135, 1);
	animsHumanLeftThigh[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -135, 1);
	animsHumanLeftThigh[1].addKeyframe(55, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftThigh[1].addKeyframe(60, 0, 0, 0, 0, 0, 0, -125, 1);
	animsHumanLeftThigh[1].addKeyframe(65, 0, 0, 0, 0, 0, 0, -115, 1);
	animsHumanLeftThigh[1].addKeyframe(70, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftThigh[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftThigh[1].addKeyframe(80, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftThigh[1].addKeyframe(85, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftThigh[1].addKeyframe(90, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftThigh[1].addKeyframe(95, 0, 0, 0, 0, 0, 0, 12, 1);
	animsHumanLeftThigh[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftThigh[2].addKeyframe(100, 0, 0, 0, -20, 0, 0, 20, 1);
	animsHumanLeftThigh[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, -80, 1);
	animsHumanLeftThigh[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, -80, 1);
	animsHumanLeftThigh[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanLeftThigh[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanLeftThigh[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanLeftThigh[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanLeftThigh[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanLeftThigh[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftThigh[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanLeftThigh[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanLeftThigh[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftThigh[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanLeftThigh[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftThigh[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftThigh[12].addKeyframe(0, 0, 0, 0, 20, 0, 0, 0, 1);
	animsHumanLeftThigh[12].addKeyframe(100, 0, 0, 0, 20, 0, 0, 0, 1);
	animsHumanLeftThigh[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftThigh[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftThigh[14].addKeyframe(0,   0, 0, 0, 50, 0, 0, -55,    1);
	animsHumanLeftThigh[14].addKeyframe(100, 0, 0, 0, 50, 0, 0, -37.2f, 1);
	animsHumanLeftThigh[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftThigh[15].addKeyframe(50, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanLeftThigh[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftThigh[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftThigh[16].addKeyframe(50, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanLeftThigh[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanLeftThigh[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftThigh[18].addKeyframe(0,   0, 0, 0, 22.5f, 0, 0, -11.6f, 1);
	animsHumanLeftThigh[18].addKeyframe(100, 0, 0, 0, 22.5f, 0, 0, -11.6f, 1);
	animsHumanLeftThigh[19].addKeyframe(0,   0, 0, 0, 85.8f, 0, 0, -20.6f, 1);
	animsHumanLeftThigh[19].addKeyframe(100, 0, 0, 0, 85.8f, 0, 0, -20.6f, 1);
	animsHumanLeftThigh[20].addKeyframe(0,   0, 0, 0, 70, 0, 0, -30, 1);
	animsHumanLeftThigh[20].addKeyframe(100, 0, 0, 0, 70, 0, 0, -30, 1);
	animsHumanLeftThigh[21].addKeyframe(0,   0, 0, 0, 70, 0, 0, -40.8f, 1);
	animsHumanLeftThigh[21].addKeyframe(100, 0, 0, 0, 70, 0, 0, -40.8f, 1);
	animsHumanLeftThigh[22].addKeyframe(0,   0, 0, 0, -4.7f, 0, 0, -109, 1);
	animsHumanLeftThigh[22].addKeyframe(100, 0, 0, 0, 10.9f, 0, 0, -31.1f, 1);
	animsHumanLeftThigh[23].addKeyframe(0,   0, 0, 0, 1,     0, 0, -77.7f, 1);
	animsHumanLeftThigh[23].addKeyframe(50,  0, 0, 0, 30.4f, 0, 0, -27,    1); //right thigh, -xrot
	animsHumanLeftThigh[23].addKeyframe(100, 0, 0, 0, 1,     0, 0, -77.7f, 1);
	animsHumanLeftThigh[24].addKeyframe(0,   0, 0, 0, -10,   0, 0, -95.9f, 1);
	animsHumanLeftThigh[24].addKeyframe(100, 0, 0, 0, -10,   0, 0, -95.9f, 1);
	animsHumanLeftThigh[25].addKeyframe(0,   0, 0, 0, -11,   0, 0, -110, 1);
	animsHumanLeftThigh[25].addKeyframe(100, 0, 0, 0, -11,   0, 0, -110, 1);

	animsHumanLeftShin[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftShin[0].addKeyframe(50, 0, 0, 0, 0, 0, 0, -40, 1);
	animsHumanLeftShin[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftShin[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, -40, 1);
	animsHumanLeftShin[1].addKeyframe(5, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanLeftShin[1].addKeyframe(10, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftShin[1].addKeyframe(15, 0, 0, 0, 0, 0, 0, -17, 1);
	animsHumanLeftShin[1].addKeyframe(20, 0, 0, 0, 0, 0, 0, -17, 1);
	animsHumanLeftShin[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, -5, 1);
	animsHumanLeftShin[1].addKeyframe(30, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[1].addKeyframe(35, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[1].addKeyframe(40, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[1].addKeyframe(45, 0, 0, 0, 0, 0, 0, -15, 1);
	animsHumanLeftShin[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftShin[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftShin[1].addKeyframe(55, 0, 0, 0, 0, 0, 0, -25, 1);
	animsHumanLeftShin[1].addKeyframe(60, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanLeftShin[1].addKeyframe(65, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanLeftShin[1].addKeyframe(70, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftShin[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, -125, 1);
	animsHumanLeftShin[1].addKeyframe(80, 0, 0, 0, 0, 0, 0, -140, 1);
	animsHumanLeftShin[1].addKeyframe(85, 0, 0, 0, 0, 0, 0, -135, 1);
	animsHumanLeftShin[1].addKeyframe(90, 0, 0, 0, 0, 0, 0, -115, 1);
	animsHumanLeftShin[1].addKeyframe(95, 0, 0, 0, 0, 0, 0, -80, 1);
	animsHumanLeftShin[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftShin[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftShin[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftShin[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanLeftShin[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftShin[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftShin[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanLeftShin[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanLeftShin[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, -60, 1);
	animsHumanLeftShin[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftShin[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftShin[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftShin[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftShin[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[12].addKeyframe(0, 0, 0, 0, 0, 0, 0, -120, 1);
	animsHumanLeftShin[12].addKeyframe(100, 0, 0, 0, 0, 0, 0, -120, 1);
	animsHumanLeftShin[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[14].addKeyframe(0,   0, 0, 0, 0, 0, 0, -70,   1);
	animsHumanLeftShin[14].addKeyframe(100, 0, 0, 0, 0, 0, 0, -1.4f, 1);
	animsHumanLeftShin[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftShin[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftShin[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanLeftShin[19].addKeyframe(0,   0, 0, 0, 0, 0, 0, -94.6f, 1);
	animsHumanLeftShin[19].addKeyframe(100, 0, 0, 0, 0, 0, 0, -94.6f, 1);
	animsHumanLeftShin[20].addKeyframe(0,   0, 0, 0, 0, 0, 0, -64, 1);
	animsHumanLeftShin[20].addKeyframe(100, 0, 0, 0, 0, 0, 0, -64, 1);
	animsHumanLeftShin[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, -51.8f, 1);
	animsHumanLeftShin[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, -51.8f, 1);
	animsHumanLeftShin[22].addKeyframe(0,   0, 0, 0, 0, 0, 0, -53, 1);
	animsHumanLeftShin[22].addKeyframe(100, 0, 0, 0, 0, 0, 0, -23.4f, 1);
	animsHumanLeftShin[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, -18.6f, 1);
	animsHumanLeftShin[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, -120,   1); //right shin, -xrot
	animsHumanLeftShin[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, -18.6f, 1);
	animsHumanLeftShin[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, -19, 1);
	animsHumanLeftShin[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, -19, 1);
	animsHumanLeftShin[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanLeftShin[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);

	animsHumanLeftFoot[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, 125, 1);
	animsHumanLeftFoot[0].addKeyframe(50, 0, 0, 0, 0, 0, 0, 110, 1);
	animsHumanLeftFoot[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, 125, 1);
	animsHumanLeftFoot[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanLeftFoot[1].addKeyframe(5, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanLeftFoot[1].addKeyframe(10, 0, 0, 0, 0, 0, 0, 95, 1);
	animsHumanLeftFoot[1].addKeyframe(15, 0, 0, 0, 0, 0, 0, 95, 1);
	animsHumanLeftFoot[1].addKeyframe(20, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanLeftFoot[1].addKeyframe(30, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanLeftFoot[1].addKeyframe(35, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[1].addKeyframe(40, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftFoot[1].addKeyframe(45, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftFoot[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftFoot[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanLeftFoot[1].addKeyframe(55, 0, 0, 0, 0, 0, 0, 15, 1);
	animsHumanLeftFoot[1].addKeyframe(60, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[1].addKeyframe(65, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanLeftFoot[1].addKeyframe(70, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanLeftFoot[1].addKeyframe(80, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftFoot[1].addKeyframe(85, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[1].addKeyframe(90, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanLeftFoot[1].addKeyframe(95, 0, 0, 0, 0, 0, 0, 115, 1);
	animsHumanLeftFoot[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanLeftFoot[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanLeftFoot[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftFoot[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftFoot[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanLeftFoot[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanLeftFoot[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanLeftFoot[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanLeftFoot[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanLeftFoot[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanLeftFoot[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanLeftFoot[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanLeftFoot[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanLeftFoot[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanLeftFoot[12].addKeyframe(0, 0, 0, 0, 0, 0, 0, 150, 1);
	animsHumanLeftFoot[12].addKeyframe(100, 0, 0, 0, 0, 0, 0, 150, 1);
	animsHumanLeftFoot[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[14].addKeyframe(0,   0, 0, 0, 0, 0, 0, 125, 1);
	animsHumanLeftFoot[14].addKeyframe(100, 0, 0, 0, 0, 0, 0, 39,  1);
	animsHumanLeftFoot[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanLeftFoot[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanLeftFoot[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, 36.2f, 1);
	animsHumanLeftFoot[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, 36.2f, 1);
	animsHumanLeftFoot[19].addKeyframe(0,   0, 0, 0, 0, 0, 0, 81.6f, 1);
	animsHumanLeftFoot[19].addKeyframe(100, 0, 0, 0, 0, 0, 0, 81.6f, 1);
	animsHumanLeftFoot[20].addKeyframe(0,   0, 0, 0, 0, 0, 0, 35.5f, 1);
	animsHumanLeftFoot[20].addKeyframe(100, 0, 0, 0, 0, 0, 0, 35.5f, 1);
	animsHumanLeftFoot[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, 62, 1);
	animsHumanLeftFoot[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, 62, 1);
	animsHumanLeftFoot[22].addKeyframe(0,   0, 0, 0, 0, 0, 0, 133, 1);
	animsHumanLeftFoot[22].addKeyframe(100, 0, 0, 0, 0, 0, 0, 67.2f, 1);
	animsHumanLeftFoot[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, 48.4f, 1);
	animsHumanLeftFoot[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, 157,   1); //right foot, -xrot
	animsHumanLeftFoot[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, 48.4f, 1);
	animsHumanLeftFoot[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, 44.7f, 1);
	animsHumanLeftFoot[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, 44.7f, 1);
	animsHumanLeftFoot[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, 32, 1);
	animsHumanLeftFoot[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, 32, 1);

	animsHumanRightThigh[0].addKeyframe(0, 0, 0, 0, -50, 0, 0, -55, 1);
	animsHumanRightThigh[0].addKeyframe(50, 0, 0, 0, -50, 0, 0, -70, 1);
	animsHumanRightThigh[0].addKeyframe(100, 0, 0, 0, -50, 0, 0, -55, 1);
	animsHumanRightThigh[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, -135, 1);
	animsHumanRightThigh[1].addKeyframe(5, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightThigh[1].addKeyframe(10, 0, 0, 0, 0, 0, 0, -125, 1);
	animsHumanRightThigh[1].addKeyframe(15, 0, 0, 0, 0, 0, 0, -115, 1);
	animsHumanRightThigh[1].addKeyframe(20, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightThigh[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightThigh[1].addKeyframe(30, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightThigh[1].addKeyframe(35, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightThigh[1].addKeyframe(40, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightThigh[1].addKeyframe(45, 0, 0, 0, 0, 0, 0, 12, 1);
	animsHumanRightThigh[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, 15, 1);
	animsHumanRightThigh[1].addKeyframe(55, 0, 0, 0, 0, 0, 0, 5, 1);
	animsHumanRightThigh[1].addKeyframe(60, 0, 0, 0, 0, 0, 0, -50, 1);
	animsHumanRightThigh[1].addKeyframe(65, 0, 0, 0, 0, 0, 0, -55, 1);
	animsHumanRightThigh[1].addKeyframe(70, 0, 0, 0, 0, 0, 0, -60, 1);
	animsHumanRightThigh[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[1].addKeyframe(80, 0, 0, 0, 0, 0, 0, -100, 1);
	animsHumanRightThigh[1].addKeyframe(85, 0, 0, 0, 0, 0, 0, -120, 1);
	animsHumanRightThigh[1].addKeyframe(90, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightThigh[1].addKeyframe(95, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightThigh[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, -135, 1);
	animsHumanRightThigh[2].addKeyframe(0, 0, 0, 0, -20, 0, 0, -90, 1);
	animsHumanRightThigh[2].addKeyframe(100, 0, 0, 0, -50, 0, 0, -90, 1);
	animsHumanRightThigh[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, -95, 1);
	animsHumanRightThigh[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, -95, 1);
	animsHumanRightThigh[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, -80, 1);
	animsHumanRightThigh[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanRightThigh[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanRightThigh[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanRightThigh[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanRightThigh[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightThigh[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanRightThigh[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, 20, 1);
	animsHumanRightThigh[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightThigh[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanRightThigh[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightThigh[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightThigh[12].addKeyframe(0, 0, 0, 0, -20, 0, 0, 0, 1);
	animsHumanRightThigh[12].addKeyframe(100, 0, 0, 0, -20, 0, 0, 0, 1);
	animsHumanRightThigh[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightThigh[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightThigh[14].addKeyframe(0,   0, 0, 0, -50, 0, 0, -55,    1);
	animsHumanRightThigh[14].addKeyframe(100, 0, 0, 0, -50, 0, 0, -22.3f, 1);
	animsHumanRightThigh[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanRightThigh[15].addKeyframe(50, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightThigh[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanRightThigh[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightThigh[16].addKeyframe(50, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanRightThigh[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, -110, 1);
	animsHumanRightThigh[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightThigh[18].addKeyframe(0,   0, 0, 0, -50, 0, 0, -55, 1);
	animsHumanRightThigh[18].addKeyframe(100, 0, 0, 0, -50, 0, 0, -55, 1);
	animsHumanRightThigh[19].addKeyframe(0,   0, 0, 0, -87.8f, 0, 0, -35.3f, 1);
	animsHumanRightThigh[19].addKeyframe(100, 0, 0, 0, -87.8f, 0, 0, -35.3f, 1);
	animsHumanRightThigh[20].addKeyframe(0,   0, 0, 0, -70, 0, 0, -30, 1);
	animsHumanRightThigh[20].addKeyframe(100, 0, 0, 0, -70, 0, 0, -30, 1);
	animsHumanRightThigh[21].addKeyframe(0,   0, 0, 0, -23.5f, 0, 0, -17, 1);
	animsHumanRightThigh[21].addKeyframe(100, 0, 0, 0, -23.5f, 0, 0, -17, 1);
	animsHumanRightThigh[22].addKeyframe(0,   0, 0, 0, -30.4f, 0, 0, -37.9f, 1);
	animsHumanRightThigh[22].addKeyframe(100, 0, 0, 0, 4.5f, 0, 0, -114, 1);
	animsHumanRightThigh[23].addKeyframe(0,   0, 0, 0, -30.4f, 0, 0, -27,    1);
	animsHumanRightThigh[23].addKeyframe(50,  0, 0, 0, -1,     0, 0, -77.7f, 1); //left thigh, -xrot
	animsHumanRightThigh[23].addKeyframe(100, 0, 0, 0, -30.4f, 0, 0, -27,    1);
	animsHumanRightThigh[24].addKeyframe(0,   0, 0, 0,  10,   0, 0, -95.9f, 1);
	animsHumanRightThigh[24].addKeyframe(100, 0, 0, 0,  10,   0, 0, -95.9f, 1);
	animsHumanRightThigh[25].addKeyframe(0,   0, 0, 0,  11,   0, 0, -110, 1);
	animsHumanRightThigh[25].addKeyframe(100, 0, 0, 0,  11,   0, 0, -110, 1);

	animsHumanRightShin[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightShin[0].addKeyframe(50, 0, 0, 0, 0, 0, 0, -40, 1);
	animsHumanRightShin[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightShin[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanRightShin[1].addKeyframe(5, 0, 0, 0, 0, 0, 0, -25, 1);
	animsHumanRightShin[1].addKeyframe(10, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanRightShin[1].addKeyframe(15, 0, 0, 0, 0, 0, 0, -45, 1);
	animsHumanRightShin[1].addKeyframe(20, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightShin[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, -125, 1);
	animsHumanRightShin[1].addKeyframe(30, 0, 0, 0, 0, 0, 0, -140, 1);
	animsHumanRightShin[1].addKeyframe(35, 0, 0, 0, 0, 0, 0, -135, 1);
	animsHumanRightShin[1].addKeyframe(40, 0, 0, 0, 0, 0, 0, -115, 1);
	animsHumanRightShin[1].addKeyframe(45, 0, 0, 0, 0, 0, 0, -80, 1);
	animsHumanRightShin[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, -40, 1);
	animsHumanRightShin[1].addKeyframe(55, 0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanRightShin[1].addKeyframe(60, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanRightShin[1].addKeyframe(65, 0, 0, 0, 0, 0, 0, -17, 1);
	animsHumanRightShin[1].addKeyframe(70, 0, 0, 0, 0, 0, 0, -17, 1);
	animsHumanRightShin[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, -5, 1);
	animsHumanRightShin[1].addKeyframe(80, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[1].addKeyframe(85, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[1].addKeyframe(90, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[1].addKeyframe(95, 0, 0, 0, 0, 0, 0, -15, 1);
	animsHumanRightShin[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, -20, 1);
	animsHumanRightShin[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightShin[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, -130, 1);
	animsHumanRightShin[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, -90, 1);
	animsHumanRightShin[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, -60, 1);
	animsHumanRightShin[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightShin[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightShin[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightShin[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightShin[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[12].addKeyframe(0, 0, 0, 0, 0, 0, 0, -120, 1);
	animsHumanRightShin[12].addKeyframe(100, 0, 0, 0, 0, 0, 0, -120, 1);
	animsHumanRightShin[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[14].addKeyframe(0,   0, 0, 0, 0, 0, 0, -70,    1);
	animsHumanRightShin[14].addKeyframe(100, 0, 0, 0, 0, 0, 0, -94.4f, 1);
	animsHumanRightShin[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightShin[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightShin[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, -70, 1);
	animsHumanRightShin[19].addKeyframe(0,   0, 0, 0, 0, 0, 0, -48.2f, 1);
	animsHumanRightShin[19].addKeyframe(100, 0, 0, 0, 0, 0, 0, -48.2f, 1);
	animsHumanRightShin[20].addKeyframe(0,   0, 0, 0, 0, 0, 0, -64, 1);
	animsHumanRightShin[20].addKeyframe(100, 0, 0, 0, 0, 0, 0, -64, 1);
	animsHumanRightShin[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, -64, 1);
	animsHumanRightShin[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, -64, 1);
	animsHumanRightShin[22].addKeyframe(0,   0, 0, 0, 0, 0, 0, -42.2f, 1);
	animsHumanRightShin[22].addKeyframe(100, 0, 0, 0, 0, 0, 0, -65.5f, 1);
	animsHumanRightShin[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, -120,   1);
	animsHumanRightShin[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, -18.6f, 1); //left shin, -xrot
	animsHumanRightShin[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, -120,   1);
	animsHumanRightShin[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, -19, 1);
	animsHumanRightShin[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, -19, 1);
	animsHumanRightShin[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, -30, 1);
	animsHumanRightShin[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, -30, 1);

	animsHumanRightFoot[0].addKeyframe(0, 0, 0, 0, 0, 0, 0, 125, 1);
	animsHumanRightFoot[0].addKeyframe(50, 0, 0, 0, 0, 0, 0, 110, 1);
	animsHumanRightFoot[0].addKeyframe(100, 0, 0, 0, 0, 0, 0, 125, 1);
	animsHumanRightFoot[1].addKeyframe(0, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightFoot[1].addKeyframe(5, 0, 0, 0, 0, 0, 0, 15, 1);
	animsHumanRightFoot[1].addKeyframe(10, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[1].addKeyframe(15, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanRightFoot[1].addKeyframe(20, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[1].addKeyframe(25, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightFoot[1].addKeyframe(30, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightFoot[1].addKeyframe(35, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[1].addKeyframe(40, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanRightFoot[1].addKeyframe(45, 0, 0, 0, 0, 0, 0, 115, 1);
	animsHumanRightFoot[1].addKeyframe(50, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanRightFoot[1].addKeyframe(55, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanRightFoot[1].addKeyframe(60, 0, 0, 0, 0, 0, 0, 95, 1);
	animsHumanRightFoot[1].addKeyframe(65, 0, 0, 0, 0, 0, 0, 95, 1);
	animsHumanRightFoot[1].addKeyframe(70, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightFoot[1].addKeyframe(75, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanRightFoot[1].addKeyframe(80, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanRightFoot[1].addKeyframe(85, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[1].addKeyframe(90, 0, 0, 0, 0, 0, 0, 10, 1);
	animsHumanRightFoot[1].addKeyframe(95, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightFoot[1].addKeyframe(100, 0, 0, 0, 0, 0, 0, -10, 1);
	animsHumanRightFoot[2].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[2].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[3].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[3].addKeyframe(75, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[3].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[4].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightFoot[4].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[5].addKeyframe(0, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightFoot[5].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[6].addKeyframe(0, 0, 0, 0, 0, 0, 0, 45, 1);
	animsHumanRightFoot[6].addKeyframe(100, 0, 0, 0, 0, 0, 0, 0, 1);
	animsHumanRightFoot[7].addKeyframe(0, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanRightFoot[7].addKeyframe(100, 0, 0, 0, 0, 0, 0, 70, 1);
	animsHumanRightFoot[8].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightFoot[8].addKeyframe(100, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanRightFoot[9].addKeyframe(0, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanRightFoot[9].addKeyframe(100, 0, 0, 0, 0, 0, 0, 60, 1);
	animsHumanRightFoot[10].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightFoot[10].addKeyframe(100, 0, 0, 0, 0, 0, 0, 80, 1);
	animsHumanRightFoot[11].addKeyframe(0, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanRightFoot[11].addKeyframe(100, 0, 0, 0, 0, 0, 0, 120, 1);
	animsHumanRightFoot[12].addKeyframe(0, 0, 0, 0, 0, 0, 0, 150, 1);
	animsHumanRightFoot[12].addKeyframe(100, 0, 0, 0, 0, 0, 0, 150, 1);
	animsHumanRightFoot[13].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[13].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[14].addKeyframe(0,   0, 0, 0, 0, 0, 0, 125, 1);
	animsHumanRightFoot[14].addKeyframe(100, 0, 0, 0, 0, 0, 0, 117, 1);
	animsHumanRightFoot[15].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[15].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[16].addKeyframe(0, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[16].addKeyframe(100, 0, 0, 0, 0, 0, 0, 30, 1);
	animsHumanRightFoot[17].addKeyframe(0, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightFoot[17].addKeyframe(100, 0, 0, 0, 0, 0, 0, 90, 1);
	animsHumanRightFoot[18].addKeyframe(0,   0, 0, 0, 0, 0, 0, 73.8f, 1);
	animsHumanRightFoot[18].addKeyframe(100, 0, 0, 0, 0, 0, 0, 73.8f, 1);
	animsHumanRightFoot[19].addKeyframe(0,   0, 0, 0, 0, 0, 0, 73.8f, 1);
	animsHumanRightFoot[19].addKeyframe(100, 0, 0, 0, 0, 0, 0, 73.8f, 1);
	animsHumanRightFoot[20].addKeyframe(0,   0, 0, 0, 0, 0, 0, 35.5f, 1);
	animsHumanRightFoot[20].addKeyframe(100, 0, 0, 0, 0, 0, 0, 35.5f, 1);
	animsHumanRightFoot[21].addKeyframe(0,   0, 0, 0, 0, 0, 0, 45.6f, 1);
	animsHumanRightFoot[21].addKeyframe(100, 0, 0, 0, 0, 0, 0, 45.6f, 1);
	animsHumanRightFoot[22].addKeyframe(0,   0, 0, 0, 0, 0, 0, 103, 1);
	animsHumanRightFoot[22].addKeyframe(100, 0, 0, 0, 0, 0, 0, 138, 1);
	animsHumanRightFoot[23].addKeyframe(0,   0, 0, 0, 0, 0, 0, 157,   1);
	animsHumanRightFoot[23].addKeyframe(50,  0, 0, 0, 0, 0, 0, 48.4f, 1); //left foot -xrot
	animsHumanRightFoot[23].addKeyframe(100, 0, 0, 0, 0, 0, 0, 157,   1);
	animsHumanRightFoot[24].addKeyframe(0,   0, 0, 0, 0, 0, 0, 44.7f, 1);
	animsHumanRightFoot[24].addKeyframe(100, 0, 0, 0, 0, 0, 0, 44.7f, 1);
	animsHumanRightFoot[25].addKeyframe(0,   0, 0, 0, 0, 0, 0, 32, 1);
	animsHumanRightFoot[25].addKeyframe(100, 0, 0, 0, 0, 0, 0, 32, 1);
}

void AnimationResources::assignAnimationsHuman(Body* body, Limb* head,
	Limb* leftHumerus, Limb* leftForearm, Limb* leftHand,
	Limb* rightHumerus, Limb* rightForearm, Limb* rightHand,
	Limb* leftThigh, Limb* leftShin, Limb* leftFoot,
	Limb* rightThigh, Limb* rightShin, Limb* rightFoot)
{
	body->animations         = &AnimationResources::animsHumanBody;
	head->animations         = &AnimationResources::animsHumanHead;
	leftHumerus->animations  = &AnimationResources::animsHumanLeftHumerus;
	leftForearm->animations  = &AnimationResources::animsHumanLeftForearm;
	leftHand->animations     = &AnimationResources::animsHumanLeftHand;
	rightHumerus->animations = &AnimationResources::animsHumanRightHumerus;
	rightForearm->animations = &AnimationResources::animsHumanRightForearm;
	rightHand->animations    = &AnimationResources::animsHumanRightHand;
	leftThigh->animations    = &AnimationResources::animsHumanLeftThigh;
	leftShin->animations     = &AnimationResources::animsHumanLeftShin;
	leftFoot->animations     = &AnimationResources::animsHumanLeftFoot;
	rightThigh->animations   = &AnimationResources::animsHumanRightThigh;
	rightShin->animations    = &AnimationResources::animsHumanRightShin;
	rightFoot->animations    = &AnimationResources::animsHumanRightFoot;
}