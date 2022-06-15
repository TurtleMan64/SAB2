#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile
{
private:
    int xIdx;
    int zIdx;
    float x;
    float z;

public:
    static const float TILE_SIZE;

    WaterTile(float centerX, float centerZ);

    float getX();

    float getZ();
};
#endif
