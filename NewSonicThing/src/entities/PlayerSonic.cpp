#include <glad/glad.h>

#include "entity.hpp"
#include "playersonic.hpp"
#include "../models/models.hpp"
#include "../toolbox/vector.hpp"
#include "../renderEngine/display.hpp"
#include "../loading/loadergl.hpp"
#include "../loading/modelloader.hpp"
#include "../engineTester/main.hpp"
#include "../entities/camera.hpp"
#include "../collision/collisionchecker.hpp"
#include "../collision/triangle3d.hpp"
#include "../toolbox/maths.hpp"
#include "../audio/audioplayer.hpp"
#include "../particles/particle.hpp"
#include "../particles/particleresources.hpp"
#include "../particles/particlemaster.hpp"
#include "../toolbox/input.hpp"
#include "../fontMeshCreator/guinumber.hpp"
#include "../toolbox/split.hpp"
#include "../audio/source.hpp"
#include "checkpoint.hpp"
#include "../guis/guimanager.hpp"
#include "CharacterModels/playermodel.hpp"
#include "CharacterModels/maniasonicmodel.hpp"
#include "CharacterModels/maniamightymodel.hpp"
#include "CharacterModels/maniatailsmodel.hpp"
#include "CharacterModels/maniaknucklesmodel.hpp"
#include "CharacterModels/supersonicmodel.hpp"
#include "ringmoving.hpp"
#include "../menu/timer.hpp"
#include "stage.hpp"
#include "shieldgreen.hpp"
#include "shieldmagnet.hpp"
#include "../guis/guiimage.hpp"
#include "../menu/hud.hpp"
#include "../renderEngine/masterrenderer.hpp"

#include <list>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>


extern float dt;

PlayerSonic::PlayerSonic()
{

}

PlayerSonic::PlayerSonic(float x, float y, float z)
{
    position.set(x+0.00001f, y, z+0.00001f);
    vel.set(0.00001f, 0.00001f, 0.00001f);
    relativeUp.set(0, 1, 0);
    relativeUpSmooth.set(0, 1, 0);
    relativeUpAnim.set(0, 1, 0);
    onGround = false;
    camDir.set(0, 0, -1);
    camDirSmooth.set(0, 0, -1);

    if (Global::gameSaveData.find("PLAY_AS") != Global::gameSaveData.end() && Global::gameSaveData["PLAY_AS"] == "Mighty")
    {
        playerModel = new ManiaMightyModel; INCR_NEW("Entity");
    }
    else
    {
        playerModel = new ManiaSonicModel; INCR_NEW("Entity");
    }
    Global::addEntity(playerModel);

    visible = false;
    loadVehicleInfo();

    if (homingAttackReticle == nullptr)
    {
        float aspectRatio = Display::ASPECT_RATIO;

        homingAttackReticle = new GuiImage(LoaderGL::loadTexture("res/Images/HomingReticleSmooth.png"), 0.5f, 0.5f, 0.1f/aspectRatio, 0.1f, 0); INCR_NEW("GuiImage");
        homingAttackReticle->visible = true;
    }

    nearbyEntities.reserve(10);
}

PlayerSonic::~PlayerSonic()
{
    if (homingAttackReticle != nullptr)
    {
        LoaderGL::deleteTexture(homingAttackReticle->textureId);
        delete homingAttackReticle; homingAttackReticle = nullptr; INCR_DEL("GuiImage");
    }

    if (sourceGrind != nullptr)
    {
        sourceGrind->stop();
        sourceGrind = nullptr;
    }

    if (sourceStomp != nullptr)
    {
        sourceStomp->stop();
        sourceStomp = nullptr;
    }
}

