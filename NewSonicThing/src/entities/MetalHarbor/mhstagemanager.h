#ifndef MH_STAGEMANAGER_H
#define MH_STAGEMANAGER_H

class TexturedModel;

#include <list>
#include "../entity.h"

class MH_StageManager : public Entity
{
private:
	static std::list<TexturedModel*> modelsSkydome;

public:
	MH_StageManager();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
