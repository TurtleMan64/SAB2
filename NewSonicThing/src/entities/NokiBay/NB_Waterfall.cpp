#include <glad/glad.h>

#include "../entity.hpp"
#include "../../models/models.hpp"
#include "../../toolbox/vector.hpp"
#include "nbwaterfall.hpp"
#include "../../loading/modelloader.hpp"
#include "../../engineTester/main.hpp"

#include <list>
#include <iostream>

std::vector<TexturedModel*> NB_Waterfall::models;

NB_Waterfall::NB_Waterfall()
{
    position.x = 0;
    position.y = 0;
    position.z = 0;
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    scale = 1;
    visible = true;
    updateTransformationMatrix();
    //todo: maybe make  another render order where it does not render until after the water renders.
    // this means if it is underwater it will be invisible, and it will also not show up in water reflections.
    // but for instance this waterfall, i think that is a good trade off.
}

void NB_Waterfall::step()
{
    //todo waterfall noise
}

std::vector<TexturedModel*>* NB_Waterfall::getModels()
{
    return &NB_Waterfall::models;
}

void NB_Waterfall::loadStaticModels()
{
    if (NB_Waterfall::models.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    printf("Loading NB_Waterfall static models...\n");
    #endif

    ModelLoader::loadModel(&NB_Waterfall::models, "res/Models/Levels/NokiBay/Waterfall/", "Waterfall");

    //for (auto element : NB_Waterfall::models)
    //{
        //printf("%d\n", element->renderOrder);
        //printf("%d\n", element->getTexture()->renderOrder);
    //}
}

void NB_Waterfall::deleteStaticModels()
{
    #ifdef DEV_MODE
    printf("Deleting NB_Waterfall static models...\n");
    #endif

    Entity::deleteModels(&NB_Waterfall::models);
}
