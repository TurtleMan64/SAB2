#ifndef T_STAGEMANAGER_H
#define T_STAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"

class T_StageManager : public Entity
{
private:
	static std::list<TexturedModel*> modelsSkyPass1;
    static std::list<TexturedModel*> modelsSkyPass2;
    static std::list<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2 = nullptr;
    Dummy* skyTransparent = nullptr;

public:
	T_StageManager();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
