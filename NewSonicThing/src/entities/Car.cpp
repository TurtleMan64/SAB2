#include <glad/glad.h>

#include "entity.h"
#include "car.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../entities/camera.h"
#include "../collision/collisionchecker.h"
#include "../collision/triangle3d.h"
#include "../toolbox/maths.h"
#include "../audio/audioplayer.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../toolbox/input.h"
#include "../fontMeshCreator/guinumber.h"
#include "../toolbox/split.h"
#include "../audio/source.h"
#include "checkpoint.h"
#include "../guis/guimanager.h"
#include "maniasonicmodel.h"

#include <list>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>


extern float dt;

Car::Car()
{

}

Car::Car(float x, float y, float z)
{
	position.set(x+0.00001f, y, z+0.00001f);
	vel.set(0.00001f, 0.00001f, 0.00001f);
	relativeUp.set(0, 1, 0);
	relativeUpSmooth.set(0, 1, 0);
	relativeUpAnim.set(0, 1, 0);
	onGround = false;
	camDir.set(0, 0, -1);
	camDirSmooth.set(0, 0, -1);

	maniaSonicModel = new ManiaSonicModel; INCR_NEW("Entity");
	Main_addEntity(maniaSonicModel);
	maniaSonicModel->setVisible(true);

	visible = false;
	loadVehicleInfo();
}

Car::~Car()
{

}

