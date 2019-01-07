#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile
{
private:
	float height;
	float x;
	float z;

public:
	static const float TILE_SIZE;

	WaterTile(float centerX, float centerZ, float height);

	float getHeight();

	float getX();

	float getZ();
};
#endif