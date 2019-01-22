#include <list>
#include <fstream>

#include "../toolbox/vector.h"
#include "rail.h"
#include "../toolbox/maths.h"
#include "../engineTester/main.h"
#include "car.h"
#include "../toolbox/input.h"
#include "../toolbox/getline.h"
#include "../toolbox/split.h"

#include "ring.h"

#include <chrono>
#include <thread>

RailSegment::RailSegment(Vector3f* begin, Vector3f* end, std::vector<RailSegment>* rails, int index)
{
	pointBegin.set(begin);
	pointEnd.set(end);
	pointsDiff = pointEnd - pointBegin;
	length = pointsDiff.length();
	if (length < 1.0f)
	{
		std::fprintf(stdout, "Warning: Very small rail segment.\n");
	}

	Ring* ring1 = new Ring(pointBegin.x, pointBegin.y, pointBegin.z); INCR_NEW
	Main_addEntity(ring1);
	Ring* ring2 = new Ring(pointEnd.x, pointEnd.y, pointEnd.z); INCR_NEW
	Main_addEntity(ring2);

	playerIsOn = false;
	playerProgress = 0;
	this->rails = rails;
	this->index = index;


	speedToAdd = -pointsDiff.y*(5.0f); //speed constant
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

	std::ifstream file(railPath);
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file '%s'\n", railPath);
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

			if (splitLength == 3)
			{
				Vector3f p(std::stof(lineSplit[0]), std::stof(lineSplit[1]), std::stof(lineSplit[2]));
				pointsList.push_back(p);
			}
			free(lineSplit);
		}
		file.close();
	}

	int n = ((int)pointsList.size())-1;
	for (int i = 0; i < n; i++)
	{
		rails.push_back(RailSegment(&pointsList[i], &pointsList[i+1], &rails, i));
	}

	setVisible(false);
	currentSegment = false;
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
			//Check if sonic should get on a rail
			for (RailSegment r : rails)
			{
				float dist = r.distanceToPoint(Global::gameMainVehicle->getPosition());
				if (dist < 5)
				{
					Vector3f landingPoint = r.closestPoint(Global::gameMainVehicle->getPosition());
					Global::gameMainVehicle->setPosition(&landingPoint);
					currentSegment = &rails[r.index];
					currentSegmentIndex = r.index;
					Global::gameMainVehicle->startGrinding();
					Vector3f newVel(Global::gameMainVehicle->getVelocity());
					newVel = Maths::projectAlongLine(&newVel, &r.pointsDiff);
					Global::gameMainVehicle->setVelocity(newVel.x, newVel.y, newVel.z);
					//sound effect start
					r.playerProgress = r.calcProgress(&landingPoint);
					r.playerIsOn = true;
					//playerSpeed = newVel.length(); //TODO calculate initial speed
					playerSpeed = 0;
					timer = 1.0f;
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
		//move sonic along to the next rail, or maybe he is jumping off?

		//change the characters speed based on velToAdd and if holding the B button
		//TODO
		//Vector3f* playerVel = Global::gameMainVehicle->getVelocity();
		//playerVel->x += currentSegment->velToAdd.x*dt;
		//playerVel->y += currentSegment->velToAdd.y*dt;
		//playerVel->z += currentSegment->velToAdd.z*dt;
		//Global::gameMainVehicle->setVelocity(playerVel->x, playerVel->y, playerVel->z);
		playerSpeed += currentSegment->speedToAdd*dt;


		float newProgress = currentSegment->playerProgress + (playerSpeed/currentSegment->length)*dt;

		if (newProgress < 0) //go to previous rail segment
		{
			if (currentSegmentIndex > 0)
			{
				float totalDistance = (currentSegment->playerProgress - newProgress)/currentSegment->length;
				float distanceAlreadyUsed = currentSegment->playerProgress/currentSegment->length;
				float distanceRemaining = totalDistance - distanceAlreadyUsed;

				//clean up this segment
				currentSegment->playerIsOn = false;
				currentSegment->playerProgress = 0;

				currentSegmentIndex--;
				currentSegment = &rails[currentSegmentIndex];
				currentSegment->playerIsOn = true;

				float newSegmentProgress = 1.0f - distanceRemaining/currentSegment->length;
				currentSegment->playerProgress = newSegmentProgress;
				Vector3f progessVec = currentSegment->pointsDiff.scaleCopy(newSegmentProgress);
				Vector3f nP = currentSegment->pointBegin + progessVec;
				Global::gameMainVehicle->setPosition(&nP);
				Global::gameMainVehicle->updateTransformationMatrix();
			}
			else
			{
				Global::gameMainVehicle->stopGrinding();
				currentSegment = nullptr;
				currentSegmentIndex = -1;
			}
		}
		else if (newProgress < 1) //stay on current segment
		{
			//Global::gameMainVehicle->setPosition(&nextPosition);
			currentSegment->playerProgress = newProgress;
			Vector3f progessVec = currentSegment->pointsDiff.scaleCopy(newProgress);
			Vector3f nP = currentSegment->pointBegin + progessVec;
			Global::gameMainVehicle->setPosition(&nP);
			Global::gameMainVehicle->updateTransformationMatrix();

			Vector3f newVel(currentSegment->pointsDiff);
			newVel.normalize();
			newVel.scale(playerSpeed);
			Global::gameMainVehicle->setVelocity(newVel.x, newVel.y, newVel.z);
		}
		else //go to next rail segment
		{
			if (currentSegmentIndex < rails.size()-1)
			{
				float totalDistance = (newProgress - currentSegment->playerProgress)/currentSegment->length;
				float distanceAlreadyUsed = (1.0f - currentSegment->playerProgress)/currentSegment->length;
				float distanceRemaining = totalDistance - distanceAlreadyUsed;

				//clean up this segment
				currentSegment->playerIsOn = false;
				currentSegment->playerProgress = 0;

				currentSegmentIndex++;
				currentSegment = &rails[currentSegmentIndex];
				currentSegment->playerIsOn = true;

				float newSegmentProgress = distanceRemaining/currentSegment->length;
				currentSegment->playerProgress = newSegmentProgress;
				Vector3f progessVec = currentSegment->pointsDiff.scaleCopy(newSegmentProgress);
				Vector3f nP = currentSegment->pointBegin + progessVec;
				Global::gameMainVehicle->setPosition(&nP);
				Global::gameMainVehicle->updateTransformationMatrix();
			}
			else
			{
				Global::gameMainVehicle->stopGrinding();
				currentSegment = nullptr;
				currentSegmentIndex = -1;
			}
		}

		if (Input::inputs.INPUT_ACTION1 && Input::inputs.INPUT_PREVIOUS_ACTION1)
		{
			Global::gameMainVehicle->stopGrinding();
			Vector3f* playerVel = Global::gameMainVehicle->getVelocity();
			Global::gameMainVehicle->setVelocity(playerVel->x, playerVel->y+200, playerVel->z);
			//Global::gameMainVehicle->Jump();
			currentSegment = nullptr;
			currentSegmentIndex = -1;
		}
	}
}
