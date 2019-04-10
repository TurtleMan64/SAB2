#include <glad/glad.h>

#include "entity.h"
#include "../toolbox/vector.h"
#include "windgust.h"
#include "../renderEngine/renderEngine.h"
#include "../engineTester/main.h"
#include "../entities/car.h"
#include "../toolbox/maths.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

extern float dt;

WindGust::WindGust() {}

WindGust::WindGust(float x, float y, float z, float rotY, float rotZ, float radius, float height, float power)
{
    //setup transform
    position = Vector3f(x, y, z);
    this->rotY = rotY;
    this->rotZ = rotZ;
    
    //store important variables from arguments
    this->radius = radius;
    this->height = height;
    this->power = power;

    scale = 1;
    visible = false;
}

void step()
{

}

