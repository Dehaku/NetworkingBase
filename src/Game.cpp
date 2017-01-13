#include "Game.h"

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

void gameSetup()
{
    worldTilesSetup();
}

void runGame()
{
    std::cout << "World Size: " << WorldTiles.size() << std::endl;
}