void Car::step()
{
	canMoveTimer      = std::fmaxf(0.0f, canMoveTimer      - dt);
	hoverTimer        = std::fmaxf(0.0f, hoverTimer        - dt);
	homingAttackTimer = std::fmaxf(-1.0f, homingAttackTimer - dt);

	setInputs();

    //Start Lightdash
    if (!isLightdashing)
    {
        if (inputAction3 && !inputAction3Previous)
        {
            lightdashTrail.clear();
            lightdashTrailProgress = -1.0f;

            //search through close entities to find rings
            std::list<std::unordered_set<Entity*>*> entities;
            Global::getNearbyEntities(position.x, position.z, 1, &entities);

            //keep track of rings we've already used, to not use them again
            std::unordered_set<Entity*> alreadyUsedRings;

            float closest = 100000000.0f;
            Vector3f* closestPoint = nullptr;
            Entity* closestEntity = nullptr;

            for (std::unordered_set<Entity*>* set : entities)
	        {
		        for (Entity* e : (*set))
		        {
                    if (!e->canLightdashOn())
                    {
                        continue;
                    }

                    Vector3f diff = position - e->position;
                    float thisdist = diff.lengthSquared();
                    if (thisdist < closest)
                    {
                        closest = thisdist;
                        closestPoint = &e->position;
                        closestEntity = e;
                    }
                }
            }

            if (closest < 45.0f*45.0f)
            {
                isLightdashing = true;
                lightdashTrailProgress = 0.0f;
                lightdashTrail.push_back(closestPoint);
                alreadyUsedRings.insert(closestEntity);

                bool keepGoing = true;
                while (keepGoing)
                {
                    Vector3f center = lightdashTrail.back();
                    //search through close entities to find rings
                    Global::getNearbyEntities(center.x, center.z, 1, &entities);

                    closest = 100000000.0f;
                    closestPoint = nullptr;

                    for (std::unordered_set<Entity*>* set : entities)
                    {
                        for (Entity* e : (*set))
                        {
                            if (!e->canLightdashOn() || alreadyUsedRings.find(e) != alreadyUsedRings.end())
                            {
                                continue;
                            }

                            Vector3f diff = center - e->position;
                            float thisdist = diff.lengthSquared();
                            if (thisdist < closest)
                            {
                                closest = thisdist;
                                closestPoint = &e->position;
                                closestEntity = e;
                            }
                        }
                    }

                    if (closest < 80.0f*80.0f)
                    {
                        lightdashTrail.push_back(closestPoint);
                        alreadyUsedRings.insert(closestEntity);
                    }
                    else
                    {
                        keepGoing = false;
                    }
                }
            }

            //Dont lightdash on a single ring.
            // We only want to dash on 2 or more.
            if (isLightdashing && (int)lightdashTrail.size() < 2)
            {
                lightdashTrail.clear();
                lightdashTrailProgress = -1.0f;
                isLightdashing = false;
            }
        }
    }

    //Move along lightdash trail
    if (isLightdashing)
    {
        if (((int)lightdashTrailProgress)+1 < (int)lightdashTrail.size())
        {
            Vector3f p1 = lightdashTrail[(int)lightdashTrailProgress];
            Vector3f p2 = lightdashTrail[((int)lightdashTrailProgress)+1];
            Vector3f segment = p2 - p1;

            float currentSegmentProg = fmodf(lightdashTrailProgress, 1.0f);

            Vector3f diff = segment.scaleCopy(currentSegmentProg);
            position = p1 + diff;

            vel.set(&segment);
            vel.setLength(lightdashSpeed);

            float segmentLength = segment.length();

            float howMuchProgToAdd = (dt*lightdashSpeed)/segmentLength;
            float howMuchProgLeft = 1.0f - currentSegmentProg;

            if (howMuchProgToAdd < howMuchProgLeft)
            {
                lightdashTrailProgress += howMuchProgToAdd;
            }
            else //go to next segment
            {
                lightdashTrailProgress = std::ceil(lightdashTrailProgress);

                if (((int)lightdashTrailProgress)+1 < (int)lightdashTrail.size())
                {
                    float leftoverProg = howMuchProgToAdd - howMuchProgLeft;
                    float leftoverLen = segmentLength*leftoverProg;

                    p1 = lightdashTrail[(int)lightdashTrailProgress];
                    p2 = lightdashTrail[((int)lightdashTrailProgress)+1];
                    segment = p2 - p1;

                    segmentLength = segment.length();
                    howMuchProgToAdd = leftoverLen/segmentLength;

                    if (howMuchProgToAdd < 1)
                    {
                        lightdashTrailProgress+=howMuchProgToAdd;
                    }
                    else //todo make this keep going until you run out
                    {
                        lightdashTrailProgress+=0.99999f;
                    }
                }
            }

            onGround = false;
        }
        else
        {
            position.set(&lightdashTrail.back());
            vel.setLength(lightdashExitSpeed);
            isLightdashing = false;
            lightdashTrail.clear();
            lightdashTrailProgress = -1.0f;
        }
    }

	//Homing attack
	if (onGround)
	{
		homingAttackTimer = -1.0f;
		justHomingAttacked = false;
        isHomingOnPoint = false;
	}
	else
	{
		if (inputJump && !inputJumpPrevious && (isBall || isJumping) && !justHomingAttacked)
		{
            homingAttack();
		}
	}

	//Jump
	if (onGround)
	{
		if (inputJump && !inputJumpPrevious)
		{
			vel = vel + relativeUp.scaleCopy(jumpPower);
			hoverTimer = hoverTimerThreshold;
			onGround = false;
			isJumping = true;
			AudioPlayer::play(12, getPosition());
		}
	}
	else
	{
		if (inputJump && hoverTimer > 0.0f && isJumping) //Add vel from hover
		{
            if (!(isHomingOnPoint && homingAttackTimer > 0))
            {
			    vel = vel + relativeUpSmooth.scaleCopy(hoverPower*dt);
            }
		}
	}

	//Ball
	if (onGround)
	{
		isJumping = false;
		justHomingAttacked = false;

		if (inputAction2 && !inputAction2Previous)
		{
			if (isBall)
			{
				isBall = false;
				spindashReleaseTimer = spindashReleaseTimerMax;
				spindashRestartDelay = spindashRestartDelayMax;
			}
			else
			{
				float speed = vel.lengthSquared();
				if (speed > autoUnrollThreshold*autoUnrollThreshold)
				{
					AudioPlayer::play(41, getPosition()); //roll
					isBall = true;
				}
			}
		}

		if (isBall)
		{
			float speed = vel.lengthSquared();
			if (speed < autoUnrollThreshold*autoUnrollThreshold)
			{
				isBall = false;
				spindashReleaseTimer = spindashReleaseTimerMax;
				spindashRestartDelay = spindashRestartDelayMax;
			}
		}
	}

	//Spindash stuff
	if (onGround)
	{
		isStomping = false;

		if (!isBall && spindashRestartDelay == 0)
		{
			canStartSpindash = true;
		}
		else
		{
			canStartSpindash = false;
		}

		if (spindashRestartDelay > 0)
		{
			if (inputAction  && !inputActionPrevious)
			{
				bufferedSpindashInput = true;
			}
		}

		if (((inputAction && !inputActionPrevious) && canStartSpindash) || 
			(bufferedSpindashInput && (inputAction) && canStartSpindash))
		{
			if (!isSpindashing)
			{
				storedSpindashSpeed = vel.length();
			}
			isSpindashing = true;
		}

		if (!inputAction)
		{
			isSpindashing = false;
			bufferedSpindashInput = false;
		}

		if (isSpindashing)
		{
			if (spindashTimer == 0)
			{
				AudioPlayer::play(39, getPosition()); //peel charge
			}

			spindashTimer = std::fminf(spindashTimer + dt, spindashTimerMax);
			float sssToAdd = spindashChargeRate*dt;
			if (storedSpindashSpeed + sssToAdd < spindashPowerMax)
			{
				storedSpindashSpeed += sssToAdd;
			}
			else if (storedSpindashSpeed < spindashPowerMax && 
				     storedSpindashSpeed + sssToAdd > spindashPowerMax)
			{
				storedSpindashSpeed = spindashPowerMax;
			}

			isSpindashing = true;
			calcSpindashDirection();
			if (spindashTimer >= spindashDelay)
			{
				vel = Maths::applyDrag(&vel, -spindashFriction, dt);
				if (vel.lengthSquared() < spindashPowerfulFrictionThreshold*spindashPowerfulFrictionThreshold)
				{
					vel = Maths::applyDrag(&vel, -spindashPowerfulFriction, dt);
				}
			}
		}
		else
		{
			if (spindashTimer > 0)
			{
				spindash();
                spindashRestartDelay = spindashRestartDelayMax;
			}
			spindashTimer = 0;
			storedSpindashSpeed = 0;
		}

		if (inputAction  && !inputActionPrevious)
		{
			if (isBall)
			{
				spindashReleaseTimer = spindashReleaseTimerMax;
				spindashRestartDelay = spindashRestartDelayMax;
			}

			isBall = false;
		}
	}
	else
	{
		isSpindashing = false;
		canStartSpindash = false;
		bufferedSpindashInput = false;
		spindashReleaseTimer = 0;
		spindashRestartDelay = 0;
		spindashTimer = 0;
	}
	spindashReleaseTimer = std::fmaxf(spindashReleaseTimer - dt, 0);
	spindashRestartDelay = std::fmaxf(spindashRestartDelay - dt, 0);

	//Bouncing
	if (onGround)
	{
		isBouncing = false;
		justBounced = false;
	}
	else
	{
		if (inputAction && !inputActionPrevious && (isJumping || isBall) && !isBouncing && !justHomingAttacked && !isStomping)
		{
			vel.y = bounceVel;
			isBouncing = true;
		}
	}

	//Grinding
	if (isGrinding)
	{
		onGround = false;
		isJumping = false;
		isBouncing = false;
		isBall = false;
		isLightdashing = false;
		isSkidding = false;
		isSpindashing = false;
		isStomping = false;
	}

    //Skidding (more logic done in moveMeGround)
    if (!onGround)
    {
        isSkidding = false;
    }

	if (onGround)
	{
		moveMeGround();
	}
	else
	{
		moveMeAir();
	}

    //camera stuff
	if (!isGrinding && !onRocket && !isLightdashing)
	{
		//Twisting camera from user input
		camDir = Maths::rotatePoint(&camDir, &relativeUp, -inputX2*dt);

		//camera adjust to direction you are heading in
		if (onGround)
		{
			if (Global::isAutoCam)
			{
                //idea: dont adjust the camera if sonic is heading towards it
                if (camDir.dot(&vel) > -0.5f)
                {
                    //this causes annoying camera jitter when you have fps lag spikes
                    //camDir = Maths::interpolateVectorDebug(&camDir, &vel, fminf(vel.length()*0.014f*dt, 45.0f*dt));

                    //new stuff to get rid of annoying jitter: only rotate "horizontally", not vertically.
                    // this means we need to decrease the max angle you can look down to not stare at the ground
                    // during loops.
                    Vector3f camDirFlat = Maths::projectOntoPlane(&camDir, &relativeUp);
                    float angleDiff = Maths::angleBetweenVectors(&camDirFlat, &vel);
                    float percent = fminf(vel.length()*0.014f*dt, 45.0f*dt);
                    percent = fminf(1.0f, fmaxf(0.0f, percent));
                    float angToRotate = angleDiff*percent;

                    Vector3f camDirRotatePositive = Maths::rotatePoint(&camDirFlat, &relativeUp, angToRotate);
                    float angleDiffAfter = Maths::angleBetweenVectors(&camDirRotatePositive, &vel);

                    if (angleDiffAfter > angleDiff)
                    {
                        camDir = Maths::rotatePoint(&camDir, &relativeUp, -angToRotate);
                    }
                    else
                    {
                        camDir = Maths::rotatePoint(&camDir, &relativeUp, angToRotate);
                    }
                }
			}
			else
			{
				//camDir = Maths::interpolateVector(&camDir, &vel, fminf(vel.length()*0.004f*dt, 0.5f*dt));
			}
		}
		else
		{
			Vector3f noY(vel);
			if (noY.y > 0)
			{
				noY.y = 0;
			}

			if (Global::isAutoCam)
			{
				camDir = Maths::interpolateVector(&camDir, &noY, fminf(noY.length()*0.01f*dt, 30.0f*dt));
			}
			else
			{
				//camDir = Maths::interpolateVector(&camDir, &vel, fminf(vel.length()*0.004f*dt, 0.5f*dt));
			}
		}

		//vertical adjusting camera
		if (onGround)
		{
			//player input
			Vector3f perpen = camDir.cross(&relativeUp);
			camDir = Maths::rotatePoint(&camDir, &perpen, -inputY2*dt);

			//vertical check - rotate down if too high or too low
			float dot = camDir.dot(&relativeUp);
			//if (dot < -0.325f)
            if (dot < -0.2f) //had to change this because of new change in camera rotation to remove annoying jitter
			{
                //float tryingToRotate = -(((dot+0.325f)*15)*dt);
                float tryingToRotate = -(((dot+0.2f)*15)*dt);
                float targetAngle = acosf(-0.2f);
                float currentAngle = acosf(dot);
                float maxAngle = currentAngle-targetAngle;
                //rotate whichever is "less" rotation
                if (fabsf(tryingToRotate) < fabsf(maxAngle))
                {
                    camDir = Maths::rotatePoint(&camDir, &perpen, tryingToRotate);
                }
                else
                {
                    camDir = Maths::rotatePoint(&camDir, &perpen, maxAngle);
                }

                //have it always at least be -0.4f with the ground
                float dotNew = camDir.dot(&relativeUp);
                if (dotNew < -0.4f)
                {
                    targetAngle = acosf(-0.4f);
                    currentAngle = acosf(dotNew);
                    maxAngle = currentAngle-targetAngle;
                    camDir = Maths::rotatePoint(&camDir, &perpen, maxAngle);
                    dotNew = camDir.dot(&relativeUp);
                }
			}
			else if (dot > -0.2f)
			{
                float tryingToRotate = -(((dot+0.2f)*40)*dt);
                float targetAngle = acosf(-0.2f);
                float currentAngle = acosf(dot);
                float maxAngle = currentAngle-targetAngle;
                //rotate whichever is "less" rotation
                if (fabsf(tryingToRotate) < fabsf(maxAngle))
                {
                    camDir = Maths::rotatePoint(&camDir, &perpen, tryingToRotate);
                }
                else
                {
                    camDir = Maths::rotatePoint(&camDir, &perpen, maxAngle);
                }

                //have it always at least be flat with the ground
                float dotNew = camDir.dot(&relativeUp);
                if (dotNew > 0.0f)
                {
                    targetAngle = acosf(0.0f);
                    currentAngle = acosf(dotNew);
                    maxAngle = currentAngle-targetAngle;
                    camDir = Maths::rotatePoint(&camDir, &perpen, maxAngle);
                }

                //Idea: the smooth camera direction should NEVER have a dot product > 0
                float dotSmooth = camDirSmooth.dot(&relativeUp);
                if (dotSmooth > 0.0f)
                {
                    targetAngle = acosf(0.0f);
                    currentAngle = acosf(dotSmooth);
                    maxAngle = currentAngle-targetAngle;
                    camDirSmooth = Maths::rotatePoint(&camDirSmooth, &perpen, maxAngle);
                }
			}
		}
		else
		{
			//player input
			Vector3f perpen = camDir.cross(&relativeUp);
			camDir = Maths::rotatePoint(&camDir, &perpen, -inputY2*dt);

			//vertical check - rotate down if too high or too low
			float dot = camDir.dot(&relativeUp);
			if (dot < -0.75f)
			{
				camDir = Maths::rotatePoint(&camDir, &perpen, -((dot+0.75f)*12)*dt);
			}
			else if (dot > -0.2f)
			{
				camDir = Maths::rotatePoint(&camDir, &perpen, -((dot+0.2f)*20)*dt);
			}
		}
	}
	else //rotating the camera if youre grinding or on a rocket or lightdashing
	{
		//Twisting camera from user input
		camDir = Maths::rotatePoint(&camDir, &relativeUp, -inputX2*dt);

		camDir = Maths::interpolateVector(&camDir, &vel, vel.length()*0.05f*dt);


		//player input
		Vector3f perpen = camDir.cross(&relativeUp);
		camDir = Maths::rotatePoint(&camDir, &perpen, -inputY2*dt);

		//vertical check - rotate down if too high or too low
		float dot = camDir.dot(&relativeUp);
		if (dot < -0.325f)
		{
			camDir = Maths::rotatePoint(&camDir, &perpen, -((dot+0.325f)*12)*dt);
		}
		else if (dot > -0.2f)
		{
			camDir = Maths::rotatePoint(&camDir, &perpen, -((dot+0.2f)*20)*dt);
		}
	}

    //wall sticking
    if (!onGround)
    {
        wallStickTimer = 0.0f;
    }
    else
    {
        if ((currentTriangle->isWall() || currentTriangle->normal.y < wallStickThreshold) &&
            vel.lengthSquared() < wallStickSpeedRequirement*wallStickSpeedRequirement)
        {
            wallStickTimer += dt;

            if (wallStickTimer > wallStickTimerMax)
            {
                popOffWall();
            }
        }
        else
        {
            wallStickTimer = 0.0f;
        }
    }

	//smoothing
	camDirSmooth = Maths::interpolateVector(&camDirSmooth, &camDir, 10*dt);
	relativeUpSmooth = Maths::interpolateVector(&relativeUpSmooth, &relativeUp, 3*dt);
	relativeUpAnim = Maths::interpolateVector(&relativeUpAnim, &relativeUp, 15*dt);

	if (!isGrinding && !onRocket && !isLightdashing)
	{
		//speed before adjusting
		float originalSpeed = vel.length();
		CollisionChecker::setCheckPlayer();
		if (CollisionChecker::checkCollision(getX(), getY(), getZ(), getX()+vel.x*dt, getY()+vel.y*dt, getZ()+vel.z*dt))
		{
			Vector3f* colNormal = &CollisionChecker::getCollideTriangle()->normal;

			if (onGround  == false) //Air to ground
			{
				if (isBouncing)
				{
					vel = Maths::bounceVector(&vel, colNormal, bounceFactor);
					isBall = true;
					isBouncing = false;
					justBounced = true;
					isStomping = false;
					justHomingAttacked = false;
					homingAttackTimer = -1.0f;
                    isHomingOnPoint = false;
					hoverTimer = 0.0f;
					AudioPlayer::play(8, getPosition());
				}
				else
				{
					if (CollisionChecker::getCollideTriangle()->isWall() || colNormal->y < wallStickThreshold)
					{
						if (justBounced)
						{
							float spdBefore = vel.length();
							Vector3f newDirection = Maths::projectOntoPlane(&vel, colNormal);
							newDirection.setLength(spdBefore);
							vel.set(&newDirection);

							justBounced = false;

							setPosition(CollisionChecker::getCollidePosition());
							increasePosition(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET);
						}
						else
						{
							Vector3f newDirection = Maths::projectOntoPlane(&vel, colNormal);
							Vector3f posAfterMoveToWall = Vector3f(CollisionChecker::getCollidePosition());
							Vector3f posDelta = posAfterMoveToWall - position;
							posAfterMoveToWall = posAfterMoveToWall + colNormal->scaleCopy(FLOOR_OFFSET);
							float distLeftToMove = vel.scaleCopy(dt).length();
							float distMoved = posDelta.length();
							distLeftToMove -= distMoved;
						
							vel.set(&newDirection);
							setPosition(&posAfterMoveToWall);
						
							Vector3f velToMove = Vector3f(&vel);
							velToMove.setLength(distLeftToMove);
						
							//move additional distance
							if (distLeftToMove > 0)
							{
								if (CollisionChecker::checkCollision(getX(), getY(), getZ(), getX()+velToMove.x, getY()+velToMove.y, getZ()+velToMove.z) == false)
								{
									increasePosition(velToMove.x, velToMove.y, velToMove.z);
								}
							}

							onGround = false;
							relativeUp.set(0, 1, 0);

                            if (!(isHomingOnPoint && homingAttackTimer > 0))
                            {
							    //vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);

							    Vector3f velToAddFromGravity(relativeUp);
							    velToAddFromGravity.setLength(-gravityForce*dt);
                                vel = vel + velToAddFromGravity;
                            }


							//Vector3f velFrame = vel.scaleCopy(dt);
							//Vector3f newDirection = Maths::projectOntoPlane(&velFrame, colNormal);
							//Vector3f posAfterMoveToWall = Vector3f(CollisionChecker::getCollidePosition());
							//Vector3f posDelta = posAfterMoveToWall - position;
							//posAfterMoveToWall = posAfterMoveToWall + colNormal->scaleCopy(FLOOR_OFFSET);
							//
							//if (CollisionChecker::checkCollision(getX(), getY(), getZ(), posAfterMoveToWall.x, posAfterMoveToWall.y, posAfterMoveToWall.z) == false)
							//{
							//	setPosition(&posAfterMoveToWall);
							//}
							//if (CollisionChecker::checkCollision(getX(), getY(), getZ(), getX() + newDirection.x, getY() + newDirection.y, getZ() + newDirection.z) == false)
							//{
							//	increasePosition(newDirection.x, newDirection.y, newDirection.z);
							//}
							//
							//onGround = false;
							//relativeUp.set(0, 1, 0);
							//vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);
							//Vector3f velToAddFromGravity(relativeUp);
							//velToAddFromGravity.setLength(-gravityForce*dt);
						}
					}
					else
					{
						currentTriangle = CollisionChecker::getCollideTriangle();
						Vector3f newDirection = Maths::projectOntoPlane(&vel, colNormal);
						vel.set(&newDirection);

						setPosition(CollisionChecker::getCollidePosition());
						increasePosition(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET);

						relativeUp.set(colNormal);
						onGround = true;
						isBall = false;
					}
				}
			}
			else //Ground to a different triangle
			{
				//check if you can smoothly transition from previous triangle to this triangle
				float dotProduct = relativeUp.dot(colNormal);
				if (dotProduct < smoothTransitionThreshold || CollisionChecker::getCollideTriangle()->isWall())
				{
					float len = vel.length();
					float dot = vel.dot(colNormal)/len;
					float fact = sqrtf(1 - dot*dot);
					Vector3f bounce   = Maths::bounceVector(&vel, colNormal, 1.0f);
					Vector3f parallel = Maths::projectOntoPlane(&vel, colNormal);
					bounce   = Maths::projectOntoPlane(&bounce,   &relativeUp);
					parallel = Maths::projectOntoPlane(&parallel, &relativeUp);
					Vector3f rotate1 = Maths::rotatePoint(&parallel, &relativeUp, 0.1f);
					Vector3f rotate2 = Maths::rotatePoint(&parallel, &relativeUp, -0.1f);
					vel = Maths::getCloserPoint(&rotate1, &rotate2, &bounce);
					vel.setLength(len*fact);

					canMoveTimer = hitWallTimePunish;
					//AudioPlayer::play(4, getPosition());

					increasePosition(colNormal->x*FLOOR_OFFSET*2, colNormal->y*FLOOR_OFFSET*2, colNormal->z*FLOOR_OFFSET*2);
				}
				else
				{
					currentTriangle = CollisionChecker::getCollideTriangle();
					Vector3f newDirection = Maths::projectOntoPlane(&vel, colNormal);
					if (newDirection.lengthSquared() != 0)
					{
						newDirection.normalize();
						newDirection.x*=originalSpeed;
						newDirection.y*=originalSpeed;
						newDirection.z*=originalSpeed;
						vel.set(&newDirection);
					}

					relativeUp.set(colNormal);
					onGround = true;

					Vector3f newPosition(CollisionChecker::getCollidePosition());
					//newPosition.add(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET); //not sure if it would be better to calculate new positoin like this instead
					Vector3f travelDelta = newPosition - position;

					setPosition(CollisionChecker::getCollidePosition());
					increasePosition(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET);

					float distanceTraveled = travelDelta.length();
					float distanceRemaining = (vel.length()*dt)-distanceTraveled;

					Vector3f nextVel(&vel);
					nextVel.normalize();
					nextVel.scale(distanceRemaining);

					while (distanceRemaining > 0.0f)
					{
						CollisionChecker::setCheckPlayer();
						if (CollisionChecker::checkCollision(getX(), getY(), getZ(), getX()+nextVel.x, getY()+nextVel.y, getZ()+nextVel.z))
						{
							colNormal = &CollisionChecker::getCollideTriangle()->normal;

							//check if you can smoothly transition from previous triangle to this triangle
							dotProduct = relativeUp.dot(colNormal);
							if (dotProduct < smoothTransitionThreshold || CollisionChecker::getCollideTriangle()->isWall())
							{
								float len = vel.length();
								float dot = vel.dot(colNormal)/len;
								float fact = sqrtf(1 - dot*dot);
								Vector3f bounce   = Maths::bounceVector(&vel, colNormal, 1.0f);
								Vector3f parallel = Maths::projectOntoPlane(&vel, colNormal);
								bounce   = Maths::projectOntoPlane(&bounce,   &relativeUp);
								parallel = Maths::projectOntoPlane(&parallel, &relativeUp);
								Vector3f rotate1 = Maths::rotatePoint(&parallel, &relativeUp, 0.1f);
								Vector3f rotate2 = Maths::rotatePoint(&parallel, &relativeUp, -0.1f);
								vel = Maths::getCloserPoint(&rotate1, &rotate2, &bounce);
								vel.setLength(len*fact);

								canMoveTimer = hitWallTimePunish;
								//AudioPlayer::play(4, getPosition());

								increasePosition(colNormal->x*FLOOR_OFFSET*2, colNormal->y*FLOOR_OFFSET*2, colNormal->z*FLOOR_OFFSET*2);

								distanceRemaining = 0.0f;
							}
							else
							{
								currentTriangle = CollisionChecker::getCollideTriangle();
								newDirection = Maths::projectOntoPlane(&nextVel, colNormal);
								if (newDirection.lengthSquared() != 0)
								{
									newDirection.normalize();
									newDirection.x*=originalSpeed;
									newDirection.y*=originalSpeed;
									newDirection.z*=originalSpeed;
									vel.set(&newDirection);
								}

								relativeUp.set(colNormal);
								onGround = true;

								newPosition.set(CollisionChecker::getCollidePosition());
								travelDelta = newPosition - position;


								setPosition(CollisionChecker::getCollidePosition());
								increasePosition(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET);

								distanceTraveled = travelDelta.length();
								distanceRemaining = distanceRemaining-distanceTraveled;

								nextVel.set(&vel);
								nextVel.normalize();
								nextVel.scale(distanceRemaining);
							}
						}
						else //no more collisions, travel the remaining distance
						{
							increasePosition(nextVel.x, nextVel.y, nextVel.z);
							distanceRemaining = 0.0f;
						}
					}
				}
			}
		}
		else //No initial collision
		{
			increasePosition(vel.x*dt, vel.y*dt, vel.z*dt);

			bool checkPassed = false;
			CollisionChecker::setCheckPlayer();
			if (onGround)
			{
				checkPassed = CollisionChecker::checkCollision(getX(), getY(), getZ(), getX() - relativeUp.x*surfaceTension, getY() - relativeUp.y*surfaceTension, getZ() - relativeUp.z*surfaceTension);
			}
			if (checkPassed)
			{
				float dotProduct = relativeUp.dot(&(CollisionChecker::getCollideTriangle()->normal));

				if (dotProduct < smoothTransitionThreshold || CollisionChecker::getCollideTriangle()->isWall()) //It's a wall, pretend the collision check didn't see it
				{
					Vector3f perpen = Maths::calcThirdAxis(&vel, &relativeUp);
					Vector3f coordsFlat = Maths::coordinatesRelativeToBasis(&vel, &relativeUp, &perpen, &(CollisionChecker::getCollideTriangle()->normal));

					if (coordsFlat.x > 0) //Only ignore walls that are cliffs, not walls
					{
						checkPassed = false;
					}
				}
			}
			
			if (checkPassed)
			{
				Vector3f* colNormal = &CollisionChecker::getCollideTriangle()->normal;

				float dotProduct = relativeUp.dot(&(CollisionChecker::getCollideTriangle()->normal));
				if (dotProduct < smoothTransitionThreshold || CollisionChecker::getCollideTriangle()->isWall())
				{
					CollisionChecker::falseAlarm();

					float len = vel.length();
					float dot = fabsf(vel.dot(colNormal)/len);
					float fact = sqrtf(1 - dot*dot);
					Vector3f bounce   = Maths::bounceVector(&vel, colNormal, 1.0f);
					Vector3f parallel = Maths::projectOntoPlane(&vel, colNormal);
					bounce   = Maths::projectOntoPlane(&bounce,   &relativeUp);
					parallel = Maths::projectOntoPlane(&parallel, &relativeUp);
					Vector3f rotate1 = Maths::rotatePoint(&parallel, &relativeUp, 0.1f);
					Vector3f rotate2 = Maths::rotatePoint(&parallel, &relativeUp, -0.1f);
					vel = Maths::getCloserPoint(&rotate1, &rotate2, &bounce);
					vel.setLength(len*fact);

					canMoveTimer = hitWallTimePunish;
					//AudioPlayer::play(4, getPosition());

					increasePosition(colNormal->x*FLOOR_OFFSET*2, colNormal->y*FLOOR_OFFSET*2, colNormal->z*FLOOR_OFFSET*2);
				}
				else
				{
					currentTriangle = CollisionChecker::getCollideTriangle();
					Vector3f* normal = &CollisionChecker::getCollideTriangle()->normal;

					setPosition(CollisionChecker::getCollidePosition());
					increasePosition(normal->x*FLOOR_OFFSET, normal->y*FLOOR_OFFSET, normal->z*FLOOR_OFFSET);
				
					//speed before adjusting
					float speed = vel.length();
				
					Vector3f newDirection = Maths::projectOntoPlane(&vel, normal);

					newDirection.normalize();
					newDirection.x*=speed;
					newDirection.y*=speed;
					newDirection.z*=speed;
					vel.set(&newDirection);
				
					relativeUp.set(normal);
					onGround = true;
				}
			}
			else
			{
				CollisionChecker::falseAlarm();
				onGround = false;

				relativeUp.set(0, 1, 0);

                if (!(isHomingOnPoint && homingAttackTimer > 0))
                {
                    //vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);

				    Vector3f velToAddFromGravity(relativeUp);
				    velToAddFromGravity.setLength(-gravityForce*dt);
                    vel = vel + velToAddFromGravity;
				    if (Input::inputs.INPUT_RB)
				    {
					    vel = vel - velToAddFromGravity.scaleCopy(4);
				    }
                }
			}
		}
	}

	camDir.normalize();

	//Animating us
	updateAnimationValues();
	animate();

	//Animating the camera
	refreshCamera();

	Global::gameMainVehicleSpeed = (int)(vel.length());

	//Vector3f posDiffDelta = position - prevPos;
	//std::fprintf(stdout, "delta pos = %f\n\n", posDiffDelta.length()/dt);



	//std::fprintf(stdout, "%f\n\n\n\n", vel.length());

	//Vector3f vnorm(&vel);
	//vnorm.normalize();
	//std::fprintf(stdout, "pos  = [%f, %f, %f]\n", position.x, position.y, position.z);
	//std::fprintf(stdout, "norm = [%f, %f, %f]\n", currNorm.x, currNorm.y, currNorm.z);
	//std::fprintf(stdout, "dir  = [%f, %f, %f]\n", vnorm   .x, vnorm   .y, vnorm   .z);
	//std::fprintf(stdout, "%f %f %f   %f %f %f   %f %f %f\n", position.x, position.y, position.z, currNorm.x, currNorm.y, currNorm.z, vnorm.x, vnorm.y, vnorm.z);
    centerPosPrev = getCenterPosition();
}

