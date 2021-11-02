#include <list>
#include <fstream>
#include <cstring>

#include "rail.hpp"
#include "controllableplayer.hpp"
#include "../toolbox/maths.hpp"
#include "../toolbox/vector.hpp"
#include "../engineTester/main.hpp"
#include "../toolbox/input.hpp"
#include "../toolbox/getline.hpp"
#include "../toolbox/split.hpp"

#include "ring.hpp"

RailSegment::RailSegment(Vector3f* begin, Vector3f* end, Vector3f* beginNormal, Vector3f* endNormal, std::vector<RailSegment>* rails, int index)
{
    pointBegin.set(begin);
    pointEnd.set(end);
    pointsDiff = pointEnd - pointBegin;
    normalBegin.set(beginNormal);
    normalEnd.set(endNormal);
    length = pointsDiff.length();
    if (length < 1.0f)
    {
        std::fprintf(stdout, "Warning: Very small rail segment.\n");
    }

    //Ring* ring1 = new Ring(pointBegin.x, pointBegin.y, pointBegin.z); INCR_NEW
    //Global::addEntity(ring1);
    //Ring* ring2 = new Ring(pointEnd.x, pointEnd.y, pointEnd.z); INCR_NEW
    //Global::addEntity(ring2);

    playerIsOn = false;
    playerProgress = 0;
    this->rails = rails;
    this->index = index;

    Vector3f toAdd(&pointsDiff);
    toAdd.normalize();
    speedToAdd = -toAdd.y*(RailSegment::steepnessFactor); //speed constant
}

//I want to optimize this as much as I can
float RailSegment::distanceToPoint(Vector3f* q)
{
    Vector3f u = pointEnd - pointBegin;
    Vector3f v(q);
    v = v - pointBegin;

    float dot = u.dot(&v);
    float euc2 = u.lengthSquared();
    float frac = dot/euc2;
    if (frac >= 0.0f && frac <= 1.0f)
    {
        Vector3f p = pointBegin + u.scaleCopy(frac);
        Vector3f diff = p - q;
        return diff.length();
    }
    else if (frac < 0.0f)
    {
        Vector3f diff = pointBegin - q;
        return diff.length();
    }

    Vector3f diff = pointEnd - q;
    return diff.length();
}

Vector3f RailSegment::closestPoint(Vector3f* q)
{
    Vector3f u = pointEnd - pointBegin;
    Vector3f v(q);
    v = v - pointBegin;

    float dot = u.dot(&v);
    float euc2 = u.lengthSquared();
    float frac = dot/euc2;
    if (frac >= 0.0f && frac <= 1.0f)
    {
        return pointBegin + u.scaleCopy(frac);
    }
    else if (frac < 0.0f)
    {
        return pointBegin;
    }

    return pointEnd;
}

float RailSegment::calcProgress(Vector3f* point)
{
    Vector3f u = pointEnd - pointBegin;
    Vector3f v(point);
    v = v - pointBegin;

    float dot = u.dot(&v);
    float euc2 = u.lengthSquared();
    return dot/euc2;
}

Rail::Rail(const char* railPath)
{
    std::vector<Vector3f> pointsList;
    std::vector<Vector3f> normalList;

    std::ifstream file(Global::pathToEXE+railPath);
    if (!file.is_open())
    {
        std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE+railPath).c_str());
        file.close();
    }
    else
    {
        std::string line;

        while (!file.eof())
        {
            getlineSafe(file, line);

            char lineBuf[512];
            memcpy(lineBuf, line.c_str(), line.size()+1);

            int splitLength = 0;
            char** lineSplit = split(lineBuf, ' ', &splitLength);

            if (splitLength == 6)
            {
                Vector3f p(std::stof(lineSplit[0]), std::stof(lineSplit[1]), std::stof(lineSplit[2]));
                pointsList.push_back(p);

                Vector3f n(std::stof(lineSplit[3]), std::stof(lineSplit[4]), std::stof(lineSplit[5]));
                normalList.push_back(n);

                maxX = fmaxf(maxX, p.x);
                maxY = fmaxf(maxY, p.y);
                maxZ = fmaxf(maxZ, p.z);

                minX = fminf(minX, p.x);
                minY = fminf(minY, p.y);
                minZ = fminf(minZ, p.z);
            }
            free(lineSplit);
        }
        file.close();
    }

    int n = ((int)pointsList.size())-1;
    for (int i = 0; i < n; i++)
    {
        rails.push_back(RailSegment(&pointsList[i], &pointsList[i+1], &normalList[i], &normalList[i+1], &rails, i));
    }

    setVisible(false);
    currentSegment = nullptr;
    currentSegmentIndex = -1;
    timer = 0;
}