void PlayerSonic::step()
{
    canMoveTimer      = std::fmaxf(0.0f,  canMoveTimer      - dt);
    hitSpringTimer    = std::fmaxf(0.0f,  hitSpringTimer    - dt);
    hoverTimer        = std::fmaxf(0.0f,  hoverTimer        - dt);
    homingAttackTimer = std::fmaxf(-1.0f, homingAttackTimer - dt);
    hitTimer          = std::fmaxf(0.0f,  hitTimer          - dt);
    hitFlashingTimer  = std::fmaxf(0.0f,  hitFlashingTimer  - dt);
    speedShoesTimer   = std::fmaxf(0.0f,  speedShoesTimer   - dt);
    float deadTimerOld = deadTimer;
    if (deadTimer >= 0.0f)
    {
        deadTimer -= dt;
    }

    float invincibleTimerOld = invincibleTimer;
    invincibleTimer = std::fmaxf(0.0f, invincibleTimer - dt);
    int invTimOld = (int)(invincibleTimerOld*4);
    int invTimNew = (int)(invincibleTimer*4);
    if (invTimNew != invTimOld)
    {
        invincibleColor1.set(&invincibleColor2);
        invincibleColor2.set(Maths::nextUniform()*3+0.3f, Maths::nextUniform()*3+0.3f, Maths::nextUniform()*3+0.3f);
    }

    if (deadTimerOld >  1.0f && 
        deadTimer    <= 1.0f)
    {
        Vector3f partVel(0, 0, 0);
        ParticleMaster::createParticle(ParticleResources::textureBlackFadeOutAndIn, Global::gameCamera->getFadePosition1(), &partVel, 0, 2.0f, 400, 0, true, false, 1, false);
    }
    else if (deadTimerOld >  0.0f &&
             deadTimer    <= 0.0f)
    {
        Global::shouldLoadLevel = true;
    }

    setInputs();

    //to use later when doing running on water stuff
    Vector3f posBefore = position;
    Vector3f velBefore = vel;
    //Vector3f relativeUpBefore = relativeUp;
    bool onGroundBefore = onGround;

    // Dropdash
    if (!onGround && inputAction4 && !isLightdashing && !isBouncing && !isHomingOnPoint && !justHomingAttacked && !isGrinding && !isGrabbing && !isDriving)
    {
        dropdashTimer += dt;

        if (dropdashTimer >= dropdashTimerMax && !isDropdashing)
        {
            isDropdashing = true;
            isStomping = false;
            AudioPlayer::play(78, &position);
        }
    }
    else
    {
        dropdashTimer = 0.0f;
    }

    //Start Lightdash
    if (!isLightdashing)
    {
        if (inputAction3 && !inputAction3Previous && !isDriving)
        {
            lightdashTrail.clear();
            lightdashTrailProgress = -1.0f;

            //search through close entities to find rings
            Global::getNearbyEntities(position.x, position.z, &nearbyEntities, lightdashStartRingMinDist);

            //keep track of rings we've already used, to not use them again
            std::unordered_set<Entity*> alreadyUsedRings;

            float closest = 100000000.0f;
            Vector3f* closestPoint = nullptr;
            Entity* closestEntity = nullptr;

            for (std::unordered_set<Entity*>* set : nearbyEntities)
            {
                for (auto it = set->cbegin(); it != set->cend(); it++)
                {
                    Entity* e = *it;

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

            if (closest < lightdashStartRingMinDist*lightdashStartRingMinDist)
            {
                isLightdashing = true;
                lightdashTrailProgress = 0.0f;
                lightdashTrail.push_back(closestPoint);
                alreadyUsedRings.insert(closestEntity);

                Vector3f currentLightdashDir = vel;

                if (vel.lengthSquared() < 20.0f*20.0f)
                {
                    currentLightdashDir = (*closestPoint) - position;
                }

                currentLightdashDir.normalize();

                bool keepGoing = true;
                while (keepGoing)
                {
                    Vector3f center = lightdashTrail.back();
                    //search through close entities to find rings
                    Global::getNearbyEntities(center.x, center.z, &nearbyEntities, lightdashContinueRingMinDist);

                    float bestScore = -100000000000.0f;
                    Vector3f* bestPoint = nullptr;
                    Entity* bestEntity = nullptr;

                    for (std::unordered_set<Entity*>* set : nearbyEntities)
                    {
                        for (auto it = set->cbegin(); it != set->cend(); it++)
                        {
                            Entity* e = *it;

                            if (!e->canLightdashOn() || alreadyUsedRings.find(e) != alreadyUsedRings.end())
                            {
                                continue;
                            }

                            Vector3f diff = e->position - center;
                            float thisDistSquared = diff.lengthSquared();
                            if (thisDistSquared < lightdashContinueRingMinDist*lightdashContinueRingMinDist)
                            {
                                float thisDist = sqrtf(thisDistSquared);

                                Vector3f diffDir = diff;
                                diffDir.normalize();
                                float thisScore = (lightdashContinueRingMinDist - thisDist)*currentLightdashDir.dot(&diffDir);

                                if (thisScore > bestScore)
                                {
                                    bestScore = thisScore;
                                    bestPoint = &e->position;
                                    bestEntity = e;
                                }
                            }
                        }
                    }

                    if (bestScore > 0.0f) //you at least must turn < 90 degrees to the next ring
                    {
                        lightdashTrail.push_back(bestPoint);
                        alreadyUsedRings.insert(bestEntity);
                        currentLightdashDir = (*bestPoint) - center;
                        currentLightdashDir.normalize();
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

        if (isLightdashing)
        {
            if (onGround && relativeUp.y < wallStickThreshold)
            {
                reattachAfterLightdash = true;
            }
            else
            {
                reattachAfterLightdash = false;
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

            //re attach to te ground
            Vector3f down = position - relativeUp.scaleCopy(surfaceTension);
            if (reattachAfterLightdash && CollisionChecker::checkCollision(&position, &down))
            {
                currentTriangle = CollisionChecker::getCollideTriangle();
                relativeUp = currentTriangle->normal;
                position = CollisionChecker::getCollidePosition();
                position = position + relativeUp.scaleCopy(FLOOR_OFFSET);
                onGround = true;
                onGroundBefore = true;
            }
        }
    }

    //Homing attack
    homingAttackReticle->visible = false;
    GuiManager::addImageToRender(homingAttackReticle);
    if (onGround)
    {
        homingAttackTimer = -1.0f;
        justHomingAttacked = false;
        isHomingOnPoint = false;
    }
    else
    {
        if ((isBall || isJumping) && !justHomingAttacked && !isLightdashing && !isDropdashing)
        {
            Vector3f homeTar(0,0,0);
            bool foundTarget = findHomingTarget(&homeTar);
            if (foundTarget && !(canMoveTimer > 0.0f || Global::finishStageTimer >= 0.0f || hitTimer > 0.0f || deadTimer > -1.0f || hitFlashingTimer > 0.0f))
            {
                Vector2f pos = Maths::calcScreenCoordsOfWorldPoint(&homeTar);
                homingAttackReticle->getPosition()->set(&pos);
                homingAttackReticle->visible = true;
            }

            if (inputJump && !inputJumpPrevious)
            {
                homingAttack(&homeTar, foundTarget);
            }
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
            animationTime = 0;
            AudioPlayer::play(12, &position);
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

        if (inputAction2 && !inputAction2Previous && !isLightdashing)
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
                    AudioPlayer::play(41, &position); //roll
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

        if (isRunningOnWater)
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
                AudioPlayer::play(39, &position); //peel charge
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
        if (inputAction && !inputActionPrevious && (isJumping || isBall) && !isBouncing && !justHomingAttacked && !isStomping && !isLightdashing && !isDropdashing)
        {
            vel.y = bounceVel;
            isBouncing = true;
        }
    }

    //Stomping
    if (onGround)
    {
        isStomping = false;
    }
    else
    {
        if (inputAction2 && !inputAction2Previous && isJumping && !isBouncing && !justHomingAttacked && !isStomping && !isLightdashing && !isDropdashing && !isDriving)
        {
            if (sourceStomp != nullptr)
            {
                sourceStomp->stop();
                sourceStomp = nullptr;
            }
            sourceStomp = AudioPlayer::play(16, &position);
            isJumping = false;
            isStomping = true;
            vel.y = 0;
            vel.setLength(1.0f);
            vel.y = stompSpeed;
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
        isDropdashing = false;
        justHomingAttacked = false;

        //sparks
        if (vel.lengthSquared() > 140.0f*140.0f)
        {
            float chanceToSpawn = dt*60.0f;

            if (Maths::nextUniform() < chanceToSpawn) //on higher than 60fps, dont spawn every frame
            {
                Vector3f partVel(&vel);
                partVel.scale(0.75f);
                partVel = partVel + relativeUp.scaleCopy(5.0f);
                Vector3f rand(Maths::nextUniform() - 0.5f, Maths::nextUniform() - 0.5f, Maths::nextUniform() - 0.5f);
                rand.scale(30.0f);
                partVel = partVel + rand;
                ParticleMaster::createParticle(ParticleResources::textureSparkleYellow, &position, &partVel, 0.2f, 1.0f, false, true);
            }
        }

        float grindAudioPitch = 0.65f + vel.length()/1000.0f;
        int audioIdLoop = 57;
        int audioIdLand = 59;

        if (Global::levelId == LVL_GREEN_FOREST || 
            Global::levelId == LVL_FROG_FOREST  ||
            Global::levelId == LVL_DRAGON_ROAD)
        {
            audioIdLoop = 58;
            audioIdLand = 60;
        }

        if (sourceGrind == nullptr)
        {
            sourceGrind = AudioPlayer::play(audioIdLoop, &position, grindAudioPitch, true);
            AudioPlayer::play(audioIdLand, &position);
        }

        if (sourceGrind != nullptr)
        {
            sourceGrind->setPosition(position.x, position.y, position.z);
            sourceGrind->setPitch(grindAudioPitch);
        }
    }
    else
    {
        if (sourceGrind != nullptr)
        {
            sourceGrind->stop();
            sourceGrind = nullptr;
        }
    }

    //Skidding (more logic done in moveMeGround)
    if (!onGround)
    {
        isSkidding = false;
    }

    //spring stuff
    if (onGround)
    {
        hitSpringTimer = 0.0f;
    }

    //hit timer stuff
    if (onGround)
    {
        hitTimer = 0.0f;
    }

    //combo stuff
    if (onGround)
    {
        if (combo > 1)
        {
            Global::gameScore += 100*(combo - 1);
            HUD::displayPointBonus(combo - 1);
        }
        combo = 0;
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
    if (velocityMovesPlayer && !isGrinding && !isGrabbing && !isLightdashing)
    {
        //camera adjust to direction you are heading in
        if (onGround)
        {
            //Twisting camera from user input
            camDir = Maths::rotatePoint(&camDir, &relativeUp, -inputX2*dt);

            if (Global::isAutoCam)
            {
                //idea: dont adjust the camera if sonic is heading towards it
                if (vel.lengthSquared() > 20.0f*20.0f && camDir.dot(&vel) > -0.5f)
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
                if (std::abs(tryingToRotate) < std::abs(maxAngle))
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

                    //speed up smoothing
                    //printf("speed up smoothing\n");
                    //this works but makes this a little more choppy... not sure if worth
                    //camDirSmooth = Maths::interpolateVector(&camDirSmooth, &camDir, 3*dt);
                    //relativeUpSmooth = Maths::interpolateVector(&relativeUpSmooth, &relativeUp, 1*dt);
                }
            }
            else if (dot > -0.2f)
            {
                float tryingToRotate = -(((dot+0.2f)*40)*dt);
                float targetAngle = acosf(-0.2f);
                float currentAngle = acosf(dot);
                float maxAngle = currentAngle-targetAngle;
                //rotate whichever is "less" rotation
                if (std::abs(tryingToRotate) < std::abs(maxAngle))
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
                    //targetAngle = acosf(0.0f);
                    //currentAngle = acosf(dotSmooth);
                    //maxAngle = currentAngle-targetAngle;
                    //camDirSmooth = Maths::rotatePoint(&camDirSmooth, &perpen, maxAngle);

                    //speed up smoothing
                    camDirSmooth = Maths::interpolateVector(&camDirSmooth, &camDir, 10*dt);
                    relativeUpSmooth = Maths::interpolateVector(&relativeUpSmooth, &relativeUp, 3*dt);
                }
            }
        }

        if (!onGround)
        {
            //Twisting camera from user input
            camDir = Maths::rotatePoint(&camDir, &relativeUp, -inputX2*dt);

            Vector3f noY(vel);
            if (noY.y > 0)
            {
                noY.y = 0;
            }

            if (Global::isAutoCam && hitTimer == 0.0f)
            {
                camDir = Maths::interpolateVector(&camDir, &noY, fminf(noY.length()*0.01f*dt, 30.0f*dt));
            }

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
        //New idea: rotate a camera direction target vector instead, and then
        // interpolate towards that once art the end
        Vector3f camDirTarget(&vel);

        //point it a bit downwards
        Vector3f perpen = camDirTarget.cross(&relativeUp);
        camDirTarget = Maths::rotatePoint(&camDirTarget, &perpen, -0.15f);
        //player input
        camDirTarget = Maths::rotatePoint(&camDirTarget, &perpen,     -inputY2*0.05f);
        camDirTarget = Maths::rotatePoint(&camDirTarget, &relativeUp, -inputX2*0.05f);

        //the final actual moving of camDir to target
        camDir = Maths::interpolateVector(&camDir, &camDirTarget, vel.length()*0.03f*dt);
    }

    //smoothing
    camDirSmooth = Maths::interpolateVector(&camDirSmooth, &camDir, 10*dt);
    relativeUpSmooth = Maths::interpolateVector(&relativeUpSmooth, &relativeUp, 3*dt);
    relativeUpAnim = Maths::interpolateVector(&relativeUpAnim, &relativeUp, 15*dt);

    if (velocityMovesPlayer && !isGrinding && !isGrabbing && !isLightdashing)
    {
        //add vel from gravity
        //if (!onGround)
        //{
        //    if (isHomingOnPoint && homingAttackTimer > 0)
        //    {
        //            
        //    }
        //    else
        //    {
        //        vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);
        //
        //        Vector3f velToAddFromGravity(relativeUp);
        //        velToAddFromGravity.setLength(-gravityForce*dt);
        //        //vel = vel + velToAddFromGravity;
        //        if (Input::inputs.INPUT_RB)
        //        {
        //            vel = vel - velToAddFromGravity.scaleCopy(4);
        //        }
        //    }
        //}

        //CollisionChecker::checkCollision(-2568.486816f, -4019.688477f, -19681.607422f, -2571.251953f, -4011.733887f, -19681.025391f);

        //speed before adjusting
        float originalSpeed = vel.length();
        CollisionChecker::setCheckPlayer(true);
        //CollisionChecker::debug = false;
        //CollisionChecker::debugFilename = "1.obj";
        //std::vector<Vector3f> debugCollisionPath;
        //bool debugOnGroundBefore = onGround;
        //debugCollisionPath.push_back(position);
        if (CollisionChecker::checkCollision(position.x, position.y, position.z, position.x+vel.x*dt, position.y+vel.y*dt, position.z+vel.z*dt))
        {
            Vector3f* colNormal = &CollisionChecker::getCollideTriangle()->normal;

            if (CollisionChecker::getCollideTriangle()->isDeath())
            {
                die();
                increasePosition(vel.x*dt, vel.y*dt, vel.z*dt);
            }
            else if (onGround  == false) //Air to ground
            {
                if (isBouncing) //dont stick to the ground, bounce off it
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
                    AudioPlayer::play(8, &position);
                    setPosition(CollisionChecker::getCollidePosition());
                    increasePosition(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET);
                }
                else
                {
                    //check to see if it is a wall. if so then we cant stick to it
                    if ((CollisionChecker::getCollideTriangle()->isWall() || colNormal->y < wallStickThreshold) && hitSpringTimer == 0.0f) //When you get launched off a spring, we can stick on walls
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
                                //CollisionChecker::debugFilename = "2.obj";
                                if (CollisionChecker::checkCollision(position.x, position.y, position.z, position.x+velToMove.x, position.y+velToMove.y, position.z+velToMove.z) == false)
                                {
                                    increasePosition(velToMove.x, velToMove.y, velToMove.z);
                                }
                            }

                            onGround = false;
                            relativeUp.set(0, 1, 0);

                            if (isHomingOnPoint && homingAttackTimer > 0)
                            {
                            
                            }
                            else
                            {
                                vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);
                            
                                //Vector3f velToAddFromGravity(relativeUp);
                                //velToAddFromGravity.setLength(-gravityForce*dt);
                                //vel = vel + velToAddFromGravity;
                            }


                            //Vector3f velFrame = vel.scaleCopy(dt);
                            //Vector3f newDirection = Maths::projectOntoPlane(&velFrame, colNormal);
                            //Vector3f posAfterMoveToWall = Vector3f(CollisionChecker::getCollidePosition());
                            //Vector3f posDelta = posAfterMoveToWall - position;
                            //posAfterMoveToWall = posAfterMoveToWall + colNormal->scaleCopy(FLOOR_OFFSET);
                            //
                            //if (CollisionChecker::checkCollision(position.x, position.y, position.z, posAfterMoveToWall.x, posAfterMoveToWall.y, posAfterMoveToWall.z) == false)
                            //{
                            //    setPosition(&posAfterMoveToWall);
                            //}
                            //if (CollisionChecker::checkCollision(position.x, position.y, position.z, position.x + newDirection.x, position.y + newDirection.y, position.z + newDirection.z) == false)
                            //{
                            //    increasePosition(newDirection.x, newDirection.y, newDirection.z);
                            //}
                            //
                            //onGround = false;
                            //relativeUp.set(0, 1, 0);
                            //vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);
                            //Vector3f velToAddFromGravity(relativeUp);
                            //velToAddFromGravity.setLength(-gravityForce*dt);
                        }
                    }
                    else // stick to the ground and start running on it
                    {
                        currentTriangle = CollisionChecker::getCollideTriangle();
                        Vector3f newDirection = Maths::projectOntoPlane(&vel, colNormal);
                        vel.set(&newDirection);

                        setPosition(CollisionChecker::getCollidePosition());
                        increasePosition(colNormal->x*FLOOR_OFFSET, colNormal->y*FLOOR_OFFSET, colNormal->z*FLOOR_OFFSET);

                        relativeUp.set(colNormal);
                        onGround = true;

                        if (isDropdashing)
                        {
                            isDropdashing = false;
                            isBall = true;

                            AudioPlayer::play(15, &position);

                            vel.setLength(fmaxf(dropdashSpeed, originalSpeed));
                        }
                        else
                        {
                            isBall = false;
                        }
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
                        CollisionChecker::setCheckPlayer(true);
                        //CollisionChecker::debugFilename = std::to_string(distanceRemaining) + ".obj";
                        if (CollisionChecker::checkCollision(position.x, position.y, position.z, position.x+nextVel.x, position.y+nextVel.y, position.z+nextVel.z))
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
            //Vector3f debugBefore = position;
            increasePosition(vel.x * dt, vel.y * dt, vel.z * dt);

            //debugCollisionPath.push_back(position);

            bool checkPassed = false;
            if (onGround)
            {
                //CollisionChecker::debug = true;
                CollisionChecker::setCheckPlayer(true);
                //CollisionChecker::debugFilename = "4.obj";
                checkPassed = CollisionChecker::checkCollision(position.x, position.y, position.z, position.x - relativeUp.x * surfaceTension, position.y - relativeUp.y * surfaceTension, position.z - relativeUp.z * surfaceTension);
                //if (CollisionChecker::debug == false)
                //{
                //    printf("v %f %f %f\n", debugBefore.x, debugBefore.y, debugBefore.z);
                //    printf("v %f %f %f\n\n", position.x, position.y, position.z);
                //}
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
                    float dot = std::abs(vel.dot(colNormal) / len);
                    float fact = sqrtf(1 - dot * dot);
                    Vector3f bounce = Maths::bounceVector(&vel, colNormal, 1.0f);
                    Vector3f parallel = Maths::projectOntoPlane(&vel, colNormal);
                    bounce = Maths::projectOntoPlane(&bounce, &relativeUp);
                    parallel = Maths::projectOntoPlane(&parallel, &relativeUp);
                    Vector3f rotate1 = Maths::rotatePoint(&parallel, &relativeUp, 0.1f);
                    Vector3f rotate2 = Maths::rotatePoint(&parallel, &relativeUp, -0.1f);
                    vel = Maths::getCloserPoint(&rotate1, &rotate2, &bounce);
                    vel.setLength(len * fact);

                    canMoveTimer = hitWallTimePunish;
                    //AudioPlayer::play(4, getPosition());

                    increasePosition(colNormal->x * FLOOR_OFFSET * 2, colNormal->y * FLOOR_OFFSET * 2, colNormal->z * FLOOR_OFFSET * 2);
                }
                else
                {
                    currentTriangle = CollisionChecker::getCollideTriangle();
                    Vector3f* normal = &CollisionChecker::getCollideTriangle()->normal;

                    setPosition(CollisionChecker::getCollidePosition());
                    increasePosition(normal->x * FLOOR_OFFSET, normal->y * FLOOR_OFFSET, normal->z * FLOOR_OFFSET);

                    //speed before adjusting
                    float speed = vel.length();

                    Vector3f newDirection = Maths::projectOntoPlane(&vel, normal);

                    newDirection.normalize();
                    newDirection.x *= speed;
                    newDirection.y *= speed;
                    newDirection.z *= speed;
                    vel.set(&newDirection);

                    relativeUp.set(normal);
                    onGround = true;
                }
            }
            else
            {
                CollisionChecker::falseAlarm();
                //CollisionChecker::debug = false;
                onGround = false;

                relativeUp.set(0, 1, 0);

                if (isHomingOnPoint && homingAttackTimer > 0)
                {

                }
                else
                {
                    vel.y = Maths::approach(vel.y, gravityTerminal, gravityApproach, dt);

                    Vector3f velToAddFromGravity(relativeUp);
                    velToAddFromGravity.setLength(-gravityForce * dt);
                    //vel = vel + velToAddFromGravity;
#ifdef DEV_MODE
                    if (Input::inputs.INPUT_LB)
                    {
                        vel = vel - velToAddFromGravity.scaleCopy(4);
                    }
#endif
                }
            }
            //CollisionChecker::debug = false;
        }
    }

    if (onGround && currentTriangle->isDeath())
    {
        die();
    }

    camDir.normalize();

    //Bouncing from collision type
    if (onGround)
    {
        float bounceStrength = currentTriangle->bounceStrength();
        if (bounceStrength > 0.0f)
        {
            position = position + currentTriangle->normal;
            onGround = false;

            Vector3f velAlongLine = Maths::projectAlongLine(&vel, &currentTriangle->normal);
            float speedAlongLine = velAlongLine.length();

            float bounceSpeed = std::fmaxf(bounceStrength, speedAlongLine);

            vel = vel + currentTriangle->normal.scaleCopy(bounceSpeed);

            setHoverTimer(1.0f);
            relativeUp.set(0, 1, 0);

            justBounced = false;
            justHomingAttacked = false;
            isBall = false;
            isStomping = false;
            isBouncing = false;
            isJumping = true;

            float pitch = 1.0f - bounceSpeed/1000.0f;
            pitch = Maths::clamp(0.5f, pitch, 1.0f);

            AudioPlayer::play(6, &position, pitch);
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

    //transition from running on ground to water
    if (onGroundBefore && Global::stageUsesWater && Global::levelId != LVL_EMERALD_COAST && Global::levelId != LVL_SEASIDE_HILL &&
        posBefore.y >= Global::waterHeight &&
        position.y  <= Global::waterHeight)
    {
        Vector3f waterNormal(0, 1, 0);
        //when running fast, run on water
        float currSpeedSquared = vel.lengthSquared();
        if (currSpeedSquared >= runningOnWaterSpeedThreshold*runningOnWaterSpeedThreshold &&
            relativeUp.dot(&waterNormal) >= runOnWaterTransitionThreshold)
        {
            position.y = Global::waterHeight;
            onGround = true;
            isJumping = false;
            isSpindashing = false;

            if (isRunningOnWater)
            {
                if (relativeUp.x == 0 &&
                    relativeUp.y == 1 &&
                    relativeUp.z == 0) //since last frame, we didnt collide downward with the ground
                {
                    vel.y = 0;
                }
                else //the ground is right below us, ignore any speed changes from it
                {
                    float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
                    float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
                    Vector3f dirForward = Maths::projectOntoPlane(&camDir, &Y_AXIS);
                    dirForward.setLength(stickRadius);

                    Vector3f stickDirection = Maths::rotatePoint(&dirForward, &Y_AXIS, stickAngle);
                    vel.y = 0;

                    if (stickRadius > 0.1f)
                    {
                        vel = Maths::interpolateVector(&velBefore, &stickDirection, dt*(60.0f/12.0f));
                        vel.setLength(velBefore.length());
                    }
                    else
                    {
                        vel.setLength(runningOnWaterSpeedThreshold - 1.0f);
                    }
                }
            }
            else //we are running from the ground onto water for the first time
            {
                float newSpeed = sqrtf(currSpeedSquared);
                vel.y = 0;
                vel.setLength(newSpeed);
            }

            relativeUp.set(0, 1, 0);

            isRunningOnWater = true;
        }
        else
        {
            isRunningOnWater = false;
        }
    }
    else
    {
        isRunningOnWater = false;
    }

    if (isStomping && onGround)
    {
        if (sourceStomp != nullptr)
        {
            sourceStomp->stop();
            sourceStomp = nullptr;
        }

        AudioPlayer::play(17, &position);

        int numBubbles = ((int)abs((vel.y/60.0f) * 10)) + 18;
        for (int i = 0; i < numBubbles; i++)
        {
            float xOff = (18 * (Maths::random() - 0.5f));
            float zOff = (18 * (Maths::random() - 0.5f));

            Vector3f partPos = position;
            partPos.x += xOff;
            partPos.y += 2;
            partPos.z += zOff;

            Vector3f partVel(
                (Maths::random() - 0.5f) * 3 + (vel.x/60.0f)*0.8f,
                (Maths::random()*1.2f + 0.5f),
                (Maths::random() - 0.5f) * 3 + (vel.z/60.0f)*0.8f);
            partVel.scale(60.0f);

            ParticleMaster::createParticle(playerModel->getBallTexture(), &partPos, &partVel, 0.12f*60.0f*60.0f, 0.5f, 14.0f, -28.0f, false, false, 2.0f, true);
        }
    }

    //Water 
    if (position.y < Global::waterHeight - 5 && Global::stageUsesWater)
    {
        inWater = true;
        waterHeight = Global::waterHeight;
    }

    if (!inWater && inWaterPrevious)
    {
        AudioPlayer::play(5, &position);
        Vector3f partPos(&position);
        partPos.y = waterHeight + 5;
        ParticleMaster::createParticle(ParticleResources::textureSplash, &partPos, 0.5f, 10.0f, false, true);

        if (!onGround)
        {
            vel.y += 50.0f; //waterExitBoost
        }
        
        int numBubbles = ((int)abs((vel.y/60.0f) * 8)) + 18;
        for (int i = 0; i < numBubbles; i++)
        {
            float xOff = (7*(Maths::random() - 0.5f));
            float zOff = (7*(Maths::random() - 0.5f));

            Vector3f bubPos(
                position.x + xOff,
                waterHeight + 2,
                position.z + zOff);

            Vector3f bubVel(
                Maths::random() - 0.5f + (vel.x/60.0f)*0.4f,
                Maths::random()*0.3f + 0.2f + (vel.y/60.0f)*0.3f,
                Maths::random() - 0.5f + (vel.z/60.0f)*0.4f);

            bubVel.scale(60.0f);
            ParticleMaster::createParticle(ParticleResources::textureBubble, &bubPos, &bubVel, 60*60*0.25f, 1.0f, 4.0f, 0.0f, false, false, 1.0f, true);
        }
    }

    if (inWater && !inWaterPrevious)
    {
        AudioPlayer::play(5, &position);
        Vector3f partPos(&position);
        partPos.y = waterHeight + 5;
        ParticleMaster::createParticle(ParticleResources::textureSplash, &partPos, 0.5f, 10.0f, false, true);

        int numBubbles = ((int)abs((vel.y/60.0f) * 8)) + 18;
        for (int i = 0; i < numBubbles; i++)
        {
            float xOff = (7*(Maths::random() - 0.5f));
            float zOff = (7*(Maths::random() - 0.5f));

            Vector3f bubPos(
                position.x + xOff,
                waterHeight + 2,
                position.z + zOff);

            Vector3f bubVel(
                Maths::random() - 0.5f + (vel.x/60.0f)*0.4f,
                Maths::random()*0.3f + 0.2f - (vel.y/60.0f)*0.3f,
                Maths::random() - 0.5f + (vel.z/60.0f)*0.4f);

            bubVel.scale(60.0f);
            ParticleMaster::createParticle(ParticleResources::textureBubble, &bubPos, &bubVel, 60*60*0.05f, 1.0f, 4.0f, 0.0f, false, false, 1.0f, true);
        }

        vel.y = fmaxf(vel.y, -200.0f); //waterEntryMaxYVel

        Vector3f noY(&vel);
        noY.y = 0;
        float newSpeed = Maths::applyDrag(noY.length(), -0.35f, 1.0f); //waterEntrySlowdown
        noY.setLength(newSpeed);
        vel.x = noY.x;
        vel.z = noY.z;
    }

    //underwater friction
    if (inWater)
    {
        float newSpeed = Maths::applyDrag(vel.length(), -0.35f, dt); //waterDeceleration
        vel.setLength(newSpeed);
    }

    if (position.y < Global::deathHeight)
    {
        die();
    }

    //Animating us
    updateAnimationValues();
    animate();

    inWaterPrevious = inWater;
    inWater = false;

    //if (Global::shouldLogRace)
    {
        playerModel->log();
        //fprintf(stdout, "log\n");
    }

    //Animating the camera
    refreshCamera();

    Global::gameMainVehicleSpeed = (int)(vel.length());

    //Vector3f posDiffDelta = position - prevPos;
    //printf("delta pos = %f\n\n", posDiffDelta.length()/dt);


    //printf("%f\n", animationTime);
    //printf("%f\n\n\n\n", vel.length());

    //printf("camDir       = [%f, %f, %f]\n", camDir.x, camDir.y, camDir.z);
    //printf("camDirSmooth = [%f, %f, %f]\n", camDirSmooth.x, camDirSmooth.y, camDirSmooth.z);

    //Vector3f vnorm(&vel);
    //vnorm.normalize();
    //printf("pos  = [%f, %f, %f]\n", position.x, position.y, position.z);
    //printf("norm = [%f, %f, %f]\n", currNorm.x, currNorm.y, currNorm.z);
    //printf("dir  = [%f, %f, %f]\n", vnorm   .x, vnorm   .y, vnorm   .z);
    //printf("%f %f %f   %f %f %f   %f %f %f\n", position.x, position.y, position.z, currNorm.x, currNorm.y, currNorm.z, vnorm.x, vnorm.y, vnorm.z);
    centerPosPrev = getCenterPosition();
}

void PlayerSonic::spindash()
{
    Vector3f newDir = Maths::projectOntoPlane(&spindashDirection, &relativeUp);
    newDir.setLength(storedSpindashSpeed);
    vel.set(&newDir);
    //printf("spindash at %f speed\n", storedSpindashSpeed);
    //isBall = true;
    isBall = false; //geeks idea change
    AudioPlayer::play(40, &position); //peel release
    storedSpindashSpeed = 0;

    //burst of speed = burst of particles
    for (int i = 0; i < 30; i++)
    {
        Vector3f rng(Maths::nextUniform()-0.5f, Maths::nextUniform()-0.5f, Maths::nextUniform()-0.5f);
        rng.scale(300.0f);
        rng = Maths::projectOntoPlane(&rng, &relativeUp);

        Vector3f spd = relativeUp.scaleCopy(55.0f) + rng;
        Vector3f partPos = position + relativeUp.scaleCopy(1.5f);
        ParticleMaster::createParticle(ParticleResources::textureDust, &partPos, &spd, 0, 0.25f + (0.125f*Maths::nextGaussian()), 5.0f+Maths::nextGaussian(), 0.0f, false, false, 1.0f, true);
    }
}

void PlayerSonic::calcSpindashDirection()
{
    float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
    float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
    Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
    dirForward.setLength(stickRadius);
    Vector3f newDir = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);

    if (stickRadius >= 0.6f)
    {
        spindashDirection.set(&newDir);
    }
    else if (vel.length() > 10.0f)
    {
        spindashDirection.set(&vel);
    }
}

void PlayerSonic::moveMeGround()
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
            if (speedShoesTimer <= 0.0f)
            {
                //Add vel from player stick input
                if (vel.lengthSquared() < groundRunPushThreshold*groundRunPushThreshold)
                {
                    vel = vel + velToAdd.scaleCopy(3*groundRunPush*dt);
                }
                else
                {
                    vel = vel + velToAdd.scaleCopy(groundRunPush*dt);
                }
            }
            else
            {
                vel = vel + velToAdd.scaleCopy(groundRunPushSpeedshoes*dt); //Add even more vel from player stick input
            }

            Vector3f fr(0, -0.001f, 0); //-.25
            fr = Maths::projectOntoPlane(&fr, &relativeUp);
            float frictionPower = groundRunFriction*(1 - fr.length());
            vel = Maths::applyDrag(&vel, -frictionPower, dt); //Slow vel down due to friction

            //rotate vel to where the stick is going
            float ang = Maths::angleBetweenVectors(&vel, &velToAdd);
            if (ang > Maths::toRadians(120.0f) && vel.lengthSquared() > 30*30 ) //skid
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
    //printf("relativeU.y = %f\n", relativeUp.y);
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

    if (skidded)
    {
        vel = Maths::applyDrag(&velBefore, skidPower, dt);
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

void PlayerSonic::moveMeAir()
{
    if (!velocityMovesPlayer || isGrinding || isLightdashing || (isHomingOnPoint && homingAttackTimer > 0) || isGrabbing)
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
        float storedVelY = vel.y;
        vel.y = 0;
        //Add vel from player stick input
        if (vel.lengthSquared() > airFrictionThreshold*airFrictionThreshold)
        {
            vel = vel + velToAdd.scaleCopy(airRunPush*dt);
        }
        else
        {
            vel = vel + velToAdd.scaleCopy(3.5f*airRunPush*dt); //Move a bit more if youre going slow
        }
        vel.y = storedVelY;

        if (vel.lengthSquared() > 0.00001f)
        {
            storedVelY = vel.y;
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
        storedVelY = vel.y;
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

void PlayerSonic::setVelocity(float xVel, float yVel, float zVel)
{
    vel.x = xVel;
    vel.y = yVel;
    vel.z = zVel;
}

Vector3f* PlayerSonic::getVelocity()
{
    return &vel;
}

void PlayerSonic::setCanMoveTimer(float newTimer)
{
    canMoveTimer = newTimer;
}

void PlayerSonic::startGrinding()
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

void PlayerSonic::stopGrinding()
{
    isGrinding = false;
}

void PlayerSonic::jump()
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
    AudioPlayer::play(12, &position);
}

void PlayerSonic::jumpOffPulley(Vector3f forwardDirectionVector)
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
    onGround = false;
    isJumping = true;

    forwardDirectionVector.normalize();
    if (inputX == 0 and inputY == 0)
    {
        //Stick isn't being held, move straight forward instead
        vel = forwardDirectionVector.scaleCopy(jumpPowerPulley);
    }
    else
    {
        //Stick is being held, move in that direction
        float stickAngle = -atan2f(inputY, inputX) - Maths::PI/2; //angle you are holding on the stick, with 0 being up
        float stickRadius = sqrtf(inputX*inputX + inputY*inputY);
        Vector3f dirForward = Maths::projectOntoPlane(&camDir, &relativeUp);
        dirForward.setLength(stickRadius);
        Vector3f velNew = Maths::rotatePoint(&dirForward, &relativeUp, stickAngle);
        vel = velNew.scaleCopy(jumpPowerPulley);
    }
    

    //jump sound
    AudioPlayer::play(12, &position);
}

void PlayerSonic::rebound(Vector3f* source)
{
    if (!onGround && !isGrinding && !isLightdashing)
    {
        if (inputJump)
        {
            vel.y = 0;
            vel.setLength(5.0f);
        }
        else  //retain speed if you don't hold A during impact
        {
            if (homingAttackTimer > 0.0f) //if you came in from a homingAttack, cut speed a bit
            {
                vel.y = 0;
                vel.setLength(vel.length()*0.45f);
            }
        }

        if (!inputJump)
        {
            if (vel.y < 126.0f)
            {
                vel.y = 126.0f;
            }
        }
        else
        {
            vel.y = 126.0f;
        }

        position.set(source->x, source->y+0.01f, source->z);
        homingAttackTimer = -1.0f;
        justHomingAttacked = false;
        isHomingOnPoint = false;
        hoverTimer = hoverTimerThreshold/2;

        isGrinding = false;
        isBouncing = false;
        isBall = false;
        isLightdashing = false;
        isSkidding = false;
        isSpindashing = false;
        isStomping = false;
        justBounced = false;
        onGround = false;
        isJumping = true;
    }
}

void PlayerSonic::goThroughDashRing(Vector3f* center, Vector3f* dir, float power, float lockTime, bool changeCamera)
{
    vel = dir->scaleCopy(power);

    if (changeCamera)
    {
        camDir.set(dir);
        camDir.y = 0;
        camDir.normalize();
    }
    canMoveTimer = lockTime;
    position.set(center);

    isGrinding = false;
    isBouncing = false;
    isBall = false;
    isLightdashing = false;
    isSkidding = false;
    isSpindashing = false;
    isStomping = false;
    justBounced = false;
    isDropdashing = false;
    onGround = false;
    isJumping = true;
    homingAttackTimer = -1.0f;
    justHomingAttacked = false;
    isHomingOnPoint = false;
    hoverTimer = 0;
}

void PlayerSonic::takeDamage(Vector3f* source)
{
    if (hitTimer == 0.0f && invincibleTimer == 0.0f && hitFlashingTimer == 0.0f)
    {
        Vector3f posDiff = position - source;
        posDiff.y = 0;
        posDiff.setLength(90.0f);
        onGround = false;
        vel.x = posDiff.x;
        vel.z = posDiff.z;
        vel.y = 60*1.5f;
        hitTimer = 2.0f;
        hitFlashingTimer = hitTimer;
        isJumping = false;
        isSpindashing = false;
        isSkidding = false;
        isBall = false;
        isBouncing = false;
        isStomping = false;
        isLightdashing = false;
        spindashReleaseTimer = 0;
        spindashRestartDelay = 0;

        if (myShieldGreen != nullptr || myShieldMagnet != nullptr)
        {
            if (myShieldMagnet != nullptr)
            {
                Global::deleteEntity(myShieldMagnet);
                myShieldMagnet = nullptr;
            }
            if (myShieldGreen != nullptr)
            {
                Global::deleteEntity(myShieldGreen);
                myShieldGreen = nullptr;
            }
        }
        else
        {
            int ringsToScatter = Global::gameRingCount;
            Global::gameRingCount = 0;

            if (ringsToScatter == 0)
            {
                die();
            }
            else
            {
                AudioPlayer::play(10, &position);
            }

            ringsToScatter = std::min(ringsToScatter, 50);

            while (ringsToScatter > 0)
            {
                float spoutSpd = 3.5f*60.0f;
                float anglH = (float)(Maths::PI * 2 * Maths::random());
                float anglV = (Maths::toRadians(Maths::nextGaussian() * 42 + 90));
            
                float yspd = (spoutSpd*sin(anglV));
                float hpt = (spoutSpd*cos(anglV));
            
                float xspd = (hpt*cos(anglH));
                float zspd = (hpt*sin(anglH));
            
                RingMoving* ring = new RingMoving(position.x, position.y+5, position.z, xspd, yspd, zspd); INCR_NEW("Entity")
            
                Global::addEntity(ring);
            
                ringsToScatter--;
            }
        }
    }
}

bool PlayerSonic::isVulnerable()
{
    return !(
        homingAttackTimer > 0   ||
        isBouncing              ||
        isJumping               ||
        isBall                  ||
        isSpindashing           ||
        isStomping              ||
        isDropdashing           ||
        hitTimer > 0.0f         ||
        hitFlashingTimer > 0.0f ||
        invincibleTimer > 0.0f  ||
        Global::finishStageTimer >= 0);
}

bool PlayerSonic::findHomingTarget(Vector3f* target)
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
    Global::getNearbyEntities(position.x, position.z, &nearbyEntities, homingAttackRangeMax);

    float closestDist = homingAttackRangeMax*homingAttackRangeMax;
    float bestDotProduct = -1.0f;

    bool homeInOnPoint = false;
    Vector3f homeTargetPoint(0, 0, 0);

    if (lookingForClosest)
    {
        for (std::unordered_set<Entity*>* set : nearbyEntities)
        {
            for (auto it = set->cbegin(); it != set->cend(); it++)
            {
                Entity* e = *it;

                if (!e->canHomingAttackOn())
                {
                    continue;
                }

                Vector3f otherHomingCenter = e->getHomingCenter();

                Vector3f diff = position - otherHomingCenter;
                if (diff.y < -12 || diff.y > 80)
                {
                    continue;
                }

                float thisdist = diff.lengthSquared();
                if (thisdist < closestDist && !CollisionChecker::checkCollision(&position, &otherHomingCenter))
                {
                    closestDist = thisdist;
                    homeTargetPoint = otherHomingCenter;
                    homeInOnPoint = true;
                }
            }
        }
    }
    else 
    {
        for (std::unordered_set<Entity*>* set : nearbyEntities)
        {
            for (auto it = set->cbegin(); it != set->cend(); it++)
            {
                Entity* e = *it;

                if (!e->canHomingAttackOn())
                {
                    continue;
                }

                Vector3f otherHomingCenter = e->getHomingCenter();

                Vector3f diff = position - otherHomingCenter;
                if (diff.y < -12 || diff.y > 80)
                {
                    continue;
                }

                if (diff.lengthSquared() >= homingAttackRangeMax*homingAttackRangeMax)
                {
                    continue;
                }

                diff.y = 0;
                diff.neg();
                diff.normalize();

                float thisdot = diff.dot(&stickDirection);
                if (thisdot > bestDotProduct && !CollisionChecker::checkCollision(&position, &otherHomingCenter))
                {
                    bestDotProduct = thisdot;
                    homeTargetPoint = otherHomingCenter;
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
        target->set(&homeTargetPoint);
    }
    
    return homeInOnPoint;
}

void PlayerSonic::homingAttack(Vector3f* target, bool homeInOnIt)
{
    homingAttackTimer = homingAttackTimerMax;

    if (homeInOnIt)
    {
        Vector3f homeTargetPoint(target);
        vel = homeTargetPoint - position;
        vel.setLength(6.7f*60.0f);

        isHomingOnPoint = true;
        homingAttackTimer+=0.2f; //give some more time to home in on a target
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

    isBall = false;
    isJumping = true;
    isBouncing = false;
    isStomping = false;
    justBounced = false;
    justHomingAttacked = true;
    AudioPlayer::play(11, &position);
}

void PlayerSonic::setRelativeUp(Vector3f* newUp)
{
    relativeUp.set(newUp);
}

void PlayerSonic::updateAnimationValues()
{
    float currSpeed = vel.length();

    if (deadTimer > -1.0f)
    {
        animationTime = 0;
    }
    else if (isLightdashing)
    {
        animationTime = 0;
    }
    else if (isDropdashing)
    {
        animationTime -= 4000 * dt;
    }
    else if (isDriving)
    {
        animationTime = 0;
    }
    else if (isGrabbing)
    {
        animationTime = 0;
    }
    else if (isJumping)
    {
        animationTime -= 3000*dt;
    }
    else if (isBouncing)
    {
        animationTime -= 3500*dt;
    }
    else if (isStomping)
    {
        animationTime = 0;
    }
    else if (isBall)
    {
        animationTime -= (8.0f*currSpeed + 300)*dt;
    }
    else if (isGrinding)
    {
        animationTime = 0;
    }
    else if (hitTimer > 0.0f)
    {
        animationTime = 0;
    }
    else if (isSpindashing)
    {
        if (spindashTimer >= spindashTimerMax)
        {
            animationTime -= 2500*dt;
        }
        else
        {
            animationTime = -(spindashTimer*spindashTimer*0.8f*60*60);
        }
    }
    else if (isSkidding)
    {
        animationTime = 0;
    }
    else if (spindashReleaseTimer > 0)
    {
        animationTime = (spindashReleaseTimer*spindashReleaseTimer*0.4f*60*60);
    }
    else if (onGround && currSpeed < 0.6f) //stand
    {
        animationTime += 60.0f*dt;
        animationTime = fmaxf(0.0f, animationTime);
    }
    else if (!onGround) //freefall
    {
        animationTime = 0;
    }
    else //running animation
    {
        animationTime += (1.5f*currSpeed)*dt;
        animationTime = fmaxf(0.0f, animationTime);

        if (currSpeed > spindashPowerMax-20.0f)
        {
            float chanceToSpawn = dt*60.0f;

            if (Maths::nextUniform() < chanceToSpawn) //on higher than 60fps, dont spawn every frame
            {
                Vector3f rng(Maths::nextUniform()-0.5f, Maths::nextUniform()-0.5f, Maths::nextUniform()-0.5f);
                rng.scale(75.0f);
                rng = Maths::projectOntoPlane(&rng, &relativeUp);

                if (isRunningOnWater || (Global::stageUsesWater && (position.y < Global::waterHeight && position.y > Global::waterHeight - 10.0f)))
                {
                    float partScale = 5.0f+Maths::nextGaussian();
                    Vector3f spd = rng;
                    Vector3f partPos = position;
                    partPos.y = Global::waterHeight + partScale/2;
                    //todo make this look good
                    ParticleMaster::createParticle(ParticleResources::textureSplash, &partPos, &spd, 0, 0.25f + (0.125f*Maths::nextGaussian()), partScale, 0.0f, false, false, 1.0f, true);
                }
                else
                {
                    Vector3f spd = relativeUp.scaleCopy(55.0f) + rng;
                    Vector3f partPos = position + relativeUp.scaleCopy(1.5f);
                    ParticleMaster::createParticle(ParticleResources::textureDust, &partPos, &spd, 0, 0.25f + (0.125f*Maths::nextGaussian()), 5.0f+Maths::nextGaussian(), 0.0f, false, false, 1.0f, true);
                }
                //new Particle(ParticleResources::textureDust, &partPos, 0.25f, 6.0f, 2.8f, false);
            }
        }
    }

    ParticleTexture* trail = nullptr;
    float density = 2.0f; //1 particle every 2 units
    float opacity = 1.0f;

    if (deadTimer >= 0)
    {
        
    }
    else if (isLightdashing)
    {
        trail = ParticleResources::textureOrangeTrail;
        density = 0.75f;
        opacity = 0.25f;
    }
    else if (homingAttackTimer > 0 ||
            (isJumping && (hoverTimer > 0 && inputJump)) ||
            (isBall && onGround) ||
            isBouncing || 
            isStomping)
    {
        trail = playerModel->getBallTexture();
        density = 0.75f;
    }

    if (trail != nullptr)
    {
        Vector3f centerNew = getCenterPosition();
        Vector3f diff = centerNew - centerPosPrev;
        float diffLength = diff.length();
        int numParticles = (int)(diffLength/density);
        float remainder = fmodf(diffLength/density, 1.0f);

        if (Maths::nextUniform() < remainder)
        {
            numParticles++;
        }

        Vector3f zero(0, 0, 0);

        for (int i = 0; i < numParticles; i++)
        {
            Vector3f partPos = centerPosPrev + diff.scaleCopy(((float)i)/numParticles);
            ParticleMaster::createParticle(trail, &partPos, &zero, 0, 0.25f, 8.0f, -32.0f, false, false, opacity, true);
        }
    }

    if (invincibleTimer > 0.0f)
    {
        //remove this in favor of baseColor rainbow effect
        //Vector3f center = getCenterPosition();
        //
        //for (int i = 0; i < 2; i++)
        //{
        //    Vector3f off = Maths::randomPointOnSphere();
        //    off.scale(8);
        //    Vector3f pos1 = center + off;
        //    Vector3f pos2 = center - off;
        //    off.scale((1/16.0f)*60);
        //    ParticleMaster::createParticle(ParticleResources::textureSparkleYellow, &pos1, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
        //    off.scale(-1);
        //    ParticleMaster::createParticle(ParticleResources::textureSparkleGreen, &pos2, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
        //
        //    off = Maths::randomPointOnSphere();
        //    off.scale(8);
        //    pos1 = center + off;
        //    pos2 = center - off;
        //    off.scale((1/16.0f)*60);
        //    ParticleMaster::createParticle(ParticleResources::textureSparkleRed, &pos1, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
        //    off.scale(-1);
        //    ParticleMaster::createParticle(ParticleResources::textureSparkleBlue, &pos2, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
        //
        //    off = Maths::randomPointOnSphere();
        //    off.scale(8);
        //    pos1 = center + off;
        //    pos2 = center - off;
        //    off.scale((1/16.0f)*60);
        //    ParticleMaster::createParticle(ParticleResources::textureSparkleLightBlue, &pos1, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
        //    off.scale(-1);
        //    ParticleMaster::createParticle(ParticleResources::textureSparkleWhite, &pos2, &off, 0, 0.0833f, 0, 3.0f, -(3.0f / 0.0833f), false, false, 1.0f);
        //}
    }
}

void PlayerSonic::animate()
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
            Vector3f ballOffset = relativeUp.scaleCopy(playerModel->getDisplayBallOffset());
            dspX += ballOffset.x;
            dspY += ballOffset.y;
            dspZ += ballOffset.z;
        }
        else
        {
            //Vector3f displayOffset = relativeUp.scaleCopy(maniaSonicModel->displayHeightOffset);
            //dspX += displayOffset.x;
            //dspY += displayOffset.y;
            //dspZ += displayOffset.z;
        }
    }

    bool isVisibleFlashing = true;
    if (fmodf(hitFlashingTimer, 0.1f) > 0.05f)
    {
        isVisibleFlashing = false;

    }
    playerModel->setBaseVisibility(isVisibleFlashing);

    if (deadTimer > -1.0f)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, 0, airYaw, 0, 0, &relativeUpAnim);
        playerModel->animate(19, 0);
    }
    else if (isLightdashing)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, 0, airYaw, 90, airPitch, &relativeUpAnim);
        playerModel->animate(18, 0);
    }
    else if (isDropdashing)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, animationTime, &relativeUpAnim);
        playerModel->animate(12, 0);
    }
    else if (isDriving)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, rotX, rotY, rotZ, rotRoll, &relativeUp);
        playerModel->animate(27, 0);
    }
    else if (isGrabbing)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, 0, &relativeUpAnim);
        playerModel->animate(25, 0);
    }
    else if (isJumping)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, animationTime, &relativeUpAnim);
        playerModel->animate(12, 0);
    }
    else if (isBouncing)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, animationTime, &relativeUpAnim);
        playerModel->animate(12, 0);
    }
    else if (isStomping)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, 0, airYaw, 180, airPitch, &relativeUpAnim);
        playerModel->animate(3, 0);
    }
    else if (isBall)
    {
        if (onGround)
        {
            playerModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, animationTime, &relativeUpAnim);
        }
        else
        {
            playerModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, animationTime, &relativeUpAnim);
        }
        playerModel->animate(12, 0);
    }
    else if (isGrinding)
    {
        playerModel->setOrientation(dspX-nXAir, dspY-nYAir, dspZ-nZAir, diffGround, yawAngleGround, pitchAngleGround, -Input::inputs.INPUT_X*15, &relativeUpAnim);
        playerModel->animate(26, 0);
    }
    else if (hitTimer > 0.0f)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, 0, airYaw, 90, airPitch/8.0f, &Y_AXIS);
        playerModel->animate(11, hitTimer);
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

        playerModel->setOrientation(dspX, dspY, dspZ, diffGroundSpnd, yawAngleGroundSpnd, pitchAngleGroundSpnd, animationTime, &relativeUpAnim);
        playerModel->animate(12, 0);
    }
    else if (isSkidding)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, 0, &relativeUpAnim);
        playerModel->animate(8, 0);
    }
    else if (spindashReleaseTimer > 0)
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, animationTime, &relativeUpAnim);
        playerModel->animate(12, 0);
    }
    else if (onGround && currSpeed < 0.6f) //stand
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, 0, &relativeUpAnim);
        playerModel->animate(0, animationTime);
    }
    else if (!onGround) //freefall
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffAir, yawAngleAir, pitchAngleAir, 0, &relativeUpAnim);
        playerModel->animate(21, 0);
    }
    else //running animation
    {
        playerModel->setOrientation(dspX, dspY, dspZ, diffGround, yawAngleGround, pitchAngleGround, 0, &relativeUpAnim);

        if (currSpeed < 200)
        {
            playerModel->animate(15, animationTime);
        }
        else
        {
            playerModel->animate(1, animationTime);
        }
    }

    Vector3f newBaseColor(1, 1, 1);
    //change color with shields/invincible
    if (myShieldGreen != nullptr)
    {
        newBaseColor.set(1.0f, 2.0f, 1.0f);
    }
    if (myShieldMagnet != nullptr)
    {
        newBaseColor.set(1.0f, 1.0f, 2.0f);
    }
    if (invincibleTimer > 0.0f)
    {
        float progress = 1 - (fmodf(invincibleTimer, 0.25f)*4);
        float newR = Maths::interpolate(invincibleColor1.x, invincibleColor2.x, progress);
        float newG = Maths::interpolate(invincibleColor1.y, invincibleColor2.y, progress);
        float newB = Maths::interpolate(invincibleColor1.z, invincibleColor2.z, progress);
        newBaseColor.set(newR, newG, newB);
    }
    else if (speedShoesTimer > 0.0f)
    {
        float progress = 0.5f + 0.5f*sinf(speedShoesTimer*10.0f);
        float newR = Maths::interpolate(2.3f, 1.0f, progress);
        float newG = Maths::interpolate(3.0f, 1.0f, progress);
        float newB = Maths::interpolate(2.5f, 1.0f, progress);
        newBaseColor.set(newR, newG, newB);
    }
    playerModel->setBaseColor(newBaseColor.x, newBaseColor.y, newBaseColor.z);

    if (Global::finishStageTimer >= 1.0f)
    {
        position.set(&Global::gameStage->finishPlayerPosition);
        vel.set(&Global::gameStage->finishPlayerDir);
        vel.setLength(0.001f);
        relativeUp.set(&Global::gameStage->finishPlayerUp);
        relativeUp.normalize();

        camDir.set(&Global::gameStage->finishCameraDir);
        camDir.normalize();
        camDirSmooth.set(&camDir);

        isJumping = false;
        isSpindashing = false;
        isSkidding = false;
        isBall = false;
        isBouncing = false;
        isStomping = false;
        isLightdashing = false;
        isGrabbing = false;
        isGrinding = false;
        isDropdashing = false;

        groundSpeeds = Maths::calculatePlaneSpeed(vel.x, vel.y, vel.z, &relativeUp);
        twistAngleGround = Maths::toDegrees(atan2f(-groundSpeeds.z, groundSpeeds.x));
        nXGround = relativeUpAnim.x;
        nYGround = relativeUpAnim.y;
        nZGround = relativeUpAnim.z;
        normHLengthGround = sqrtf(nXGround*nXGround + nZGround*nZGround);
        pitchAngleGround = Maths::toDegrees(atan2f(nYGround, normHLengthGround));
        yawAngleGround = Maths::toDegrees(atan2f(-nZGround, nXGround));
        diffGround = Maths::compareTwoAngles(twistAngleGround, yawAngleGround);

        float finishTimeAnim = fmaxf(0, fminf((float)(Global::finishStageTimer-1.666f)*2.0f*60.0f, 99.999f));

        playerModel->setOrientation(position.x, position.y, position.z, diffGround, yawAngleGround, pitchAngleGround, 0, &relativeUp);
        playerModel->animate(14, finishTimeAnim);
    }
}