void Car::spindash()
{
	Vector3f newDir = Maths::projectOntoPlane(&spindashDirection, &relativeUp);
	newDir.setLength(storedSpindashSpeed);
	vel.set(&newDir);
	//std::fprintf(stdout, "spindash at %f speed\n", storedSpindashSpeed);
	//isBall = true;
	isBall = false; //geeks idea change
	AudioPlayer::play(40, getPosition()); //peel release
	storedSpindashSpeed = 0;
}

void Car::calcSpindashDirection()
{
	float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
	float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
	Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
	dirForward.setLength(stickRadius);
	Vector3f newDir = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);

	if (stickRadius >= 0.8f)
	{
		spindashDirection.set(&newDir);
	}
	else if (vel.length() > 50.0f)
	{
		spindashDirection.set(&vel);
	}
}

void Car::moveMeGround()
{
	if ((isSpindashing && vel.lengthSquared() < spindashPowerfulFrictionThreshold*spindashPowerfulFrictionThreshold) || //if you are stopped and charging a spindash, dont move sonic
        isLightdashing)
	{
		return;
	}

	float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
	float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
	Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
	dirForward.setLength(stickRadius);
	Vector3f velToAdd = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);

	Vector3f velBefore(&vel);
	bool skidded = false;
	bool heldUp = false;

	if (stickRadius > 0.1f)
	{
		heldUp = true;
		if (isBall)
		{
			Vector3f fr(0, -0.001f, 0);
			fr = Maths::projectOntoPlane(&fr, &relativeUp);
			float frictionPower = groundBallFriction*(1 - fr.length());
			vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction

			//rotate vel to where the stick is going
			float spd = vel.length();
			vel = Maths::interpolateVector(&vel, &velToAdd, dt*(60.0f/12.0f)); // 60.0f/12.0f before 6/11/2019
			vel.setLength(spd);
		}
		else
		{
			vel = vel + velToAdd.scaleCopy(groundRunPush*dt); //Add vel from player stick input

			Vector3f fr(0, -0.001f, 0); //-.25
			fr = Maths::projectOntoPlane(&fr, &relativeUp);
			float frictionPower = groundRunFriction*(1 - fr.length());
			vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction

			//rotate vel to where the stick is going
			float ang = Maths::angleBetweenVectors(&vel, &velToAdd);
			if (ang > Maths::toRadians(135.0f) && vel.length() > 30) //skid
			{
				vel = Maths::applyDrag(&vel, skidPower, dt);
				skidded = true;
                if (!isSkidding && vel.lengthSquared() >= skidAudioThreshold*skidAudioThreshold)
                {
                    AudioPlayer::play(13, &position);
                }
			}
			else
			{
				float spd = vel.length();
				vel = Maths::interpolateVector(&vel, &velToAdd, dt*(60.0f/12.0f)); // 60.0f/12.0f before 6/11/2019
				vel.setLength(spd);
			}
		}
	}
	else
	{
		if (vel.lengthSquared() > 0.00001f)
		{
			if (isBall)
			{
				//version 2
				//Vector3f fr(0, -0.95f, 0);
				Vector3f fr(0, -0.001f, 0);
				fr = Maths::projectOntoPlane(&fr, &relativeUp);
				float frictionPower = groundBallFriction*(1 - fr.length());
				vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction

				//version 1
				//float frictionPower = groundBallFriction;
				//vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction
			}
			else //neutral stick slowdown
			{
                if (vel.lengthSquared() > 35.0f*35.0f) //normal neutral stick friction
		        {
				    Vector3f fr(0, -0.95f, 0); //0.75
				    fr = Maths::projectOntoPlane(&fr, &relativeUp);
				    float frictionPower = groundNeutralFriction*(1 - fr.length());
				    vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction
                }
                else //when close to no speed, increase friction
		        {
				    Vector3f fr(0, -0.95f, 0); //0.75
				    fr = Maths::projectOntoPlane(&fr, &relativeUp);
				    float frictionPower = (groundNeutralFriction*4)*(1 - fr.length()); //multiply by 4, arbitrary
				    vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction
                }
			}
		}
	}

	//Add to velocity based on the slope you are on
	//std::fprintf(stdout, "relativeU.y = %f\n", relativeUp.y);
	if (relativeUp.y < 0.99f)
	{
		if (!isSpindashing || vel.lengthSquared() > spindashPowerfulFrictionThreshold*spindashPowerfulFrictionThreshold) //if you are stopped and charging a spindash, dont move sonic
		{
			if (isBall)
			{
				//version 2
				float slopePower = slopeBallAccel; //TODO: this might not work perfectly with arbitrary framerate
				Vector3f slopeVel(0, -slopePower*dt, 0); //slopeAccel
				slopeVel = Maths::projectOntoPlane(&slopeVel, &relativeUp);
				vel = vel + slopeVel;

				//version 1
				//float slopePower = slopeBallAccel/(vel.length()+1); //TODO: this might not work perfectly with arbitrary framerate
				//Vector3f slopeVel(0, -slopePower*dt, 0); //slopeAccel
				//slopeVel = Maths::projectOntoPlane(&slopeVel, &relativeUp);
				//vel = vel + slopeVel;
			}
			else
			{
				float slopePower = slopeRunAccel/(vel.length()+1); //TODO: this might not work perfectly with arbitrary framerate
				//float slopePower = 300.0f; //80
				Vector3f slopeVel(0, -slopePower*dt, 0); //slopeAccel
				slopeVel = Maths::projectOntoPlane(&slopeVel, &relativeUp);
				vel = vel + slopeVel;
			}
		}
	}

	//part 1 of geeks idea
	if (heldUp && !skidded && !isBall)
	{
		//dont slow down if you holding out on stick and youre running
		if (velBefore.lengthSquared() > vel.lengthSquared() &&
            velBefore.length() > spindashPowerMax-20.0f) //only dont slow down if youre already going fast
		{
			vel.setLength(velBefore.length());
		}
	}

	//extension of geeks idea
	if (isBall)
	{
		//dont let ball speed surpass some max threshold
		//const float maxBallthreshold = 800.0f;
		if (velBefore.lengthSquared() < maxBallSpeed*maxBallSpeed && 
            vel.lengthSquared() >= maxBallSpeed*maxBallSpeed)
		{
			vel.setLength(velBefore.length());
		}
        else if (velBefore.lengthSquared() > maxBallSpeed*maxBallSpeed && 
                 vel.lengthSquared() > maxBallSpeed*maxBallSpeed) //if you are over the limit, dont speed up more
        {
            if (vel.lengthSquared() > velBefore.lengthSquared())
            {
                vel.setLength(velBefore.length());
            }
        }
	}

    isSkidding = skidded;
}

