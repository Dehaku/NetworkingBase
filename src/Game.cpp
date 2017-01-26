#include "Game.h"

void clientPacketManager::handlePackets()
{
    if(packets.size() > 0)
        std::cout << "Packets: " << packets.size() << std::endl;
    for(auto &boolPacket : packets)
    {
        sf::Packet &packet = boolPacket.packet;

        std::string in;
        sf::Uint8 type;
        packet >> type >> in;

        std::cout << "Server" << int(type) << ": \"" << in << "\"" << std::endl;
        std::cout << "Organisms size" << Organisms.size() << std::endl;

    }
    packets.clear();
}

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
        // clientListen();
        clientListenThread.launch();
        // network::listening = false;
    }

    if(network::client)
    {
        { // Locking and sorting through the packets!
            sf::Lock lock(network::packetManagerHandling);
            cPM.handlePackets();
        }

        if(inputState.key[Key::V].time == 1 && inputState.key[Key::LShift].time == 0)
            clientSendingPing();
        if(inputState.key[Key::V].time == 1 && inputState.key[Key::LShift].time >= 1)
            for(int i = 0; i != 10; i++)
                clientSendingPing();
    }


    if(network::server)
    {
        { // Locking and sorting through the packets!
            sf::Lock lock(network::packetManagerHandling);
            sPM.handlePackets();
        }

        if(inputState.key[Key::V].time == 1 && inputState.key[Key::LShift].time == 0)
            serverPingAll();
        if(inputState.key[Key::V].time == 1 && inputState.key[Key::LShift].time >= 1)
            for(int i = 0; i != 10; i++)
                serverPingAll();

        if(inputState.key[Key::B].time == 1)
            std::cout << "Clients: " << clientCount() << std::endl;

    }


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
