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
    int totalRings;
    std::vector<std::vector<std::string>> missionData;

    Level();
};
#endif