void Car::moveMeAir()
{
	if (isGrinding || isLightdashing || (isHomingOnPoint && homingAttackTimer > 0))
	{
		return;
	}

	float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
	float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
	Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
	dirForward.setLength(stickRadius);
	Vector3f velToAdd = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);
	velToAdd.y = 0;

	if (stickRadius > 0.1f)
	{
		vel = vel + velToAdd.scaleCopy(airRunPush*dt); //Add vel from player stick input

		if (vel.lengthSquared() > 0.00001f)
		{
			float storedVelY = vel.y;
			vel.y = 0;
            if (vel.lengthSquared() > airFrictionThreshold*airFrictionThreshold)
            {
			    if (storedVelY >= 0)
			    {
				    vel = Maths::applyDrag(&vel, -airRunFrictionUp, dt); //Slow vel down due to friction (but no slowdown in y direction)
			    }
			    else
			    {
				    vel = Maths::applyDrag(&vel, -airRunFrictionDown, dt); //Slow vel down due to friction (but no slowdown in y direction)
			    }
            }
			vel.y = storedVelY;
		}

		//rotate vel to where the stick is going
		float storedVelY = vel.y;
		vel.y = 0;
		float ang = Maths::angleBetweenVectors(&vel, &velToAdd);
		if (ang > 0.001f)
		{
			float spd = vel.length();
			vel = Maths::interpolateVector(&vel, &velToAdd, dt*(60.0f/10.0f)); //changed from 12 on 6/11/2019
			vel.setLength(spd);
			vel.scale(1 - 4.5f*ang*dt*(1/Maths::PI)); //slow down from turning in the air
		}
		vel.y = storedVelY;
	}
	else
	{
		if (vel.lengthSquared() > 0.00001f)
		{
			float storedVelY = vel.y;
			vel.y = 0;
			vel = Maths::applyDrag(&vel, -airNeutralFriction, dt); //Slow vel down due to friction (but no slowdown in y direction)
			vel.y = storedVelY;
		}
	}
}

