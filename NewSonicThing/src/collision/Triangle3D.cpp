#include <math.h>
#include "triangle3d.h"
#include "../toolbox/vector.h"

//Loads in all data from 91 bytes
Triangle3D::Triangle3D(char* buf)
{
    p1X      = *((float*)(&buf[ 0]));
    p1Y      = *((float*)(&buf[ 4]));
    p1Z      = *((float*)(&buf[ 8]));
    p2X      = *((float*)(&buf[12]));
    p2Y      = *((float*)(&buf[16]));
    p2Z      = *((float*)(&buf[20]));
    p3X      = *((float*)(&buf[24]));
    p3Y      = *((float*)(&buf[28]));
    p3Z      = *((float*)(&buf[32]));
    normal.x = *((float*)(&buf[36]));
    normal.y = *((float*)(&buf[40]));
    normal.z = *((float*)(&buf[44]));
    A        = *((float*)(&buf[48]));
    B        = *((float*)(&buf[52]));
    C        = *((float*)(&buf[56]));
    D        = *((float*)(&buf[60]));
    maxX     = *((float*)(&buf[64]));
    minX     = *((float*)(&buf[68]));
    maxY     = *((float*)(&buf[72]));
    minY     = *((float*)(&buf[76]));
    maxZ     = *((float*)(&buf[80]));
    minZ     = *((float*)(&buf[84]));
    type     = buf[88];
    sound    = buf[89];
    particle = buf[90];
}

Triangle3D::Triangle3D(Vector3f* newP1, Vector3f* newP2, Vector3f* newP3, char type, char sound, char particle)
{
    this->p1X = newP1->x;
    this->p1Y = newP1->y;
    this->p1Z = newP1->z;

    this->p2X = newP2->x;
    this->p2Y = newP2->y;
    this->p2Z = newP2->z;

    this->p3X = newP3->x;
    this->p3Y = newP3->y;
    this->p3Z = newP3->z;

    this->type = type;
    this->sound = sound;
    this->particle = particle;

    generateValues();
}

void Triangle3D::generateValues()
{
    Vector3f vec1(p1X - p3X, p1Y - p3Y, p1Z - p3Z);
    Vector3f vec2(p2X - p3X, p2Y - p3Y, p2Z - p3Z);

    Vector3f cross = vec1.cross(&vec2);

    float newD = cross.x*p3X + cross.y*p3Y + cross.z*p3Z;

    A = cross.x;
    B = cross.y;
    C = cross.z;
    D = -newD;

    float mag = sqrtf(A*A + B*B + C*C);

    if (mag != 0)
    {
        normal.x = A / mag;
        normal.y = B / mag;
        normal.z = C / mag;
    }
    else
    {
        normal.x = 0;
        normal.y = 1;
        normal.z = 0;
    }

    maxX = fmaxf(p1X, fmaxf(p2X, p3X));
    minX = fminf(p1X, fminf(p2X, p3X));
    maxY = fmaxf(p1Y, fmaxf(p2Y, p3Y));
    minY = fminf(p1Y, fminf(p2Y, p3Y));
    maxZ = fmaxf(p1Z, fmaxf(p2Z, p3Z));
    minZ = fminf(p1Z, fminf(p2Z, p3Z));
}

bool Triangle3D::isWall()
{
    return (type == 2);
}

bool Triangle3D::isDiggable()
{
    return (type == 1);
}

bool Triangle3D::isDeath()
{
    return (type == 3);
}
