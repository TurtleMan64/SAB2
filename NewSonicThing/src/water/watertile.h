#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile
{
private:
    float x;
    float z;

public:
    static const float TILE_SIZE;

    WaterTile(float centerX, float centerZ);

    float getX();

    float getZ();
};
#endif