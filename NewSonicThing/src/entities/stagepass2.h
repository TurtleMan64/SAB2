#ifndef STAGEPASS2_H
#define STAGEPASS2_H

class TexturedModel;

#include <list>
#include "entity.h"


class StagePass2 : public Entity
{
private:
	static std::list<TexturedModel*> models;

public:
	StagePass2();
	StagePass2(const char* objFolder, const char* objFilename);

	void step();

	std::list<TexturedModel*>* getModels();

	static void deleteStaticModels();
};
#endif