void PlayerSonic::setInputs()
{
    inputJump    = Input::inputs.INPUT_ACTION1;
    inputAction  = Input::inputs.INPUT_ACTION2;
    inputAction2 = Input::inputs.INPUT_ACTION3;
    inputAction3 = Input::inputs.INPUT_ACTION4;
    inputAction4 = Input::inputs.INPUT_RB;
    inputX       = Input::inputs.INPUT_X;
    inputY       = Input::inputs.INPUT_Y;
    inputX2      = Input::inputs.INPUT_X2;
    inputY2      = Input::inputs.INPUT_Y2;
    inputZoom    = (int)Input::inputs.INPUT_DPADU - (int)Input::inputs.INPUT_DPADD;

    inputJumpPrevious    = Input::inputs.INPUT_PREVIOUS_ACTION1;
    inputActionPrevious  = Input::inputs.INPUT_PREVIOUS_ACTION2;
    inputAction2Previous = Input::inputs.INPUT_PREVIOUS_ACTION3;
    inputAction3Previous = Input::inputs.INPUT_PREVIOUS_ACTION4;
    inputAction4Previous = Input::inputs.INPUT_PREVIOUS_RB;

    if (canMoveTimer > 0.0f || Global::finishStageTimer >= 0.0f || hitTimer > 0.0f || deadTimer > -1.0f)
    {
        inputJump    = false;
        inputAction  = false;
        inputAction2 = false;
        inputAction3 = false;
        inputAction4 = false;
        inputX    = 0;
        inputY    = 0;
        inputX2   = 0;
        inputY2   = 0;
        inputZoom = 0;

        inputJumpPrevious    = false;
        inputActionPrevious  = false;
        inputAction2Previous = false;
        inputAction3Previous = false;
        inputAction4Previous = false;
    }
}

