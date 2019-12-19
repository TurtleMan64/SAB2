#include <glad/glad.h>

#include "entity.h"
#include "../toolbox/vector.h"
#include "windgust.h"
#include "../renderEngine/renderEngine.h"
#include "../engineTester/main.h"
#include "../entities/controllableplayer.h"
#include "../toolbox/maths.h"
#include "../particles/particle.h"
#include "../particles/particleresources.h"
#include "../particles/particlemaster.h"

#include <list>
#include <iostream>
#include <algorithm>
#include <cmath>

extern float dt;

WindGust::WindGust(float x, float y, float z, float rotY, float rotZ, float radius, float height, float power)
{
    //setup transform
    position = Vector3f(x, y, z);
    this->rotX = 0;
    this->rotY = rotY;
    this->rotZ = rotZ;
    
    //store important variables from arguments
    this->radius = radius;
    this->height = height;
    this->power = power;

    radiusSquared = radius * radius;

    scale = 1;
    visible = false;

    updateTransformationMatrix();
}

void WindGust::step()
{
    if (checkForPlayerCollision())
    {
        Vector3f playerVelocity = Global::gameMainPlayer->vel;
        Global::gameMainPlayer->vel.set(playerVelocity.x, playerVelocity.y + 10, playerVelocity.z);
    }

    Vector3f pos(
		getX() + radius * (Maths::random() - 0.5f),
		getY(),
		getZ() + radius * (Maths::random() - 0.5f));

	Vector3f vel(0, 5.5f, 0);
    		
    ParticleMaster::createParticle(ParticleResources::textureDust, &pos, &vel, 0, (int)(height/2.0f), 0, 10 * Maths::random() + 3, 0, false, true, 1.0f, false);
}

bool WindGust::checkForPlayerCollision()
{
    Vector3f playerPosition = Global::gameMainPlayer->getPosition();
    Vector3f distanceVectorFromCenters = Vector3f(playerPosition.x - position.x, playerPosition.y - position.y, playerPosition.z - position.z);
    float distanceFromCentersSquared = distanceVectorFromCenters.x * distanceVectorFromCenters.x + distanceVectorFromCenters.z * distanceVectorFromCenters.z;

    if (radiusSquared >= distanceFromCentersSquared && playerPosition.y >= position.y && playerPosition.y <= position.y + height)
    {
        return true;
    }
    return false;
}
