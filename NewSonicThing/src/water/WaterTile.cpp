#include "watertile.hpp"

const float WaterTile::TILE_SIZE = 1500;

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
