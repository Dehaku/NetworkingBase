#include "Game.h"



void gameSetup()
{
    worldTilesSetup();
    worldPopulationSetup();
}

void displayCritters()
{
    //for(auto &i : Organisms)
    {
        std::cout << "Success? \n";
    }
}

void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    displayCritters();

    if((globalCycle % 60) == 0)
    {
        std::cout << "World Size: " << WorldTiles.size() << std::endl;
        std::cout << "World Population: " << Organisms.size() << std::endl;
    }

}