std::vector<TexturedModel*>* PlayerSonic::getModels()
{
    return nullptr;
}

void PlayerSonic::loadVehicleInfo()
{
    #ifdef DEV_MODE
    printf("Loading PlayerSonic static models...\n");
    #endif
    
    ManiaSonicModel::loadStaticModels();
    ManiaMightyModel::loadStaticModels();
    SuperSonicModel::loadStaticModels();
    //ManiaKnucklesModel::loadStaticModels();
}

void PlayerSonic::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting PlayerSonic static models...\n");
    #endif
    
    ManiaSonicModel::deleteStaticModels();
    ManiaMightyModel::deleteStaticModels();
    SuperSonicModel::deleteStaticModels();
    //ManiaKnucklesModel::deleteStaticModels();
}

bool PlayerSonic::isVehicle() const
{
    return true;
}

void PlayerSonic::startGrabbing()
{
    isGrabbing = true;
    onGround = false;
    isBall = false;
    isJumping = false;
    isDropdashing = false;
    isHomingOnPoint = false;
    isStomping = false;
    isBouncing = false;
    justBounced = false;
    justHomingAttacked = false;
    velocityMovesPlayer = false;
}

void PlayerSonic::stopGrabbing()
{
    isGrabbing = false;
    velocityMovesPlayer = true;
}

