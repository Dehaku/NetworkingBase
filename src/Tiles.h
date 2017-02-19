#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "util.h"

class WorldTile{
public:

    sf::Color color;
    bool isWater;
    float moveSpeedModifier;
    float temperature;
    WorldTile();


};

extern std::vector<std::vector<WorldTile>> worldTiles;

void resizeWorld(int x, int y, std::vector<std::vector<WorldTile>>& worldlies);



void worldTilesSetup(std::vector<std::vector<WorldTile>>& worldlies);


#endif // TILES_H_INCLUDED
