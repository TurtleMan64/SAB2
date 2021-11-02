#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include <math.h>
#include "../toolbox/vector.hpp"


class Triangle3D
{
public:
    float p1X;
    float p1Y;
    float p1Z;

    float p2X;
    float p2Y;
    float p2Z;

    float p3X;
    float p3Y;
    float p3Z;

    Vector3f normal;

    float A;
    float B;
    float C;
    float D;

    //first 5 ls bits is the type. 0 = normal, 1 = diggable, 2 = wall, 4 = death, 8 = bounce, 16 = camera passthrough
    //3 ms bits are strength of bounce.
    // example:
    // bit 7 6 5  4 3 2 1 0
    //     1 0 1  0 1 0 0 0
    // this is bounce strength 5. bounce strength is then multiplied by 100 for final bounce speed.
    char type;

    char sound;

    char particle;

    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;

    //Loads in all data from 91 bytes
    Triangle3D(char* buf);

    Triangle3D(Vector3f* newP1, Vector3f* newP2, Vector3f* newP3, char type, char sound, char particle);

    void generateValues();

    bool isWall();

    bool isDiggable();

    bool isDeath();

    bool isNoCam();

    float bounceStrength();
};

#endif