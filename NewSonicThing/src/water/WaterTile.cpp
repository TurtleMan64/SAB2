#include "watertile.h"

const float WaterTile::TILE_SIZE = 1500;

WaterTile::WaterTile(float centerX, float centerZ, float height)
{
	this->x = centerX;
	this->z = centerZ;
	this->height = height;
}

float WaterTile::getHeight()
{
	return height;
}

float WaterTile::getX()
{
	return x;
}

float WaterTile::getZ()
{
	return z;
}
