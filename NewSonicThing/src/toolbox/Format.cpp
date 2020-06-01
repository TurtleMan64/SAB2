#include <stdlib.h> 
#include <math.h>

#include "format.h"

std::string Format::floatToPretty(float value, int numDecimals)
{
    if (numDecimals == 2) //lol
    {
        std::string leftOfDec = std::to_string((int)value);
        std::string decimal = std::to_string(fmodf(value, 1.0f));
        return leftOfDec + decimal.substr(1, 3);
    }

    return std::to_string(value);
}