void PlayerSonic::setIsDriving(bool newIsDriving)
{
    isDriving = newIsDriving;
    onGround = false;
    isBall = false;
    isJumping = false;
    isStomping = false;
    isDropdashing = false;
    velocityMovesPlayer = !newIsDriving;
}

float PlayerSonic::getHitboxHorizontal()
{
    return 6;
}

float PlayerSonic::getHitboxVertical()
{
    return 12;
}

Vector3f* PlayerSonic::getCameraDirection()
{
    return &camDir;
}

void PlayerSonic::setCameraDirection(Vector3f* newDirection)
{
    camDir.set(newDirection);
    camDirSmooth.set(newDirection);
}

void PlayerSonic::increaseCombo()
{
    combo++;
}

void PlayerSonic::setInWater(float newWaterHeight)
{
    inWater = true;
    waterHeight = newWaterHeight;
}

bool PlayerSonic::getIsRunningOnWater()
{
    return isRunningOnWater;
}

void PlayerSonic::refreshCamera()
{
    //Animating the camera

    if (deadTimer == -1.0f)
    {
        //only need this in case of FOV change
        //Master_makeProjectionMatrix();

        Vector3f camOffset(&camDirSmooth);

        if (Global::finishStageTimer < 1.0f)
        {
            camRadiusTarget -= camRadiusSpeed*inputZoom*dt;
            camRadiusTarget = fmaxf(fminf(camRadiusTarget, camRadiusMax), camRadiusMin);
            camRadius = Maths::approach(camRadius, camRadiusTarget, camRadiusApproach, dt);

            camRadius = camRadiusMax - (MasterRenderer::getVFOV() - 80.0f);

            camOffset.setLength(camRadius);
        }
        else
        {
            float tim = Global::finishStageTimer-1.0f;
            float newRad = Maths::approach(camRadius, 30.0f, 2.0f, tim);
            camOffset.setLength(newRad);
        }

        Vector3f camHeight(&relativeUpSmooth);
        camHeight.setLength(camHeightOffset);

        Vector3f eye(&position);
        eye = eye - camOffset;
        eye = eye + camHeight;

        Vector3f target(&position);
        target = target + camHeight;

        Vector3f up(&relativeUpSmooth);
        up.normalize();

        Vector3f camDelta = eye - target;
        camDelta.setLength(5); //this is what causes metal harbor to go through cam at beginning
        Vector3f camStart = target + camDelta;
        CollisionChecker::setCheckPlayer(false);
        CollisionChecker::setCheckCamera(true);
        if (CollisionChecker::checkCollision(camStart.x, camStart.y, camStart.z, eye.x, eye.y, eye.z))
        {
            Vector3f delta = eye - target;
            delta.setLength(3);
            Vector3f newPos(CollisionChecker::getCollidePosition());
            newPos = newPos - delta;
            eye.set(&newPos);
        }

        Global::gameCamera->setViewMatrixValues(&eye, &target, &up);
    }
    else
    {
        Camera* cam = Global::gameCamera;

        Vector3f target = getCenterPosition();

        Vector3f diff = cam->eye - target;

        Vector3f perpen = diff.cross(&Y_AXIS);
        Vector3f up = Maths::rotatePoint(&diff, &perpen, Maths::PI/2);

        Global::gameCamera->setViewMatrixValues(&Global::gameCamera->eye, &target, &up);
    }
}

