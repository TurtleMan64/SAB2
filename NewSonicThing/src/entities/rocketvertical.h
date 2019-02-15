#ifndef ROCKETVERTICAL_H
#define ROCKETVERTICAL_H

class TexturedModel;
class Body;
class Source;

#include <list>
#include "./entity.h"
#include "./collideableobject.h"
#include "./rocketgeneral.h"


class RocketVertical : public RocketGeneral
{
public:
	RocketVertical();
	RocketVertical(int point1ID, int point2ID);

	void step();
};
#endif