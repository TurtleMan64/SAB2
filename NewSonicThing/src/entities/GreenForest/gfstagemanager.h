#ifndef GF_STAGEMANAGER_H
#define GF_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class GF_StageManager : public Entity
{
private:
	static std::list<TexturedModel*> modelsSkydome;
	float pollenSpawnTimer = 0;
	const float pollenSparseness = 0.001f;
	const float pollenRadius = 200.0f;

public:
	GF_StageManager();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