void Car::setVelocity(float xVel, float yVel, float zVel)
{
	vel.x = xVel;
	vel.y = yVel;
	vel.z = zVel;
}

Vector3f* Car::getVelocity()
{
	return &vel;
}

void Car::setCanMoveTimer(float newTimer)
{
	canMoveTimer = newTimer;
}

void Car::startGrinding()
{
	isGrinding = true;
	isJumping = false;
	isBouncing = false;
	isBall = false;
	isLightdashing = false;
	isSkidding = false;
	isSpindashing = false;
	isStomping = false;
}

void Car::stopGrinding()
{
	isGrinding = false;
}

void Car::doJump()
{
	isGrinding = false;
	isBouncing = false;
	isBall = false;
	isLightdashing = false;
	isSkidding = false;
	isSpindashing = false;
	isStomping = false;
	justBounced = false;
	justHomingAttacked = false;
    isHomingOnPoint = false;
	homingAttackTimer = -1.0f;
	vel = vel + relativeUp.scaleCopy(jumpPower);
	hoverTimer = hoverTimerThreshold;
	onGround = false;
	isJumping = true;
	AudioPlayer::play(12, getPosition());
}

void Car::rebound(Vector3f* source)
{
	if (!onGround)
	{
        vel.y = 0;
        vel.setLength(5.0f);
	    vel.y = 126.0f;
	    setX(source->x);
	    setZ(source->z);
	    //setY(source->y + 3.5f);
        setY(source->y + 0.01f);
	    homingAttackTimer = -1.0f;
	    justHomingAttacked = false;
        isHomingOnPoint = false;
	    hoverTimer = hoverTimerThreshold/2;
	}
}

