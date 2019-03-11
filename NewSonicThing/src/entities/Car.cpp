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
#include <iostream>
#include <algorithm>
#include <fstream>


extern float dt;

Car::Car()
{

}

Car::Car(float x, float y, float z)
{
	position.set(x, y, z);
	vel.set(0, 0, 0);
	relativeUp.set(0, 1, 0);
	relativeUpSmooth.set(0, 1, 0);
	relativeUpAnim.set(0, 1, 0);
	onGround = false;
	camDir.set(0, 0, -1);
	camDirSmooth.set(0, 0, -1);

	maniaSonicModel = new ManiaSonicModel; INCR_NEW
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
	homingAttackTimer = std::fmaxf(0.0f, homingAttackTimer - dt);

	setInputs();

	//Homing attack
	if (onGround)
	{
		homingAttackTimer = 0.0f;
		justHomingAttacked = false;
	}
	else
	{
		if (inputJump && !inputJumpPrevious && (isBall || isJumping) && !justHomingAttacked)
		{
			isJumping = false;
			isBall = true;
			isBouncing = false;
			justBounced = false;
			homingAttackTimer = homingAttackTimerMax;
			justHomingAttacked = true;

			float storedVelY = vel.y;
			vel.y = 0;

			if (vel.length() < 300.0f)
			{
				vel.setLength(300.0f);
			}

			vel.y = storedVelY;
			AudioPlayer::play(11, getPosition());
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
			vel = vel + relativeUpSmooth.scaleCopy(hoverPower*dt);
		}
	}

	//Ball
	if (onGround)
	{
		isJumping = false;
		justHomingAttacked = false;

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
			if ((inputAction  && !inputActionPrevious) || 
				(inputAction2 && !inputAction2Previous))
			{
				bufferedSpindashInput = true;
			}
		}

		if ((((inputAction && !inputActionPrevious) || (inputAction2 && !inputAction2Previous)) && canStartSpindash) || 
			(bufferedSpindashInput && (inputAction || inputAction2) && canStartSpindash))
		{
			if (!isSpindashing)
			{
				storedSpindashSpeed = vel.length();
			}
			isSpindashing = true;
		}

		if (!inputAction && !inputAction2)
		{
			isSpindashing = false;
			bufferedSpindashInput = false;
		}

		if (isSpindashing)
		{
			if (spindashTimer == 0)
			{
				AudioPlayer::play(14, getPosition());
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
			}
			spindashTimer = 0;
			storedSpindashSpeed = 0;
		}

		if (((inputAction  && !inputActionPrevious) ||
			 (inputAction2 && !inputAction2Previous)))
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
	if (onGround)
	{
		isGrinding = false;
	}

	if (onGround)
	{
		moveMeGround();
	}
	else
	{
		moveMeAir();
	}

	//Add to velocity based on the slope you are on
	if (onGround)
	{
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
	}



	if (!isGrinding)
	{
		//Twisting camera from user input
		camDir = Maths::rotatePoint(&camDir, &relativeUp, -inputX2*dt);

		//camera adjust to direction you are heading in
		if (onGround)
		{
			if (Global::isAutoCam)
			{
				camDir = Maths::interpolateVector(&camDir, &vel, fminf(vel.length()*0.014f*dt, 45.0f*dt));
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
			if (dot < -0.325f)
			{
				camDir = Maths::rotatePoint(&camDir, &perpen, -((dot+0.325f)*12)*dt);
			}
			else if (dot > -0.2f)
			{
				camDir = Maths::rotatePoint(&camDir, &perpen, -((dot+0.2f)*20)*dt);
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
	else
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

	//smoothing
	camDirSmooth = Maths::interpolateVector(&camDirSmooth, &camDir, 10*dt);
	relativeUpSmooth = Maths::interpolateVector(&relativeUpSmooth, &relativeUp, 3*dt);
	relativeUpAnim = Maths::interpolateVector(&relativeUpAnim, &relativeUp, 15*dt);

	if (velocityMovesPlayer)//(!(isGrinding or onRocket))
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
					homingAttackTimer = 0.0f;
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
							vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);
							Vector3f velToAddFromGravity(relativeUp);
							velToAddFromGravity.setLength(-gravityForce*dt);




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

				vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);

				Vector3f velToAddFromGravity(relativeUp);
				velToAddFromGravity.setLength(-gravityForce*dt);
				if (Input::inputs.INPUT_RB)
				{
					vel = vel - velToAddFromGravity.scaleCopy(4);
				}
			}
		}
	}

	camDir.normalize();

	//animating us
	updateAnimationValues();
	animate();

	//Animating the camera

	Master_makeProjectionMatrix();

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
	camDelta.setLength(10);
	Vector3f camStart = target + camDelta;
	if (CollisionChecker::checkCollision(camStart.x, camStart.y, camStart.z, eye.x, eye.y, eye.z))
	{
		Vector3f delta = eye - target;
		delta.setLength(2);
		Vector3f newPos(CollisionChecker::getCollidePosition());
		newPos = newPos - delta;
		eye.set(&newPos);
	}

	Global::gameCamera->setViewMatrixValues(&eye, &target, &up);

	Global::gameMainVehicleSpeed = (int)(vel.length());

	//Vector3f posDiffDelta = position - prevPos;
	//std::fprintf(stdout, "delta pos = %f\n\n", posDiffDelta.length()/dt);



	//std::fprintf(stdout, "%f\n\n\n\n", vel.length());

	Vector3f vnorm(&vel);
	vnorm.normalize();
	//std::fprintf(stdout, "pos  = [%f, %f, %f]\n", position.x, position.y, position.z);
	//std::fprintf(stdout, "norm = [%f, %f, %f]\n", currNorm.x, currNorm.y, currNorm.z);
	//std::fprintf(stdout, "dir  = [%f, %f, %f]\n", vnorm   .x, vnorm   .y, vnorm   .z);
	//std::fprintf(stdout, "%f %f %f   %f %f %f   %f %f %f\n", position.x, position.y, position.z, currNorm.x, currNorm.y, currNorm.z, vnorm.x, vnorm.y, vnorm.z);
}

