#include "Game.h"



void gameSetup()
{
    worldTilesSetup();
    worldPopulationSetup();
}



void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    drawCritters();
    displayCrittersInfo();


    if((globalCycle % 60) == 0)
    {
        //std::cout << "World Size: " << WorldTiles.size() << std::endl;
        //std::cout << "World Population: " << Organisms.size() << std::endl;
    }

}
