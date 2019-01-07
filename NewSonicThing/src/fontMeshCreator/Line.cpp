#include <list>

#include "line.h"


Line::Line(double spaceWidth, double fontSize, double maxLength)
{
	this->spaceSize = spaceWidth * fontSize;
	this->maxLength = maxLength;
}

bool Line::attemptToAddWord(Word* word)
{
	double additionalLength = word->getWordWidth();
	additionalLength += !words.empty() ? spaceSize : 0;
	if (currentLineLength + additionalLength <= maxLength)
	{
		words.push_back(*word);
		currentLineLength += additionalLength;
		return true;
	}
	else
	{
		return false;
	}
}

double Line::getMaxLength()
{
	return maxLength;
}


double Line::getLineLength()
{
	return currentLineLength;
}


std::list<Word>* Line::getWords()
{
	return &words;
}
