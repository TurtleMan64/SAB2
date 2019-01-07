#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

class Particle;

#include <list>

class InsertionSort
{
public:
	static void sortHighToLow(std::list<Particle*>* list);

private:
	static void sortUpHighToLow(std::list<Particle*>* list, int i);

	static Particle* get(std::list<Particle*>* list, int i);
};
#endif