void Rail::step()
{
    extern float dt;

    if (currentSegment == nullptr)
    {
        if (timer < 0.0f)
        {
            const float pad = 30.0f;
            if (Global::gameMainPlayer->getX() >= minX-pad && Global::gameMainPlayer->getX() <= maxX+pad &&
                Global::gameMainPlayer->getY() >= minY-pad && Global::gameMainPlayer->getY() <= maxY+pad &&
                Global::gameMainPlayer->getZ() >= minZ-pad && Global::gameMainPlayer->getZ() <= maxZ+pad)
            {
                //Check if sonic should get on a rail
                for (RailSegment r : rails)
                {
                    float dist = r.distanceToPoint(Global::gameMainPlayer->getPosition());
                    if (dist < 5) //radius of the rail
                    {
                        Vector3f landingPoint = r.closestPoint(Global::gameMainPlayer->getPosition());
                        Global::gameMainPlayer->setPosition(&landingPoint);
                        Global::gameMainPlayer->increasePosition(r.normalBegin.x*1.0f, r.normalBegin.y*1.0f, r.normalBegin.z*1.0f);
                        currentSegment = &rails[r.index];
                        currentSegmentIndex = r.index;
                        Global::gameMainPlayer->startGrinding();
                        Vector3f newVel(&Global::gameMainPlayer->vel);
                        float originalSpeed = newVel.length();
                        newVel = Maths::projectAlongLine(&newVel, &r.pointsDiff);
                        float newSpeed = newVel.length();
                        float averageSpeed = (originalSpeed+newSpeed)*0.5f;
                        newVel.setLength(averageSpeed);
                        Global::gameMainPlayer->vel = Vector3f(newVel.x, newVel.y, newVel.z);

                        //sound effect start

                        currentSegment->playerProgress = r.calcProgress(&landingPoint);
                        currentSegment->playerIsOn = true;

                        float dot = currentSegment->pointsDiff.dot(&newVel);
                        playerSpeed = Maths::sign(dot)*newVel.length();

                        timer = timeBeforeLandAgain; //seconds before we can land on the rail again

                        //printf("landing on rail #%d\n", currentSegmentIndex);
                        break;
                    }
                }
            }
        }
        else
        {
            timer -= dt;
        }
    }
    else
    {
        //printf("  start 1\n");
        playerSpeed += currentSegment->speedToAdd*dt;
        if (Input::inputs.INPUT_ACTION2)
        {
            playerSpeed += Maths::sign(playerSpeed)*crouchPush*dt;
        }

        playerSpeed = Maths::applyDrag(playerSpeed, railDrag, dt);

        if (Input::inputs.INPUT_ACTION1 && !Input::inputs.INPUT_PREVIOUS_ACTION1)
        {
            //printf("   if 1\n");
            Global::gameMainPlayer->stopGrinding();
            Global::gameMainPlayer->jump();
            currentSegment = nullptr;
            currentSegmentIndex = -1;
        }
        else if (Input::inputs.INPUT_LB && !Input::inputs.INPUT_PREVIOUS_LB)
        {
            //printf("   if 2\n");
            Global::gameMainPlayer->stopGrinding();

            Vector3f forward = Global::gameMainPlayer->vel;
            forward.normalize();
            Vector3f up = currentSegment->normalBegin;
            Vector3f side = up.cross(&forward);
            side.setLength(200.0f);
            Global::gameMainPlayer->vel = Global::gameMainPlayer->vel + side;

            Global::gameMainPlayer->canMoveTimer = 0.3f;
            currentSegment = nullptr;
            currentSegmentIndex = -1;
        }
        else if (Input::inputs.INPUT_RB && !Input::inputs.INPUT_PREVIOUS_RB)
        {
            //printf("   if 3\n");
            Global::gameMainPlayer->stopGrinding();

            Vector3f forward = Global::gameMainPlayer->vel;
            forward.normalize();
            Vector3f up = currentSegment->normalBegin;
            Vector3f side = up.cross(&forward);
            side.setLength(200.0f);
            side.scale(-1);
            Global::gameMainPlayer->vel = Global::gameMainPlayer->vel + side;

            Global::gameMainPlayer->canMoveTimer = 0.3f;
            currentSegment = nullptr;
            currentSegmentIndex = -1;
        }
        else
        {
            //printf("   if 4\n");
            float distanceRemaining = playerSpeed*dt;
            //printf("    distanceRemaining = %f\n", distanceRemaining);
            while (distanceRemaining != 0.0f)
            {
                float newProgress = currentSegment->playerProgress + (distanceRemaining/currentSegment->length);
                //printf("        newProgress = %f\n", newProgress);
                Vector3f newVel(currentSegment->pointsDiff);
                newVel.normalize();
                newVel.scale(playerSpeed);
                Global::gameMainPlayer->vel = Vector3f(newVel.x, newVel.y, newVel.z);

                //interpolate the normal vector between the start and end points
                Vector3f interpolatedNormal = Maths::interpolateVector(&currentSegment->normalBegin, &currentSegment->normalEnd, newProgress);
                Global::gameMainPlayer->relativeUp = interpolatedNormal;

                if (newProgress < 0) //go to previous rail segment
                {
                    float distanceUsed = currentSegment->playerProgress*currentSegment->length;
                    distanceRemaining += distanceUsed; //this might be minus
                    //printf("            distanceUsed = %f\n", distanceUsed);

                    if (currentSegmentIndex > 0)
                    {
                        //clean up this segment
                        currentSegment->playerIsOn = false;
                        currentSegment->playerProgress = 0;

                        currentSegmentIndex--;
                        currentSegment = &rails[currentSegmentIndex];
                        currentSegment->playerIsOn = true;
                        currentSegment->playerProgress = 1.0f;
                    }
                    else
                    {
                        Vector3f progressVec = currentSegment->pointsDiff.scaleCopy(distanceRemaining);
                        progressVec.setLength(Maths::sign(distanceRemaining)*distanceRemaining);
                        Vector3f nP = currentSegment->pointBegin + progressVec;
                        Global::gameMainPlayer->setPosition(&nP);
                        Global::gameMainPlayer->increasePosition(currentSegment->normalBegin.x*1.0f, 
                                                                 currentSegment->normalBegin.y*1.0f, 
                                                                 currentSegment->normalBegin.z*1.0f);
                        Global::gameMainPlayer->updateTransformationMatrix();

                        Global::gameMainPlayer->stopGrinding();
                        currentSegment = nullptr;
                        currentSegmentIndex = -1;
                        distanceRemaining = 0.0f;
                    }
                }
                else if (newProgress < 1) //stay on current segment
                {
                    //printf("            staying on current segment\n");
                    currentSegment->playerProgress = newProgress;
                    Vector3f progressVec = currentSegment->pointsDiff.scaleCopy(newProgress);
                    Vector3f nP = currentSegment->pointBegin + progressVec;
                    Global::gameMainPlayer->setPosition(&nP);
                    Global::gameMainPlayer->increasePosition(currentSegment->normalBegin.x*1.0f, 
                                                             currentSegment->normalBegin.y*1.0f, 
                                                             currentSegment->normalBegin.z*1.0f);
                    Global::gameMainPlayer->updateTransformationMatrix();

                    distanceRemaining = 0.0f;
                }
                else //go to next rail segment
                {
                    float distanceUsed = (1.0f - currentSegment->playerProgress)*currentSegment->length;
                    distanceRemaining -= distanceUsed; //this might be plus
                    //printf("            distanceUsed = %f\n", distanceUsed);

                    if (currentSegmentIndex < ((int)rails.size())-1)
                    {
                        //clean up this segment
                        currentSegment->playerIsOn = false;
                        currentSegment->playerProgress = 0;

                        currentSegmentIndex++;
                        currentSegment = &rails[currentSegmentIndex];
                        currentSegment->playerIsOn = true;
                        currentSegment->playerProgress = 0.0f;
                    }
                    else
                    {
                        Vector3f progressVec = currentSegment->pointsDiff.scaleCopy(distanceRemaining);
                        progressVec.setLength(Maths::sign(distanceRemaining)*distanceRemaining);
                        Vector3f nP = currentSegment->pointEnd + progressVec;
                        Global::gameMainPlayer->setPosition(&nP);
                        Global::gameMainPlayer->increasePosition(currentSegment->normalBegin.x*1.0f, 
                                                                 currentSegment->normalBegin.y*1.0f, 
                                                                 currentSegment->normalBegin.z*1.0f);
                        Global::gameMainPlayer->updateTransformationMatrix();

                        Global::gameMainPlayer->stopGrinding();
                        currentSegment = nullptr;
                        currentSegmentIndex = -1;
                        distanceRemaining = 0.0f;
                    }
                }
            }
        }
    }

    if (currentSegment != nullptr)
    {
        //printf("on rail #%d\n", currentSegmentIndex);
        Global::gameMainPlayer->animate();
        Global::gameMainPlayer->refreshCamera();
    }
}
