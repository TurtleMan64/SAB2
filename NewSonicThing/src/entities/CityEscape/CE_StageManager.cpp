#include <glad/glad.h>

#include "../entity.hpp"
#include "cestagemanager.hpp"
#include "../../engineTester/main.hpp"
#include "../controllableplayer.hpp"
#include "../../loading/modelloader.hpp"
#include "../camera.hpp"
#include "../../audio/audioplayer.hpp"

std::list<TexturedModel*> CE_StageManager::modelsSkydome;

CE_StageManager::CE_StageManager()
{
    scale = 1.5f;
    visible = true;
    playedBGM = false;
}

void CE_StageManager::step()
{
    position.set(&Global::gameMainPlayer->position);
    updateTransformationMatrix();

    if (!playedBGM)
    {
        if (Global::gameMainPlayer->position.z > 9189.12f && 
            Global::gameMainPlayer->position.y < -15756.9f)
        {
            playedBGM = true;
            AudioPlayer::stopBGM();
            AudioPlayer::playBGMWithIntro(2, 3);
        }
    }
}

std::list<TexturedModel*>* CE_StageManager::getModels()
{
    return &CE_StageManager::modelsSkydome;
}

void CE_StageManager::loadStaticModels()
{
    if (CE_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading CE_StageManager static models...\n");
    #endif

    ModelLoader::loadModel(&CE_StageManager::modelsSkydome,  "res/Models/Levels/CityEscape/Sky/", "Sky");
}

void CE_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting CE_StageManager static models...\n");
    #endif

    Entity::deleteModels(&CE_StageManager::modelsSkydome);
}
