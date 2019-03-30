#ifndef SRSTAGEMANAGER_H
#define SRSTAGEMANAGER_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"

class SRStageManager : public Entity
{
private:
	static std::list<TexturedModel*> modelsSkyPass1;
    static std::list<TexturedModel*> modelsSkyPass2;
    static std::list<TexturedModel*> modelsSkyPass3;
    static std::list<TexturedModel*> modelsSkyTransparent;

    Dummy* skyPass2;
    Dummy* skyPass3;
    Dummy* skyTransparent;

public:
	SRStageManager();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadStaticModels();

	static void deleteStaticModels();
};
#endif
