#ifndef GETLINE_H
#define GETLINE_H

#include <istream>

//getline that works with any line endings
std::istream& getlineSafe(std::istream& is, std::string& t);

#endif