bool Car::isVulnerable()
{
	return !(homingAttackTimer > 0 ||
		isBouncing ||
		isJumping ||
		isBall ||
		isSpindashing ||
		isStomping);
		//invincibleTimer != 0);
}

void Car::homingAttack()
{
    float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
	float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
	Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
	Vector3f stickDirection = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);
    stickDirection.y = 0;
    stickDirection.normalize();

    //if neutral sticking, home in on the closest enemy
    bool lookingForClosest = (stickRadius < 0.1f);
    
    //search through close entities to find rings
    std::list<std::unordered_set<Entity*>*> entities;
    Global::getNearbyEntities(position.x, position.z, 1, &entities);

    float closestDist = homingAttackRangeMax;
    float bestDotProduct = -1.0f;

    bool homeInOnPoint = false;
    Vector3f homeTargetPoint(0, 0, 0);

    if (lookingForClosest)
    {
        for (std::unordered_set<Entity*>* set : entities)
	    {
		    for (Entity* e : (*set))
		    {
                if (!e->canHomingAttackOn())
                {
                    continue;
                }

                Vector3f diff = position - e->getHomingCenter();
                if (diff.y < -12 || diff.y > 80)
                {
                    continue;
                }

                float thisdist = diff.lengthSquared();
                if (thisdist < closestDist)
                {
                    closestDist = thisdist;
                    homeTargetPoint = e->getHomingCenter();
                    homeInOnPoint = true;
                }
            }
        }
    }
    else 
    {
        for (std::unordered_set<Entity*>* set : entities)
	    {
		    for (Entity* e : (*set))
		    {
                if (!e->canHomingAttackOn())
                {
                    continue;
                }

                Vector3f diff = position - e->getHomingCenter();
                if (diff.y < -12 || diff.y > 80)
                {
                    continue;
                }

                if (diff.lengthSquared() >= homingAttackRangeMax)
                {
                    continue;
                }

                diff.y = 0;
                diff.neg();
                diff.normalize();

                float thisdot = diff.dot(&stickDirection);
                if (thisdot > bestDotProduct)
                {
                    bestDotProduct = thisdot;
                    homeTargetPoint = e->getHomingCenter();
                }
            }
        }

        if (bestDotProduct > homingAttackDotThreshold)
        {
            homeInOnPoint = true;
        }
    }

	if (homeInOnPoint)
	{
        vel = homeTargetPoint - position;
        vel.setLength(6.7f*60.0f);

		isHomingOnPoint = true;
	}
	else
	{
        float storedVelY = vel.y;
		vel.y = 0;

		if (vel.length() < 300.0f)
		{
			vel.setLength(300.0f);
        }

		vel.y = storedVelY;

        isHomingOnPoint = false;
	}

	homingAttackTimer = homingAttackTimerMax;
	isBall = false;
	isJumping = true;
	isBouncing = false;
	isStomping = false;
	justBounced = false;
	justHomingAttacked = true;
	AudioPlayer::play(11, getPosition());
}

