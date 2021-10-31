#include <glad/glad.h>

#include "../entity.h"
#include "../../models/models.h"
#include "../../toolbox/vector.h"
#include "ecseagull.h"
#include "../../renderEngine/renderEngine.h"
#include "../../objLoader/objLoader.h"
#include "../../engineTester/main.h"
#include "../../toolbox/maths.h"
#include "../camera.h"
#include "../../audio/audioplayer.h"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> EC_Seagull::models;

EC_Seagull::EC_Seagull()
{
	
}

EC_Seagull::EC_Seagull(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	modelX = x;
	modelY = y;
	modelZ = z;

	flyAngle = 360*Maths::random();
	flyAngleDelta = 20 + 8*Maths::random();

	flyRadiusBase = 70 + 20*Maths::random();
	flyRadiusAmplitude = 20 + 5*Maths::random();
	flyRadiusDelta = 60 + 20*Maths::random();
	flyRadiusProgression = Maths::random();

	flyHeightBase = y;
	flyHeightAmplitude = 15 + 10*Maths::random();
	flyHeightDelta = 60 + 20*Maths::random();
	flyHeightProgression = Maths::random();

	soundTimer = 5.0f*Maths::random();

	updateTransformationMatrix();
}

extern float dt;

void EC_Seagull::step()
{
	prevX = modelX;
	prevY = modelY;
	prevZ = modelZ;

	flyAngle += flyAngleDelta*dt;
	flyRadiusProgression += flyRadiusDelta*dt;
	flyHeightProgression += flyHeightDelta*dt;

	float flyRadius = flyRadiusAmplitude*sinf(Maths::toRadians(flyRadiusProgression)) + flyRadiusBase;

	modelX = position.x + flyRadius*cosf(Maths::toRadians(flyAngle));
	modelZ = position.z - flyRadius*sinf(Maths::toRadians(flyAngle));

	modelY = flyHeightAmplitude*sinf(Maths::toRadians(flyHeightProgression)) + flyHeightBase;

	float xDiff = modelX - prevX;
	float yDiff = modelY - prevY;
	float zDiff = modelZ - prevZ;

	rotY = Maths::toDegrees(atan2f(-zDiff, xDiff));
	rotZ = Maths::toDegrees(atan2f(yDiff, sqrtf(xDiff*xDiff+zDiff*zDiff)));

    Vector3f center(&position);
    position.set(modelX, modelY, modelZ);
	updateTransformationMatrix();
    position = center;

	float dx = modelX - Global::gameCamera->eye.x;
	float dy = modelY - Global::gameCamera->eye.y;
	float dz = modelZ - Global::gameCamera->eye.z;

    soundTimer -= dt;

	if (dx*dx + dy*dy + dz*dz < 400.0f*400.0f &&
		soundTimer <= 0.0f)
	{
        Vector3f soundPos(modelX, modelY, modelZ);
		AudioPlayer::play(20, &soundPos);
	}

    if (soundTimer <= 0.0f)
	{
		soundTimer = 5.0f*Maths::nextUniform() + 1.0f;
	}
}

std::list<TexturedModel*>* EC_Seagull::getModels()
{
	return &EC_Seagull::models;
}

void EC_Seagull::loadStaticModels()
{
	if (EC_Seagull::models.size() > 0)
	{
		return;
	}

	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading EC_Seagull static models...\n");
	#endif

	ObjLoader::loadModel(&EC_Seagull::models, "res/Models/Levels/EmeraldCoast/Seagull/", "Seagull");
}

void EC_Seagull::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting EC_Seagull static models...\n");
	#endif

	Entity::deleteModels(&EC_Seagull::models);
}
