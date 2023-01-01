#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "ghflower.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"
#include "../dummy.hpp"

#include <list>
#include <iostream>
#include <cmath>

std::list<TexturedModel*> GH_Flower::modelsBase;
std::list<TexturedModel*> GH_Flower::modelsTop;

GH_Flower::GH_Flower()
{
    
}

GH_Flower::GH_Flower(float x, float y, float z, float rotY, float scale, std::list<Entity*>* entityListToAdd)
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

    petals = new Dummy(&GH_Flower::modelsTop); INCR_NEW("Entity");
    petals->visible = (true);
    petals->setPosition(&position);
    petals->rotY = (rotY);
    petals->updateTransformationMatrix();
    entityListToAdd->push_back(petals);
}

void GH_Flower::step()
{
    extern float dt;

    float valPrev = fmod(timer, 5.66666666667f/2);

    timer += dt;

    float val = fmod(timer, 5.66666666667f/2);

    if (valPrev > val)
    {
        petals->position.y = (position.y);
        petals->updateTransformationMatrix();
    }
    else if ((val >= 2.0f/2 && valPrev < 2.0f/2) || 
             (val >= 5.0f/2 && valPrev < 5.0f/2))
    {
        petals->position.y = (position.y + 1.0f);
        petals->updateTransformationMatrix();
    }
    else if (val >= 2.66666666667f/2 && valPrev < 2.66666666667f/2)
    {
        petals->position.y = (position.y + 2.0f);
        petals->updateTransformationMatrix();
    }
}

std::list<TexturedModel*>* GH_Flower::getModels()
{
    return &GH_Flower::modelsBase;
}

void GH_Flower::loadStaticModels()
{
    if (GH_Flower::modelsBase.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading GH_Flower static models...\n");
    #endif

    ModelLoader::loadModel(&GH_Flower::modelsBase, "res/Models/Levels/GreenHillZone/Objects/", "GreenHillPFlowerBase");
    ModelLoader::loadModel(&GH_Flower::modelsTop,  "res/Models/Levels/GreenHillZone/Objects/", "GreenHillPFlowerTop");
}

void GH_Flower::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting GH_Flower static models...\n");
    #endif

    Entity::deleteModels(&GH_Flower::modelsBase);
    Entity::deleteModels(&GH_Flower::modelsTop);
}
