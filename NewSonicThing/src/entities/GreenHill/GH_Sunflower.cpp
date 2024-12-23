#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghsunflower.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::vector<TexturedModel*> GH_Sunflower::modelsBase;
std::vector<TexturedModel*> GH_Sunflower::modelsTop;
std::vector<TexturedModel*> GH_Sunflower::modelsTop2;

GH_Sunflower::GH_Sunflower()
{
    
}

GH_Sunflower::GH_Sunflower(float x, float y, float z, float rotY, float scale, std::list<Entity*>* entityListToAdd)
{
    position.x = x;
    position.y = y;
    position.z = z;
    rotX = 0;
    this->rotY = rotY;
    rotZ = 0;
    this->scale = scale;
    visible = true;
    updateTransformationMatrix();

    petals = new Dummy(&GH_Sunflower::modelsTop); INCR_NEW("Entity");
    petals->visible = (true);
    petals->setPosition(&position);
    petals->increasePosition(0, 16.6666666f, 0);
    petals->rotY = (rotY);
    petals->updateTransformationMatrix();
    entityListToAdd->push_back(petals);
}

void GH_Sunflower::step()
{
    extern float dt;

    const float PERIOD = 0.6666666f;

    float timerPrev = timer;
    timer += dt;

    float valPrev = fmod(timerPrev, PERIOD);
    float val = fmod(timer, PERIOD);

    if (valPrev > val)
    {
        petals->setModels(&GH_Sunflower::modelsTop);
        petals->updateTransformationMatrix();
    }
    else if (valPrev < PERIOD/2.0f && val >= PERIOD/2.0f)
    {
        petals->setModels(&GH_Sunflower::modelsTop2);
        petals->updateTransformationMatrix();
    }
}

std::vector<TexturedModel*>* GH_Sunflower::getModels()
{
    return &GH_Sunflower::modelsBase;
}

void GH_Sunflower::loadStaticModels()
{
    if (GH_Sunflower::modelsBase.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GH_Sunflower static models...\n");
    #endif

    ModelLoader::loadModel(&GH_Sunflower::modelsBase, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillSunflowerBase");
    ModelLoader::loadModel(&GH_Sunflower::modelsTop,  "res/Models/Levels/GreenHillZone/Objects/", "GreenHillSunflowerTop");
    ModelLoader::loadModel(&GH_Sunflower::modelsTop2, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillSunflowerTop2");
}

void GH_Sunflower::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_Sunflower static models...\n");
    #endif

    Entity::deleteModels(&GH_Sunflower::modelsBase);
    Entity::deleteModels(&GH_Sunflower::modelsTop);
    Entity::deleteModels(&GH_Sunflower::modelsTop2);
}
