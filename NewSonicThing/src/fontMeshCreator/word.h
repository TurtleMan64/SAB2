#ifndef WORD_H
#define WORD_H

#include <list>
#include "character.h"

/**
* During the loading of a text this represents one word in the text.
* @author Karl
*
*/
class Word
{
private:
	std::list<Character> characters;
	double width = 0;
	double fontSize;

public:
	/**
	* Create a new empty word.
	* @param fontSize - the font size of the text which this word is in.
	*/
	Word(double fontSize);

	/**
	* Adds a character to the end of the current word and increases the screen-space width of the word.
	* @param character - the character to be added.
	*/
	void addCharacter(Character* character);

	/**
	* @return The list of characters in the word.
	*/
	std::list<Character>* getCharacters();

	/**
	* @return The width of the word in terms of screen size.
	*/
	double getWordWidth();
};

#endif