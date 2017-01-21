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

void runServerStuffs()
{
    if(inputState.key[Key::Home].time == 1)
        activateServer();
    if(inputState.key[Key::End].time == 1)
        activateClient();
}

void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    runBrains();
    runServerStuffs();

    if(inputState.key[Key::C].time == 1)
        addCreatures(100);


    if((globalCycle % 60) == 0)
    {

    }

}