void Car::setRelativeUp(Vector3f* newUp)
{
	relativeUp.set(newUp);
}

void Car::updateAnimationValues()
{
	float currSpeed = vel.length();

    if (isLightdashing)
    {

    }
    else if (onRocket)
    {

    }
	else if (isJumping)
	{
		runAnimationCycle -= 3000*dt;
	}
	else if (isBouncing)
	{
		runAnimationCycle -= 3500*dt;
	}
	else if (isBall)
	{
		runAnimationCycle -= (8.0f*currSpeed + 300)*dt;
	}
	else if (isGrinding)
	{

	}
	else if (isSpindashing)
	{
		if (spindashTimer >= spindashTimerMax)
		{
			runAnimationCycle -= 2500*dt;
		}
		else
		{
			runAnimationCycle = -(spindashTimer*spindashTimer*0.8f*60*60);
		}
	}
    else if (isSkidding)
    {

    }
	else if (spindashReleaseTimer > 0)
	{
		runAnimationCycle = (spindashReleaseTimer*spindashReleaseTimer*0.4f*60*60);
	}
	else if (onGround && currSpeed < 0.6f) //stand
	{

	}
	else if (!onGround) //freefall
	{

	}
	else //running animation
	{
		runAnimationCycle += (1.5f*currSpeed)*dt;
		runAnimationCycle = fmodf(runAnimationCycle, 100.0f);
		if (runAnimationCycle < 0.0f)
		{
			runAnimationCycle += 100.0f; //fmodf returns negative numbers if the number is negative
		}

        if (currSpeed > spindashPowerMax-20.0f)
        {
            Vector3f rng(Maths::nextUniform()-0.5f, Maths::nextUniform()-0.5f, Maths::nextUniform()-0.5f);
            rng.scale(75.0f);
            rng = Maths::projectOntoPlane(&rng, &relativeUp);

            Vector3f spd = relativeUp.scaleCopy(55.0f) + rng;

            Vector3f partPos = position + relativeUp.scaleCopy(1.5f);

            new Particle(ParticleResources::textureDust, &partPos, &spd, 0, 0.25f + (0.125f*Maths::nextGaussian()), 0, 5.0f+Maths::nextGaussian(), 0.0f, false, false);
            //new Particle(ParticleResources::textureDust, &partPos, 0.25f, 6.0f, 2.8f, false);
        }
	}

    ParticleTexture* trail = nullptr;
    float density = 2.0f; //1 particle every 2 units

    if (isLightdashing)
    {
        trail = ParticleResources::textureOrangeTrail;
        density = 0.75f;
    }
    else if (homingAttackTimer > 0 ||
             (isJumping && (hoverTimer > 0 && inputJump)) ||
             (isBall && onGround) ||
             isBouncing)
    {
        trail = ParticleResources::textureLightBlueTrail;
        density = 0.75f;
    }

    if (trail != nullptr)
    {
        Vector3f centerNew = getCenterPosition();
        Vector3f diff = centerNew - centerPosPrev;
        int numParticles = (int)(diff.length()/density);

        Vector3f zero(0, 0, 0);

        for (int i = 0; i < numParticles; i++)
        {
            Vector3f partPos = centerPosPrev + diff.scaleCopy(((float)i)/numParticles);
            new Particle(trail, &partPos, &zero, 0, 0.25f, 0, 8.0f, -32.0f, false, false);
        }
    }
}

void Car::animate()
{
	//idea: relativeUpSmooth works pretty well here but its a bit too smooth. maybe make a new relativeUpSmoothAnim just for this animation?
	Vector3f groundSpeeds = Maths::calculatePlaneSpeed(vel.x, vel.y, vel.z, &relativeUpAnim);
	float twistAngleGround = Maths::toDegrees(atan2f(-groundSpeeds.z, groundSpeeds.x));
	float nXGround = relativeUpAnim.x;
	float nYGround = relativeUpAnim.y;
	float nZGround = relativeUpAnim.z;
	float normHLengthGround = sqrtf(nXGround*nXGround + nZGround*nZGround);
	float pitchAngleGround = Maths::toDegrees(atan2f(nYGround, normHLengthGround));
	float yawAngleGround = Maths::toDegrees(atan2f(-nZGround, nXGround));
	float diffGround = Maths::compareTwoAngles(twistAngleGround, yawAngleGround);

	float twistAngleAir = Maths::toDegrees(atan2f(-vel.z, vel.x));
	float nXAir = relativeUpAnim.x;
	float nYAir = relativeUpAnim.y;
	float nZAir = relativeUpAnim.z;
	float normHLengthAir = sqrtf(nXAir*nXAir + nZAir*nZAir);
	float pitchAngleAir = Maths::toDegrees(atan2f(nYAir, normHLengthAir));
	float yawAngleAir = Maths::toDegrees(atan2f(-nZAir, nXAir));
	float diffAir = Maths::compareTwoAngles(twistAngleAir, yawAngleAir);

    float airYaw = Maths::toDegrees(atan2f(-vel.z, vel.x));
    float airPitch = Maths::toDegrees(atan2f(vel.y, sqrtf(vel.x*vel.x + vel.z*vel.z)));

	float currSpeed = vel.length();

	float dspX = position.x;
	float dspY = position.y;
	float dspZ = position.z;
	if (onGround)
	{
		if (isBall || isSpindashing || spindashReleaseTimer > 0)
		{
			Vector3f ballOffset = relativeUp.scaleCopy(displayBallOffset);
			dspX += ballOffset.x;
			dspY += ballOffset.y;
			dspZ += ballOffset.z;
		}
		else
		{
			Vector3f displayOffset = relativeUp.scaleCopy(displayHeightOffset);
			dspX += displayOffset.x;
			dspY += displayOffset.y;
			dspZ += displayOffset.z;
		}
	}

    if (isLightdashing)
    {
        maniaSonicModel->setOrientation(dspX, dspY, dspZ, 0, airYaw, 90, airPitch);
		maniaSonicModel->animate(18, 0);
    }
    else if (onRocket)
    {
        maniaSonicModel->setOrientation(dspX, dspY, dspZ, 0, airYaw, 90, airPitch);
		maniaSonicModel->animate(25, 0);
    }
	else if (isJumping)
	{
		maniaSonicModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, runAnimationCycle);
		maniaSonicModel->animate(12, 0);
	}
	else if (isBouncing)
	{
		maniaSonicModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, runAnimationCycle);
		maniaSonicModel->animate(12, 0);
	}
	else if (isBall)
	{
		if (onGround)
		{
			maniaSonicModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, runAnimationCycle);
		}
		else
		{
			maniaSonicModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, runAnimationCycle);
		}
		maniaSonicModel->animate(12, 0);
	}
	else if (isGrinding)
	{
		maniaSonicModel->setOrientation(getX()-nXAir, getY()-nYAir, getZ()-nZAir, diffGround, yawAngleGround, pitchAngleGround, 0);
		maniaSonicModel->animate(26, 0);
	}
	else if (isSpindashing)
	{
		Vector3f groundSpeedsSpnd = Maths::calculatePlaneSpeed(spindashDirection.x, spindashDirection.y, spindashDirection.z, &relativeUpAnim);
		float twistAngleGroundSpnd = Maths::toDegrees(atan2f(-groundSpeedsSpnd.z, groundSpeedsSpnd.x));
		float nXGroundSpnd = relativeUpAnim.x;
		float nYGroundSpnd = relativeUpAnim.y;
		float nZGroundSpnd = relativeUpAnim.z;
		float normHLengthGroundSpnd = sqrtf(nXGroundSpnd*nXGroundSpnd + nZGroundSpnd*nZGroundSpnd);
		float pitchAngleGroundSpnd = Maths::toDegrees(atan2f(nYGroundSpnd, normHLengthGroundSpnd));
		float yawAngleGroundSpnd = Maths::toDegrees(atan2f(-nZGroundSpnd, nXGroundSpnd));
		float diffGroundSpnd = Maths::compareTwoAngles(twistAngleGroundSpnd, yawAngleGroundSpnd);

		maniaSonicModel->setOrientation(dspX, dspY, dspZ, diffGroundSpnd, yawAngleGroundSpnd, pitchAngleGroundSpnd, runAnimationCycle);
		maniaSonicModel->animate(12, 0);
	}
    else if (isSkidding)
    {
        rotX = diffGround;
		rotY = yawAngleGround;
		rotZ = pitchAngleGround;
		rotRoll = 0;

		maniaSonicModel->setOrientation(getX(), getY(), getZ(), rotX, rotY, rotZ, rotRoll);
        maniaSonicModel->animate(8, 0);
    }
	else if (spindashReleaseTimer > 0)
	{
		maniaSonicModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, runAnimationCycle);
		maniaSonicModel->animate(12, 0);
	}
	else if (onGround && currSpeed < 0.6f) //stand
	{
		rotX = diffGround;
		rotY = yawAngleGround;
		rotZ = pitchAngleGround;
		rotRoll = 0;

		maniaSonicModel->setOrientation(getX(), getY(), getZ(), rotX, rotY, rotZ, rotRoll);
		maniaSonicModel->animate(15, 0);
	}
	else if (!onGround) //freefall
	{
		rotX = diffAir;
		rotY = yawAngleAir;
		rotZ = pitchAngleAir;
		rotRoll = 0;

		maniaSonicModel->setOrientation(getX(), getY(), getZ(), rotX, rotY, pitchAngleAir, rotRoll);
		maniaSonicModel->animate(21, 0);
	}
	else //running animation
	{
		rotX = diffGround;
		rotY = yawAngleGround;
		rotZ = pitchAngleGround;
		rotRoll = 0;

		maniaSonicModel->setOrientation(getX(), getY(), getZ(), rotX, rotY, rotZ, rotRoll);

		if (currSpeed < 200)
		{
			maniaSonicModel->animate(15, runAnimationCycle);
		}
		else
		{
			maniaSonicModel->animate(1, runAnimationCycle);
		}
	}
}