void Car::spindash()
{
	Vector3f newDir = Maths::projectOntoPlane(&spindashDirection, &relativeUp);
	newDir.setLength(storedSpindashSpeed);
	vel.set(&newDir);
	std::fprintf(stdout, "spindash at %f speed\n", storedSpindashSpeed);
	isBall = true;
	AudioPlayer::play(15, getPosition());
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
	if (isSpindashing && vel.lengthSquared() < spindashPowerfulFrictionThreshold*spindashPowerfulFrictionThreshold) //if you are stopped and charging a spindash, dont move sonic
	{
		return;
	}

	float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
	float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
	Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
	dirForward.setLength(stickRadius);
	Vector3f velToAdd = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);

	if (stickRadius > 0.1f)
	{
		if (isBall)
		{
			Vector3f fr(0, -0.001f, 0);
			fr = Maths::projectOntoPlane(&fr, &relativeUp);
			float frictionPower = groundBallFriction*(1 - fr.length());
			vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction

			//rotate vel to where the stick is going
			float spd = vel.length();
			vel = Maths::interpolateVector(&vel, &velToAdd, dt*(60.0f/12.0f));
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
			}
			else
			{
				float spd = vel.length();
				vel = Maths::interpolateVector(&vel, &velToAdd, dt*(60.0f/12.0f));
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
			else
			{
				Vector3f fr(0, -0.95f, 0); //0.75
				fr = Maths::projectOntoPlane(&fr, &relativeUp);
				float frictionPower = groundNeutralFriction*(1 - fr.length());
				vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction
			}
		}
	}
}

