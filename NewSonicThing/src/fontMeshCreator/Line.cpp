#include <list>

#include "line.hpp"


Line::Line(float spaceWidth, float fontSize, float maxLength)
{
    this->spaceSize = spaceWidth * fontSize;
    this->maxLength = maxLength;
}

bool Line::attemptToAddWord(Word* word)
{
    float additionalLength = word->getWordWidth();
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

float Line::getMaxLength()
{
    return maxLength;
}

float Line::getLineLength()
{
    return currentLineLength;
}

std::list<Word>* Line::getWords()
{
    return &words;
}
