#include "Tiles.h"

class worldtile{
public:
    //sf::Color color;

    bool isWater;
    float moveSpeedModifier;
    float temperature;

    worldtile()
    {
        isWater = false;
        moveSpeedModifier = 0;
        temperature = 50;
    }
};

std::vector<std::vector<worldtile>> WorldTiles;

void resizeWorld(int x, int y)
{
    WorldTiles.resize(x);

    for(int i = 0; i != x; i++)
    {
        WorldTiles.at(i).resize(y);
    }
}

void worldTilesSetup()
{
    resizeWorld(10,10);
}
