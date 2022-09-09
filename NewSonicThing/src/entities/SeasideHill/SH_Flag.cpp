#include "shflag.hpp"
#include "../entity.hpp"
#include "../../engineTester/main.hpp"
#include "../../models/models.hpp"
#include "../../loading/modelloader.hpp"
#include "../../toolbox/maths.hpp"

#include <list>
#include <cmath>

std::list<TexturedModel*> SH_Flag::models0;
std::list<TexturedModel*> SH_Flag::models1;
std::list<TexturedModel*> SH_Flag::models2;
std::list<TexturedModel*> SH_Flag::models3;
std::list<TexturedModel*> SH_Flag::models4;
std::list<TexturedModel*> SH_Flag::models5;

SH_Flag::SH_Flag(float x, float y, float z, float scale)
{
    position.set(x, y, z);
    rotY = 45;
    this->scale = scale;
    updateTransformationMatrix();
    timer = 6*Maths::random();
}

void SH_Flag::step()
{
    extern float dt;
    timer += 10*dt;
    timer = fmodf(timer, 5.99999f);
}

std::list<TexturedModel*>* SH_Flag::getModels()
{
    switch ((int)timer)
    {
        case  0: return &SH_Flag::models0;
        case  1: return &SH_Flag::models1;
        case  2: return &SH_Flag::models2;
        case  3: return &SH_Flag::models3;
        case  4: return &SH_Flag::models4;
        case  5: return &SH_Flag::models5;
        default: return &SH_Flag::models0;
    }
}

void SH_Flag::loadStaticModels()
{
    if (SH_Flag::models0.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading SH_Flag static models...\n");
    #endif

    ModelLoader::loadModel(&SH_Flag::models0, "res/Models/Levels/SeasideHill/Objects/Flag/", "Flag0");
    ModelLoader::loadModel(&SH_Flag::models1, "res/Models/Levels/SeasideHill/Objects/Flag/", "Flag1");
    ModelLoader::loadModel(&SH_Flag::models2, "res/Models/Levels/SeasideHill/Objects/Flag/", "Flag2");
    ModelLoader::loadModel(&SH_Flag::models3, "res/Models/Levels/SeasideHill/Objects/Flag/", "Flag3");
    ModelLoader::loadModel(&SH_Flag::models4, "res/Models/Levels/SeasideHill/Objects/Flag/", "Flag4");
    ModelLoader::loadModel(&SH_Flag::models5, "res/Models/Levels/SeasideHill/Objects/Flag/", "Flag5");
}

void SH_Flag::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting SH_Flag static models...\n");
    #endif

    Entity::deleteModels(&SH_Flag::models0);
    Entity::deleteModels(&SH_Flag::models1);
    Entity::deleteModels(&SH_Flag::models2);
    Entity::deleteModels(&SH_Flag::models3);
    Entity::deleteModels(&SH_Flag::models4);
    Entity::deleteModels(&SH_Flag::models5);
}