void Car::setInputs()
{
	inputJump    = Input::inputs.INPUT_ACTION1;
	inputAction  = Input::inputs.INPUT_ACTION2;
	inputAction2 = Input::inputs.INPUT_ACTION3;
    inputAction3 = Input::inputs.INPUT_ACTION4;
	inputX       = Input::inputs.INPUT_X;
	inputY       = Input::inputs.INPUT_Y;
	inputX2      = Input::inputs.INPUT_X2;
	inputY2      = Input::inputs.INPUT_Y2;

	inputJumpPrevious    = Input::inputs.INPUT_PREVIOUS_ACTION1;
	inputActionPrevious  = Input::inputs.INPUT_PREVIOUS_ACTION2;
	inputAction2Previous = Input::inputs.INPUT_PREVIOUS_ACTION3;
    inputAction3Previous = Input::inputs.INPUT_PREVIOUS_ACTION4;

	if (canMoveTimer > 0.0f || Global::finishStageTimer >= 0.0f)
	{
		inputJump    = false;
		inputAction  = false;
		inputAction2 = false;
        inputAction3 = false;
		inputX    = 0;
		inputY    = 0;
		inputX2   = 0;
		inputY2   = 0;

		inputJumpPrevious    = false;
		inputActionPrevious  = false;
		inputAction2Previous = false;
        inputAction3Previous = false;
	}
}

std::list<TexturedModel*>* Car::getModels()
{
	return nullptr;
}

void Car::loadVehicleInfo()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Car static models...\n");
	#endif
	
	ManiaSonicModel::loadStaticModels();
}

void Car::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Car static models...\n");
	#endif
	
	ManiaSonicModel::deleteStaticModels();
}

const bool Car::isVehicle()
{
	return true;
}

Vector3f* Car::getCameraDirection()
{
    return &camDir;
}

void Car::setCameraDirection(Vector3f* newDirection)
{
    camDir.set(newDirection);
    camDirSmooth.set(newDirection);
}

void Car::setOnRocket(bool newOnRocket)
{
    onRocket = newOnRocket;
}

void Car::setIsBall(bool newIsBall)
{
    isBall = newIsBall;
}

void Car::setOnGround(bool newOnGround)
{
    onGround = newOnGround;
}

void Car::refreshCamera()
{
    //Animating the camera

    //only need this in case of FOV change
	//Master_makeProjectionMatrix();

	Vector3f camOffset(&camDirSmooth);
	camOffset.normalize();
	camOffset.scale(camRadius);

	float rotationVector[3];
	Maths::rotatePoint(rotationVector, 0, 0, 0, camDirSmooth.x, camDirSmooth.y, camDirSmooth.z, relativeUpSmooth.x, relativeUpSmooth.y, relativeUpSmooth.z, -(float)(Maths::PI/2));

	float newCameraOffset[3];
	Maths::rotatePoint(newCameraOffset, 0, 0, 0, rotationVector[0], rotationVector[1], rotationVector[2], camOffset.x, camOffset.y, camOffset.z, 0);
	camOffset.set(newCameraOffset[0], newCameraOffset[1], newCameraOffset[2]);

	Vector3f camHeight(&relativeUpSmooth);
	camHeight.normalize();
	camHeight.scale(camHeightOffset);

	Vector3f eye(getPosition());
	eye = eye - camOffset;
	eye = eye + camHeight;

	Vector3f target(getPosition());
	target = target + camHeight;

	Vector3f up(&relativeUpSmooth);
	up.normalize();

	float newUp[3];
	Maths::rotatePoint(newUp, 0, 0, 0, rotationVector[0], rotationVector[1], rotationVector[2], up.x, up.y, up.z, 0);
	up.set(newUp[0], newUp[1], newUp[2]);

	Vector3f camDelta = eye - target;
	camDelta.setLength(5); //this is what causes metal harbor to go through cam at beginning
	Vector3f camStart = target + camDelta;
	//CollisionChecker::debug = Input::inputs.INPUT_ACTION4;
	//std::fprintf(stdout, "camStart = (%f, %f, %f)  \ncamEnd = (%f, %f, %f)\n", camStart.x, camStart.y, camStart.z, eye.x, eye.y, eye.z);
	if (CollisionChecker::checkCollision(camStart.x, camStart.y, camStart.z, eye.x, eye.y, eye.z))
	{
        //std::fprintf(stdout, "there was a collision");
		Vector3f delta = eye - target;
		delta.setLength(3);
		Vector3f newPos(CollisionChecker::getCollidePosition());
		newPos = newPos - delta;
		eye.set(&newPos);
	}
	//CollisionChecker::debug = false;

	Global::gameCamera->setViewMatrixValues(&eye, &target, &up);
}

//Do a small 'pop off' off the wall
void Car::popOffWall()
{
    if (onGround)
    {
	    vel.x = vel.x + relativeUp.x * 90.0f;
	    vel.z = vel.z + relativeUp.z * 90.0f;
	    vel.y = vel.y + relativeUp.y * 90.0f;
	    onGround = false;
	    relativeUp.set(0, 1, 0);
    }
}

Vector3f Car::getCenterPosition()
{
    if (onGround)
    {
        return position + relativeUp.scaleCopy(displayBallOffset);
    }
    else
    {
        return position;
    }
}