//Do a small 'pop off' off the wall
void PlayerSonic::popOffWall()
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

void PlayerSonic::die()
{
    if (deadTimer == -1.0f && Global::finishStageTimer == -1.0f)
    {
        AudioPlayer::play(9, &position);
        deadTimer = 3.0f;
        if (Global::mainHudTimer != nullptr)
        {
            Global::mainHudTimer->freeze(true);
        }
    }
}

ShieldMagnet* PlayerSonic::getShieldMagnet()
{
    return myShieldMagnet;
}

void PlayerSonic::setShieldMagnet(ShieldMagnet* newMagnet)
{
    if (myShieldMagnet != nullptr)
    {
        Global::deleteEntity(myShieldMagnet);
        myShieldMagnet = nullptr;
    }
    if (myShieldGreen != nullptr)
    {
        Global::deleteEntity(myShieldGreen);
        myShieldGreen = nullptr;
    }
    myShieldMagnet = newMagnet;
}

ShieldGreen* PlayerSonic::getShieldGreen()
{
    return myShieldGreen;
}

void PlayerSonic::setShieldGreen(ShieldGreen* newGreen)
{
    if (myShieldMagnet != nullptr)
    {
        Global::deleteEntity(myShieldMagnet);
        myShieldMagnet = nullptr;
    }
    if (myShieldGreen != nullptr)
    {
        Global::deleteEntity(myShieldGreen);
        myShieldGreen = nullptr;
    }
    myShieldGreen = newGreen;
}


