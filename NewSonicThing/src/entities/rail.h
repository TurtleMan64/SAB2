#ifndef RAIL_H
#define RAIL_H

#include <vector>
#include "entity.h"
#include "../toolbox/vector.h"

class RailSegment
{
public:
    int index; //Our index in the rails vector
    std::vector<RailSegment>* rails = nullptr;
    Vector3f pointBegin;
    Vector3f pointEnd;
    Vector3f pointsDiff;
    Vector3f normalBegin;
    Vector3f normalEnd;
    float speedToAdd; //speed to add to players vel. 
    const float steepnessFactor = 90.0f;
    float length;
    bool playerIsOn;
    float playerProgress; //players progress through the rail. 0 = at the begin point, 0.5 = halfway inbetween, 1 = at the end

    RailSegment(Vector3f* begin, Vector3f* end, Vector3f* beginNormal, Vector3f* endNormal, std::vector<RailSegment>* rails, int index);

    //returns distance to closest point on line segment
    float distanceToPoint(Vector3f* point);

    //returns closest point on this line segment to the given point
    Vector3f closestPoint(Vector3f* point);

    //given a point, calculate how far along the given point is along the segment, 0 being near the begin, 1 being at the end
    float calcProgress(Vector3f* point);
};

class Rail : public Entity
{
private:
    std::vector<RailSegment> rails;
    RailSegment* currentSegment = nullptr;
    int currentSegmentIndex;
    float playerSpeed; //positive speed means going to end point from start point
    const float crouchPush = 40.0f; //how much the player croushing accelerates them
    const float railDrag = -0.13f;
    const float timeBeforeLandAgain = 0.08f;
    float timer;
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;

public:
    Rail(const char* railPath);

    void step();
};
#endif
