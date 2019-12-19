#include <glad/glad.h>

#include "../entity.h"
#include "mhstagemanager.h"
#include "../../engineTester/main.h"
#include "../controllableplayer.h"
#include "../../objLoader/objLoader.h"
#include "../../audio/audioplayer.h"
#include "../camera.h"

std::list<TexturedModel*> MH_StageManager::modelsSkydome;

MH_StageManager::MH_StageManager()
{
    visible = true;
    scale = 2;
    playedBGM = false;
    timer = 1.666666f;
}

extern float dt;

void MH_StageManager::step()
{
    //set the position of the background sky dome
    position.y = Global::waterHeight;
    position.x = Global::gameCamera->eye.x;
    position.z = Global::gameCamera->eye.z;
    updateTransformationMatrix();

    //kill sonic
    if (Global::gameMainPlayer->position.y < Global::waterHeight-150)
    {
        Global::gameMainPlayer->die();
    }

    //make skybox invis if cam is too high
    if (Global::gameCamera->eye.y > 1775*scale + Global::waterHeight)
    {
        visible = false;
    }
    else
    {
        visible = true;
    }

    if (timer == 1.666666f)
    {
        if (Global::gameMainPlayer->position.y > 1500)
        {
            timer-=dt;
        }
    }
    else if (timer > 0.0f)
    {
        timer-=dt;
        timer = fmaxf(timer, 0.0f);
        AudioPlayer::setBGMVolume(timer/1.666666f);
    }
    else if (!playedBGM && Global::gameMainPlayer->vel.y < -60.0f)
    {
        playedBGM = true;
        AudioPlayer::stopBGM();
        AudioPlayer::playBGMWithIntro(2, 3);
    }
}

std::list<TexturedModel*>* MH_StageManager::getModels()
{
    return &MH_StageManager::modelsSkydome;
}

void MH_StageManager::loadStaticModels()
{
    if (MH_StageManager::modelsSkydome.size() > 0)
    {
        return;
    }

    #ifdef DEV_MODE
    std::fprintf(stdout, "Loading MH_StageManager static models...\n");
    #endif

    loadModel(&MH_StageManager::modelsSkydome,  "res/Models/Levels/MetalHarbor/Sky/", "SkySpheroid");
}

void MH_StageManager::deleteStaticModels()
{
    #ifdef DEV_MODE
    std::fprintf(stdout, "Deleting MH_StageManager static models...\n");
    #endif

    Entity::deleteModels(&MH_StageManager::modelsSkydome);
}
