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

sf::Thread serverListenThread(&serverListen);
sf::Thread clientListenThread(&clientListen);

void runServerStuffs()
{
    if(inputState.key[Key::Home].time == 1)
    {
        network::server = true;
        activateServer();
    }

    if(inputState.key[Key::End].time == 1)
    {
        network::client = true;
        activateClient();
    }

    if(network::server && !network::listening)
    {
        network::listening = true;
        serverListenThread.launch();
    }
    if(network::client && !network::listening)
    {
        network::listening = true;
        clientListen();
        //clientListenThread.launch();
        network::listening = false;
    }

    if(network::client && inputState.key[Key::V].time == 1)
        clientSendingPing();
    if(network::server && inputState.key[Key::V].time == 1)
        serverPingAll();

}

void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    runBrains();

    if(inputState.key[Key::C].time == 1)
        addCreatures(100);


    if((globalCycle % 60) == 0)
    {

    }

}
