#include "Tiles.h"


WorldTile::WorldTile()
    {
        isWater = false;
        moveSpeedModifier = 0;
        temperature = 50;
    }

std::vector<std::vector<WorldTile>> worldTiles;

void resizeWorld(int x, int y, std::vector<std::vector<WorldTile>>& worldlies)
{
    worldlies.resize(x);

    for(int i = 0; i != x; i++)
    {
        worldlies.at(i).resize(y);
    }
}

void worldTilesSetup()
{
    resizeWorld(10,10,worldTiles);
    for(auto i : worldTiles)
    {
        std::cout << "Wee!" << std::endl;
    }
}
