#include "Tiles.h"


WorldTile::WorldTile()
{

    // 107, 64, 0

    int ran = random(1,10);
    if(ran <= 2)
    { // 20% Water
        isWater = true;
        color = sf::Color(0,0,150);
        moveSpeedModifier = 0.8;
        temperature = random(0,110);

    }
    else if (ran <= 4)
    { // 20% Rocky
        isWater = false;
        color = sf::Color(120,120,120);
        moveSpeedModifier = 0.5;
        temperature = random(0,50);
    }
    else if (ran <= 6)
    { // 20% Desert
        isWater = false;
        color = sf::Color(180,180,60);
        moveSpeedModifier = 1.2;
        temperature = random(90,110);
    }
    else
    { // The rest is dirt.
        isWater = false;
        color = sf::Color(107,64,0);
        moveSpeedModifier = 1;
        temperature = random(40,80);
    }
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

void worldTilesSetup(std::vector<std::vector<WorldTile>>& worldlies)
{
    resizeWorld(10,10,worldlies);

    std::cout << "W: " << worldlies.size();
    std::cout << "0: " << worldlies[0].size()
    << std::endl;
    //std::cout << "1: " << worldlies[0][0].size();

    for(unsigned int i = 0; i != worldlies.size(); i++)
    {
        for(unsigned int t = 0; t != worldlies[i].size(); t++)
        {
            std::cout << "X";
        }
        std::cout << std::endl;
    }

}
