#include "animation.h"
#include "keyframe.h"


Animation::Animation()
{
	
}

void Animation::addKeyframe(float time, float x, float y, float z,
	float xRot, float yRot, float zRot, float sRot, float scale)
{
	Keyframe key(time, x, y, z, xRot, yRot, zRot, sRot, scale);

	if (keyframes.size() == 0)
	{
		keyframes.push_back(key);
	}
	else if (keyframes.size() == 1)
	{
		if (time > keyframes[0].time)
		{
			keyframes.push_back(key);
		}
		else
		{
			keyframes.insert(keyframes.begin()+0, key);
		}
	}
	else
	{
		if (time < keyframes[0].time)
		{
			keyframes.insert(keyframes.begin()+0, key);
		}
		else
		{
			bool inRange = false;
			for (unsigned int i = 0; i < keyframes.size() - 1; i++)
			{
				if (time >= keyframes[i].time && time < keyframes[i + 1].time)
				{
					keyframes.insert(keyframes.begin()+(i + 1), key);
					inRange = true;
					break;
				}
			}
			if (inRange == false)
			{
				keyframes.push_back(key);
			}
		}
	}
}

void Animation::addKeyframe(float time, float xRot, float yRot, float zRot, float sRot, float scale)
{
	this->addKeyframe(time, 0, 0, 0, xRot, yRot, zRot, sRot, scale);
}