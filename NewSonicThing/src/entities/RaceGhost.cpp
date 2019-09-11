#include <glad/glad.h>

#include "entity.h"
#include "../models/models.h"
#include "../toolbox/vector.h"
#include "raceghost.h"
#include "../renderEngine/renderEngine.h"
#include "../objLoader/objLoader.h"
#include "../engineTester/main.h"
#include "../toolbox/getline.h"
#include "../toolbox/split.h"
#include "maniamightymodel.h"
#include "../menu/timer.h"
#include "../toolbox/maths.h"

#include <list>
#include <cstring>
#include <fstream>


GhostFrame::GhostFrame(float time, int animIndex, float animTime, Vector3f* pos, Vector4f* rot, Vector3f* up)
{
    this->time = time;
    this->animIndex = animIndex;
    this->animTime = animTime;
    this->pos.set(pos);
    this->rot.set(rot);
    this->up.set(up);
}

GhostFrame::GhostFrame(GhostFrame* other)
{
    time = other->time;
    animIndex = other->animIndex;
    animTime = other->animTime;
    pos.set(&other->pos);
    rot.set(&other->rot);
    up.set(&other->up);
}

GhostFrame GhostFrame::interpolate(GhostFrame* f1, GhostFrame* f2, float time)
{
    if (time <= f1->time)
    {
        return GhostFrame(f1);
    }
    else if (time >= f2->time)
    {
        return GhostFrame(f2);
    }
    else
    {
        float timeDiff = f2->time - f1->time;
        float inbetween = time - f1->time;
        float percent = inbetween/timeDiff;

        float animTime = Maths::interpolate(f1->animTime, f2->animTime, percent);

        Vector3f pos;
        pos.x = Maths::interpolate(f1->pos.x, f2->pos.x, percent);
        pos.y = Maths::interpolate(f1->pos.y, f2->pos.y, percent);
        pos.z = Maths::interpolate(f1->pos.z, f2->pos.z, percent);

        Vector4f rot;
        rot.x = Maths::interpolate(f1->rot.x, f2->rot.x, percent);
        rot.y = Maths::interpolate(f1->rot.y, f2->rot.y, percent);
        rot.z = Maths::interpolate(f1->rot.z, f2->rot.z, percent);
        rot.w = Maths::interpolate(f1->rot.w, f2->rot.w, percent);

        Vector3f up;
        up.x = Maths::interpolate(f1->up.x, f2->up.x, percent);
        up.y = Maths::interpolate(f1->up.y, f2->up.y, percent);
        up.z = Maths::interpolate(f1->up.z, f2->up.z, percent);

        return GhostFrame(time, f1->animIndex, animTime, &pos, &rot, &up);
    }
}

RaceGhost::RaceGhost()
{

}

RaceGhost::RaceGhost(char* filePath)
{
    visible = false;

    std::ifstream file(Global::pathToEXE + filePath);
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file '%s'\n", (Global::pathToEXE + filePath).c_str());
		file.close();
	}
	else
	{
		std::string line;

		while (!file.eof())
		{
			getlineSafe(file, line);

			char lineBuf[512];
			memcpy(lineBuf, line.c_str(), line.size()+1);

			int splitLength = 0;
			char** lineSplit = split(lineBuf, ' ', &splitLength);

			if (splitLength == 13)
			{
                float time     = std::stof(lineSplit[0]);
                int animIndex  = std::stoi(lineSplit[1]);
                float animTime = std::stof(lineSplit[2]);
				Vector3f pos(std::stof(lineSplit[ 3]), std::stof(lineSplit[ 4]), std::stof(lineSplit[ 5]));
				Vector4f rot(std::stof(lineSplit[ 6]), std::stof(lineSplit[ 7]), std::stof(lineSplit[ 8]), std::stof(lineSplit[9]));
                Vector3f up (std::stof(lineSplit[10]), std::stof(lineSplit[11]), std::stof(lineSplit[12]));

                frames.push_back(GhostFrame(time, animIndex, animTime, &pos, &rot, &up));
			}
			free(lineSplit);
		}
		file.close();
	}

	GhostFrame* lastFrame = &frames[frames.size()-1];
    averageFramesPerSecond = frames.size()/lastFrame->time;

    myModel = new ManiaMightyModel; INCR_NEW("Entity");
	Main_addEntity(myModel);
}


void RaceGhost::step()
{
    float currentTime = 0.0f;
    if (Global::mainHudTimer != nullptr)
    {
        currentTime = Global::mainHudTimer->totalTime;
    }

    GhostFrame* lastFrame = &frames[frames.size()-1];
    if (currentTime >= lastFrame->time)
    {
        GhostFrame* f = lastFrame;
        myModel->setOrientation(
            f->pos.x, f->pos.y, f->pos.z,
            f->rot.x, f->rot.y, f->rot.z, f->rot.w,
            &f->up);
	    myModel->animate(f->animIndex, f->animTime);
    }
    else
    {
        int guessIndex = (int)(currentTime*averageFramesPerSecond);
        guessIndex = std::max(guessIndex, 0);
        guessIndex = std::min(guessIndex, (int)(frames.size() - 1));

        //now that we have a starting point that is really close to the real index, do a quick linear search to find the exact index
        //lets say time is 0.105
        GhostFrame* f = &frames[guessIndex];

        if (f->time > currentTime) //we need to go backwards
        {
            guessIndex--;
            while (guessIndex >= 0)
            {
                f = &frames[guessIndex];
                if (f->time <= currentTime)
                {
                    break;
                }
                guessIndex--;
            }
        }
        else if (f->time < currentTime) //we need to go forwards
        {
            guessIndex++;
            while (guessIndex < (int)frames.size())
            {
                f = &frames[guessIndex];
                if (f->time > currentTime)
                {
                    guessIndex--;
                    break;
                }
                guessIndex++;
            }
        }

        int guessIndex2 = guessIndex+1;
        guessIndex2 = std::min(guessIndex2, (int)(frames.size() - 1));

        GhostFrame* f1 = &frames[guessIndex];
        GhostFrame* f2 = &frames[guessIndex2];
        GhostFrame interpFrame = GhostFrame::interpolate(f1, f2, currentTime);
        myModel->setOrientation(
            interpFrame.pos.x, interpFrame.pos.y, interpFrame.pos.z,
            interpFrame.rot.x, interpFrame.rot.y, interpFrame.rot.z, interpFrame.rot.w,
            &interpFrame.up);
	    myModel->animate(interpFrame.animIndex, interpFrame.animTime);
    }
}

std::list<TexturedModel*>* RaceGhost::getModels()
{
	return nullptr;
}

void RaceGhost::loadStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Loading RaceGhost static models...\n");
	#endif

    ManiaMightyModel::loadStaticModels();
}

void RaceGhost::deleteStaticModels()
{
	#ifdef DEV_MODE
	std::fprintf(stdout, "Deleting RaceGhost static models...\n");
	#endif

	ManiaMightyModel::deleteStaticModels();
}
