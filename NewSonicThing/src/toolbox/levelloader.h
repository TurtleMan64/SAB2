#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <list>

class Entity;

class LevelLoader
{
private:
	static float toFloat(char* input);

	static int toInt(char* input);

	static void processLine(char** data, int dataLength, std::list<Entity*>* chunkedEntities);

	static void freeAllStaticModels();

	static int numLevels;

public:
	static void loadTitle();

	static void loadLevel(std::string levelFilename);

	static void loadLevelData();

	static int getNumLevels();
};
#endif