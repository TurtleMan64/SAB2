#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

class Level
{
public:
	std::string fileName;
	std::string displayName;
	int numMissions;
	std::vector<std::vector<std::string>> missionData;

	Level();
};
#endif