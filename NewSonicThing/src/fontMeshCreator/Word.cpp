#include <list>

#include "word.h"
#include "character.h"


Word::Word(double fontSize)
{
	this->fontSize = fontSize;
}

void Word::addCharacter(Character* character)
{
	characters.push_back((*character));
	width += character->getxAdvance()*fontSize;
}

std::list<Character>* Word::getCharacters()
{
	return &characters;
}

double Word::getWordWidth()
{
	return width;
}
