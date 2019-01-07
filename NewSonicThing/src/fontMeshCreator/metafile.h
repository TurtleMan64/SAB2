#ifndef METAFILE_H
#define METAFILE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "character.h"


class MetaFile
{
private:
	static const int PAD_TOP = 0;
	static const int PAD_LEFT = 1;
	static const int PAD_BOTTOM = 2;
	static const int PAD_RIGHT = 3;

	static const int DESIRED_PADDING = 8;

	static const char SPLITTER = ' ';
	static const char NUMBER_SEPARATOR = ',';

	double aspectRatio;

	double verticalPerPixelSize;
	double horizontalPerPixelSize;
	double spaceWidth;
	std::vector<int> padding;
	int paddingWidth;
	int paddingHeight;

	std::unordered_map<int, Character> metaData;

	std::ifstream* reader;

	std::unordered_map<std::string, std::string> values;


	/**
	* Read in the next line and store the variable values.
	*
	* @return {@code true} if the end of the file hasn't been reached.
	*/
	bool processNextLine();

	/**
	* Gets the {@code int} value of the variable with a certain name on the
	* current line.
	*
	* @param variable
	*            - the name of the variable.
	* @return The value of the variable.
	*/
	int getValueOfVariable(std::string variable);

	/**
	* Gets the array of ints associated with a variable on the current line.
	*
	* @param variable
	*            - the name of the variable.
	* @return The int array of values associated with the variable.
	*/
	std::vector<int> getValuesOfVariable(std::string variable);

	/**
	* Closes the font file after finishing reading.
	*/
	void close();

	/**
	* Opens the font file, ready for reading.
	*
	* @param file
	*            - the font file.
	*/
	void openFile(std::string filename);

	/**
	* Loads the data about how much padding is used around each character in
	* the texture atlas.
	*/
	void loadPaddingData();

	/**
	* Loads information about the line height for this font in pixels, and uses
	* this as a way to find the conversion rate between pixels in the texture
	* atlas and screen-space.
	*/
	void loadLineSizes();

	/**
	* Loads in data about each character and stores the data in the
	* {@link Character} class.
	*
	* @param imageWidth
	*            - the width of the texture atlas in pixels.
	*/
	void loadCharacterData(int imageWidth);

	/**
	* Loads all the data about one character in the texture atlas and converts
	* it all from 'pixels' to 'screen-space' before storing. The effects of
	* padding are also removed from the data.
	*
	* @param imageSize
	*            - the size of the texture atlas in pixels.
	* @return The data about the character.
	*/
	Character* loadCharacter(int imageSize);

public:
	/**
	* Opens a font file in preparation for reading.
	*
	* @param file
	*            - the font file.
	*/
	MetaFile(std::string filename);

	double getSpaceWidth();

	Character getCharacter(int ascii);
};

#endif