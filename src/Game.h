#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <iostream>
#include "Tiles.h"

extern std::vector<std::vector<worldtile>> WorldTiles;

void gameSetup();

void runGame();



#endif // GAME_H_INCLUDED
