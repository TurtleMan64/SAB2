#include "insertionsort.h"
#include "particle.h"

#include <list>

//This whole thing could probably be implemented better with vector instead of list.
//	Not sure if inserting and removing from the vector would be worse than 
//	iterating through the list so many times.

void InsertionSort::sortHighToLow(std::list<Particle*>* list)
{
	int size = (int)list->size();
	for (int i = 1; i < size; i++)
	{
		std::list<Particle*>::iterator it = list->begin();


		std::advance(it, i-1);
		Particle* itemMinusOne = *it;

		std::advance(it, 1);
		Particle* item = *it;

		if (item->getDistance() > itemMinusOne->getDistance())
		{
			sortUpHighToLow(list, i);
		}
	}
}

void InsertionSort::sortUpHighToLow(std::list<Particle*>* list, int i)
{
	Particle* item = get(list, i);
	int attemptPos = i - 1;
	while (attemptPos != 0 && get(list, attemptPos - 1)->getDistance() < item->getDistance())
	{
		attemptPos--;
	}
	list->remove(item);

	std::list<Particle*>::iterator it = list->begin();
	std::advance(it, attemptPos);

	list->insert(it, item);
}

Particle* InsertionSort::get(std::list<Particle*>* list, int i)
{
	std::list<Particle*>::iterator it = list->begin();

	std::advance(it, i);

	return *it;
}