#ifndef LINE_H
#define LINE_H

#include "word.h"

/**
* Represents a line of text during the loading of a text.
*
* @author Karl
*
*/
class Line
{
private:
	double maxLength;
	double spaceSize;

	std::list<Word> words;
	double currentLineLength = 0;

public:
	/**
	* Creates an empty line.
	*
	* @param spaceWidth
	*            - the screen-space width of a space character.
	* @param fontSize
	*            - the size of font being used.
	* @param maxLength
	*            - the screen-space maximum length of a line.
	*/
	Line(double spaceWidth, double fontSize, double maxLength);

	/**
	* Attempt to add a word to the line. If the line can fit the word in
	* without reaching the maximum line length then the word is added and the
	* line length increased.
	*
	* @param word
	*            - the word to try to add.
	* @return {@code true} if the word has successfully been added to the line.
	*/
	bool attemptToAddWord(Word* word);

	/**
	* @return The max length of the line.
	*/
	double getMaxLength();

	/**
	* @return The current screen-space length of the line.
	*/
	double getLineLength();

	/**
	* @return The list of words in the line.
	*/
	std::list<Word>* getWords();
};

#endif