void PlayerSonic::setInvincibleTimer(float newTimer)
{
    invincibleTimer = newTimer;
    invincibleColor1.set(Maths::nextUniform(), Maths::nextUniform(), Maths::nextUniform());
    invincibleColor2.set(Maths::nextUniform(), Maths::nextUniform(), Maths::nextUniform());
}

void PlayerSonic::setSpeedshoesTimer(float newTimer)
{
    speedShoesTimer = newTimer;
}

bool PlayerSonic::isDying()
{
    return (deadTimer > -1.0f);
}

bool PlayerSonic::canDealDamage()
{
    //return (hitTimer == 0.0f && hitFlashingTimer == 0.0f);
    return (isBouncing || isBall || isJumping || isSpindashing || isStomping || isDropdashing || (invincibleTimer > 0.0f));
}

bool PlayerSonic::canBreakObjects()
{
    return (isBouncing || isBall || isSpindashing || isDropdashing) && (vel.lengthSquared() > breakObjectsSpeed*breakObjectsSpeed);
}

void PlayerSonic::hitSpring(Vector3f* direction, float power, float lockInputTime, bool resetsCamera)
{
    vel = direction->scaleCopy(power);
    onGround = false;
    isSkidding = false;
    isLightdashing = false;
    isSpindashing = false;
    isGrinding = false;
    isStomping = false;
    isBouncing = false;
    isHomingOnPoint = false;
    justBounced = false;
    hoverTimer = 0.0f;
    canMoveTimer = lockInputTime;
    hitSpringTimer = lockInputTime;

    if (resetsCamera)
    {
        camDir.set(direction);
        camDir.normalize();
    }
}

