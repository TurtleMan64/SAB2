#include "watertile.hpp"

WaterTile::WaterTile(float centerX, float centerZ)
{
    this->x = centerX;
    this->z = centerZ;
}

float WaterTile::getX()
{
    return x;
}

float WaterTile::getZ()
{
    return z;
}
