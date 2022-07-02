#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile
{
private:
    float x;
    float z;

public:
    static constexpr float TILE_SIZE = 3000.0f*2;

    WaterTile(float centerX, float centerZ);

    float getX();

    float getZ();
};
#endif
