#include "Game.h"



void gameSetup()
{
    worldTilesSetup();
    worldPopulationSetup();
}

void testFunction()
{
    std::cout << "Test! \n";
}

void renderGame()
{
    drawCritters();
    displayCrittersInfo();
}

void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    runBrains();




    if((globalCycle % 60) == 0)
    {

    }

}
