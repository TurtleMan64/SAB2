#ifndef ROCKETHORIZONTAL_H
#define ROCKETHORIZONTAL_H

class TexturedModel;
class Body;
class Source;

#include <list>
#include "./entity.h"
#include "./collideableobject.h"
#include "./rocketgeneral.h"


class RocketHorizontal : public RocketGeneral
{
public:
	RocketHorizontal();
	RocketHorizontal(int point1ID, int point2ID);

	void step();
};
#endif