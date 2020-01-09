#include "point.h"
#include "../toolbox/vector.h"
#include "../engineTester/main.h"

Point::Point()
{
    this->id = -1;
}

Point::Point(float x, float y, float z, int id)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    this->id = id;
    this->rotX = 0;
    this->rotY = 0;
    this->rotZ = 0;
    this->scale = 1;
    this->visible = false;
}

void Point::step()
{
    Global::deleteEntity(this);
}

int Point::getID()
{
    return id;
}

const bool Point::isPoint()
{
    return true;
}
