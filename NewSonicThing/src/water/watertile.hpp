#ifndef WATERTILE_H
#define WATERTILE_H

class WaterTile
{
public:
    float centerX;
    float centerZ;

    static constexpr float TILE_SIZE = 3000.0f*2;

    WaterTile(float centerX, float centerZ);
};
#endif