void Car::moveMeAir()
{
	if (isGrinding or onRocket)
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
			if (storedVelY >= 0)
			{
				vel = Maths::applyDrag(&vel, -airRunFrictionUp, dt); //Slow vel down due to friction (but no slowdown in y direction)
			}
			else
			{
				vel = Maths::applyDrag(&vel, -airRunFrictionDown, dt); //Slow vel down due to friction (but no slowdown in y direction)
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
			vel = Maths::interpolateVector(&vel, &velToAdd, dt*(60.0f/12.0f));
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
	homingAttackTimer = -1.0f;
	vel = vel + relativeUp.scaleCopy(jumpPower);
	hoverTimer = hoverTimerThreshold;
	onGround = false;
	isJumping = true;
	AudioPlayer::play(12, getPosition());
}

void Car::setRelativeUp(Vector3f* newUp)
{
	relativeUp.set(newUp);
}

void Car::updateAnimationValues()
{
	float currSpeed = vel.length();

	if (isJumping)
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

	if (onRocket)
	{
		maniaSonicModel->setOrientation(getX(), getY(), getZ(), diffGround, yawAngleGround, pitchAngleGround, 0);
		maniaSonicModel->animate(27, 0);
	}
	if (onPulley)
	{
		maniaSonicModel->setOrientation(getX(), getY(), getZ(), diffGround, yawAngleGround, pitchAngleGround, 0);
		maniaSonicModel->animate(27, 0);
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
		maniaSonicModel->setOrientation(getX(), getY(), getZ(), diffGround, yawAngleGround, pitchAngleGround, 0);
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
		maniaSonicModel->animate(2, 0);
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
	inputX       = Input::inputs.INPUT_X;
	inputY       = Input::inputs.INPUT_Y;
	inputX2      = Input::inputs.INPUT_X2;
	inputY2      = Input::inputs.INPUT_Y2;

	inputJumpPrevious    = Input::inputs.INPUT_PREVIOUS_ACTION1;
	inputActionPrevious  = Input::inputs.INPUT_PREVIOUS_ACTION2;
	inputAction2Previous = Input::inputs.INPUT_PREVIOUS_ACTION3;

	if (canMoveTimer > 0.0f)
	{
		inputJump    = false;
		inputAction  = false;
		inputAction2 = false;
		inputX    = 0;
		inputY    = 0;
		inputX2   = 0;
		inputY2   = 0;

		inputJumpPrevious    = false;
		inputActionPrevious  = false;
		inputAction2Previous = false;
	}
}

std::list<TexturedModel*>* Car::getModels()
{
	return nullptr;
}

void Car::loadVehicleInfo()
{
	//if (Car::models[vehicleID].size() > 0)
	{
		//return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading Car static models...\n");
	#endif
	
	ManiaSonicModel::loadStaticModels();
	
	//std::string modelFolder = "";
	//std::string modelName = "";
	//switch (vehicleID)
	//{
	//	case 0: modelFolder = "res/Models/Characters/SADXSonic/";   modelName = "TPose";           break;
	//	case 4: modelFolder = "res/Models/Machines/Arwing/";       modelName = "Arwing";       break;
	//	case 5: modelFolder = "res/Models/Machines/RedGazelle/";   modelName = "RedGazelle";   break;
	//	case 1: modelFolder = "res/Models/Machines/TwinNorita/";   modelName = "TwinNorita";   break;
	//	case 3: modelFolder = "res/Models/Machines/BlackBull/";    modelName = "BlackBull";    break;
	//	case 2: modelFolder = "res/Models/Machines/SonicPhantom/"; modelName = "SonicPhantom"; break;
	//	default: break;
	//}
	//loadModel(&Car::models[vehicleID], modelFolder, modelName);
}

void Car::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting Car static models...\n");
	#endif
	
	ManiaSonicModel::deleteStaticModels();
	//
	//for (int i = 0; i < 30; i++)
	//{
	//	std::list<TexturedModel*>* e = &Car::models[i];
	//	if (e->size() > 0)
	//	{
	//		Entity::deleteModels(e);
	//	}
	//}
}

bool Car::isVehicle()
{
	return true;
}

bool Car::isChargingSpindash()
{
	return isSpindashing;
}

bool Car::isOnGround()
{
	return onGround;
}

void Car::setOnGround(bool newOnGround)
{
	onGround = newOnGround;
}

void Car::setHoverTimer(float newTimer)
{
	hoverTimer = newTimer;
}

void Car::setIsBall(bool newIsBall)
{
	isBall = newIsBall;
}

void Car::setSpindashTimer(float newSpindashTimer)
{
	spindashTimer = newSpindashTimer;
}

float Car::getHitboxHorizontal()
{
	return 6;
}

float Car::getHitboxVertical()
{
	return 12;
}

void Car::setOnRocket(bool newOnRocket) //for animation but that doesn't work atm
{
	onRocket = newOnRocket;
}

void Car::setVelocityMovesPlayer(bool newVelocityMovesPlayer)
{
	velocityMovesPlayer = newVelocityMovesPlayer;
}

void Car::setOnPulley(bool newOnPulley)
{
	onPulley = newOnPulley;
}