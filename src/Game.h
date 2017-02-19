#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <iostream>
#include <list>
#include <memory>

#include "Text.h"

#include "Shapes.h"
#include "math.h"
#include "Tiles.h"

#include "Simulation.h"

#include "Organism.h"
#include "util.h"
#include "Networking.h"
#include "Textures.h"
#include "Camera.h"

bool chatCommand(std::string input);

void simulationInitialization();

void displayCritters();

void gameSetup();

void runServerStuffs();
void runGame();
void renderGame();



#endif // GAME_H_INCLUDED
