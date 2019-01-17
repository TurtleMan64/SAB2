#ifndef GFSTAGEMANAGER_H
#define GFSTAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class GFStageManager : public Entity
{
private:
	static std::list<TexturedModel*> modelsSkydome;
	float pollenSpawnTimer = 0;
	const float pollenSparseness = 0.001f;
	const float pollenRadius = 200.0f;

public:
	GFStageManager();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif