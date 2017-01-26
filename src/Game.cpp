#include "Game.h"

void clientPacketManager::handlePackets()
{
    if(packets.size() > 0)
        std::cout << "Packets: " << packets.size() << std::endl;
    for(auto &currentPacket : packets)
    {
        sf::Packet &packet = currentPacket.packet;
        sf::Uint8 type;
        packet >> type;


        if(type == sf::Uint8(ident::message))
        {
            std::string in;
            packet >> in;
            std::cout << "Server" << int(type) << ": \"" << in << "\"" << std::endl;
        }
        else if(type == sf::Uint8(ident::clientID) )
        {
            std::cout << "Received out ID: ";
            packet >> myID;
            std::cout << int(myID) << std::endl;
        }



    }
    packets.clear();
}

void serverPacketManager::handlePackets()
{
    if(packets.size() > 0)
        std::cout << "Packets: " << packets.size() << std::endl;
    for(auto &currentPacket : packets)
    {
        sf::Packet &packet = currentPacket.packet;
        sf::Uint8 type;
        packet >> type;

        if(type == sf::Uint8(ident::message))
        {
            std::string in;
            packet >> in;
            std::cout << "Client" << int(type) << ": \"" << in << "\"" << std::endl;
        }

        else if(type == sf::Uint8(ident::clientID) )
        {
            std::cout << "ID Request received. \n";
            sf::Uint8 sillyID;
            packet >> sillyID; // Placeholder for login/profile info.

            sf::Packet returnPacket;
            // Send them the ID assigned to them when they joined.
            returnPacket << sf::Uint8(ident::clientID) << sf::Uint8(currentPacket.sender->id);
            currentPacket.sender->socket->send(returnPacket);
            std::cout << "Sent ID to client. \n";
        }



    }
    packets.clear();
}

void gameSetup()
{
    texturemanager.init();


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

void simulationInitialization()
{
    worldTilesSetup();
    worldPopulationSetup();
}

void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    if(inputState.key[Key::Return].time == 1)
        simulationInitialization();


    runBrains();

    if(inputState.key[Key::C].time == 1)
        addCreatures(100);


    if((globalCycle % 60) == 0)
    {

    }

}
