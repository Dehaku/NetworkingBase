#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <iostream>
#include <list>

#include "Shapes.h"
#include "math.h"
#include "Tiles.h"
#include "Organism.h"
#include "util.h"
#include "Networking.h"




void displayCritters();

void gameSetup();

void runServerStuffs();
void runGame();
void renderGame();



#endif // GAME_H_INCLUDED