void PlayerSonic::hitSpringYellow(const Vector3f* direction, float power, float lockInputTime)
{
    float speedInDir = Maths::projectAlongLine(&vel, direction).length();
    float newLineSpeed = fmaxf(speedInDir, power);
    vel = Maths::projectOntoPlane(&vel, direction);
    vel = vel + direction->scaleCopy(newLineSpeed);

    onGround = false;
    isSkidding = false;
    isBall = false;
    isJumping = true;
    isLightdashing = false;
    isSpindashing = false;
    isGrinding = false;
    isStomping = false;
    isBouncing = false;
    isHomingOnPoint = false;
    justBounced = false;
    justHomingAttacked = false;
    hoverTimer = 0.0f;
    canMoveTimer = lockInputTime;
    hitSpringTimer = lockInputTime;
}

void PlayerSonic::hitSpringTriple(Vector3f* direction, float power, float lockInputTime)
{
    Vector3f dir(direction);
    dir.y = 0;
    dir.normalize();
    dir.scale(-1.0f);
    dir.setLength(5.0f);

    vel.set(&dir);
    vel.y = power;
    camDir.set(&dir);
    camDir.normalize();

    onGround = false;
    isBall = false;
    isJumping = true;
    isSkidding = false;
    isLightdashing = false;
    isSpindashing = false;
    isGrinding = false;
    isStomping = false;
    isBouncing = false;
    isHomingOnPoint = false;
    justBounced = false;
    justHomingAttacked = false;
    hoverTimer = 0.0f;
    canMoveTimer = lockInputTime;
    hitSpringTimer = lockInputTime;
}

void PlayerSonic::hitSpeedRamp(Vector3f* direction, float speed, float lockInputTime)
{
    vel.set(direction);
    vel.setLength(speed);
    onGround = false;
    isBall = false;
    isJumping = false;
    isSkidding = false;
    isLightdashing = false;
    isSpindashing = false;
    isGrinding = false;
    isStomping = false;
    isBouncing = false;
    isHomingOnPoint = false;
    justBounced = false;
    hoverTimer = 0.0f;
    canMoveTimer = lockInputTime;

    camDir.set(direction);
    camDir.normalize();

    position.y += 4;
}

void PlayerSonic::hitDashpad()
{
    isBall = false;
    isSkidding = false;
    isSpindashing = false;
}

Vector3f PlayerSonic::getCenterPosition()
{
    if (onGround)
    {
        return position + relativeUp.scaleCopy(playerModel->getDisplayBallOffset());
    }
    else
    {
        return position;
    }
}
