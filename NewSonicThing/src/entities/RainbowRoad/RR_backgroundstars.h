#ifndef RR_BACKGROUNDSTARS_H
#define RR_BACKGROUNDSTARS_H

class TexturedModel;
class Dummy;

#include <list>
#include "../entity.h"
#include <string>


class RR_BackgroundStars : public Entity
{
private:
	static std::list<TexturedModel*> modelsPass1;
	static std::list<TexturedModel*> modelsPass2;

	Dummy* pass2;

public:
	RR_BackgroundStars();

	void step();

	std::list<TexturedModel*>* getModels();

	static void loadModels();

	static void deleteModels();
};
#endif