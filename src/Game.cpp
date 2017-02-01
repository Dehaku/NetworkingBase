#include "Game.h"

// sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), *button.drawView);

sf::Packet& operator <<(sf::Packet& packet, const brain& brain)
{
    return packet
    << brain.desiresMate
    << brain.desiredPos.x
    << brain.desiredPos.y;

}

sf::Packet& operator >>(sf::Packet& packet, brain& brain)
{
    return packet
    >> brain.desiresMate
    >> brain.desiredPos.x
    >> brain.desiredPos.y;
}

sf::Packet& operator <<(sf::Packet& packet, const organism& critter)
{
    return packet
    << critter.pos.x
    << critter.pos.y

    << critter.health
    << critter.baseSpeed
    << critter.size
    << critter.nutrition
    << critter.hydration

    << critter.name
    << critter.colorPrime.r
    << critter.colorPrime.g
    << critter.colorPrime.b
    << critter.colorSecondary.r
    << critter.colorSecondary.g
    << critter.colorSecondary.b

    << critter.ageMax
    << critter.age
    << critter.gestationPeriod

    << critter.gestationTime;
}

sf::Packet& operator >>(sf::Packet& packet, organism& critter)
{

    return packet
    >> critter.pos.x
    >> critter.pos.y

    >> critter.health
    >> critter.baseSpeed
    >> critter.size
    >> critter.nutrition
    >> critter.hydration

    >> critter.name
    >> critter.colorPrime.r
    >> critter.colorPrime.g
    >> critter.colorPrime.b
    >> critter.colorSecondary.r
    >> critter.colorSecondary.g
    >> critter.colorSecondary.b

    >> critter.ageMax
    >> critter.age
    >> critter.gestationPeriod

    >> critter.gestationTime;
}

struct CritterPositions
{
    sf::Vector2f position;
    bool brainBool;
    sf::Vector2f desiredPosition;
    CritterPositions()
    {
        brainBool = false;
    }

    CritterPositions(organism Critter)
    {
        position = Critter.pos;
        if(Critter.brain != nullptr)
        {
            brainBool = true;
            desiredPosition = Critter.brain->desiredPos;
        }
        else
            brainBool = false;

    }
};

sf::Packet& operator <<(sf::Packet& packet, const CritterPositions& critter)
{
    return packet
    << critter.position.x
    << critter.position.y
    << critter.brainBool
    << critter.desiredPosition.x
    << critter.desiredPosition.y;
}

sf::Packet& operator >>(sf::Packet& packet, CritterPositions& critter)
{
    return packet
    >> critter.position.x
    >> critter.position.y
    >> critter.brainBool
    >> critter.desiredPosition.x
    >> critter.desiredPosition.y;
}





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



        else if(type == sf::Uint8(ident::organismUpdate) )
        {
            std::cout << "Received update of ";
            unsigned int population;
            packet >> population;
            std::cout << population << std::endl;
            if(population != Organisms.size())
                std::cout << "Population Desync! Packet: " << population << ", Us: " << Organisms.size() << std::endl;

            unsigned int counter = 0;
            for(auto &critter : Organisms)
            {
                if(counter >= population)
                    break;

                CritterPositions cPos;
                packet >> cPos;


                // Update Position
                critter.pos = cPos.position;

                // If you've gotta brain, you gotta desire.
                if(cPos.brainBool)
                    critter.brain->desiredPos = cPos.desiredPosition;

                counter++;
            }

        }

        else if(type == sf::Uint8(ident::organismInitialization ) )
        {


            int population;
            sf::Uint32 recPop;
            packet >> recPop;
            population = int(recPop);
            std::cout << population << std::endl;

            std::cout << "Received Organism Initial ( " << population << ") \n";
            int counter = 0;

            organism Creature;
            brain Brain;

            for(int i = 0; i != population; i++)
            {

                packet >> Creature;
                packet >> Brain;

                Organisms.push_back(Creature);
                BrainStorage.push_back(Brain);

                Organisms.back().brain = &BrainStorage.back();
                BrainStorage.back().owner = &Organisms.back();

                counter++;
            }
        }

        else if(type == sf::Uint8(ident::clientID) )
        {
            std::cout << "Received our ID: ";
            packet >> myID;
            std::cout << int(myID) << std::endl;
            std::cout << "Requesting initial Flora and Organism setup. \n";
            sf::Packet requestPacket;
            requestPacket << sf::Uint8(ident::floraInitialization) << myID;
            serverSocket.send(requestPacket);

            requestPacket.clear();
            requestPacket << sf::Uint8(ident::organismInitialization) << myID;
            serverSocket.send(requestPacket);

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
        else if(type == sf::Uint8(ident::organismInitialization ) )
        {
            std::cout << "organism Initial 'Request' received from " << currentPacket.sender->id << std::endl;
            sf::Packet sendPacket;

            // Send the same type back.
            sendPacket << type;
            sendPacket << sf::Uint32(Organisms.size());
            int numbers = int(sf::Uint32(Organisms.size()));
            std::cout << "Things: " << Organisms.size() << "/" << sf::Uint32(Organisms.size()) << "/" << numbers << std::endl;

            for(auto &critter : Organisms)
            {
                sendPacket << critter;
                sendPacket << *(critter.brain);
            }




            currentPacket.sender->socket->send(sendPacket);

        }
        else if(type == sf::Uint8(ident::floraInitialization ) )
        {
            std::cout << "flora Initial 'Request' received from " << currentPacket.sender->id << std::endl;
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

void sendLifeUpdate()
{
    std::cout << "Sending Life Update! \n";
    sf::Packet packet;

    // Labeling the type of packet.
    packet << sf::Uint8(ident::organismUpdate);

    // Now we send the amount of creatures, so we know how many times to update. Also functions as a sync check.
    packet << sf::Uint32(Organisms.size());

    // Then to the meat of the packet!
    for(auto &critter : Organisms)
    {
        CritterPositions CPos(critter);
        //cPos.position = critter.pos;

        //packet << critter.brain->desiredPos.x << critter.brain->desiredPos.y;
        packet << CPos;
    }

    sendToAllClients(packet);
}

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


    // The Time Zone, tee hee.
    static bool oneSecondPassed = false;
    static sf::Clock oneSecondTimer;
    if(oneSecondTimer.getElapsedTime().asSeconds() > 1)
    {
        oneSecondTimer.restart();
        oneSecondPassed = true;
    }

    static bool tenSecondPassed = false;
    static sf::Clock tenSecondTimer;
    if(tenSecondTimer.getElapsedTime().asSeconds() > 10)
    {
        tenSecondTimer.restart();
        tenSecondPassed = true;
    }

    static bool oneMinutePassed = false;
    static sf::Clock oneMinuteTimer;
    if(oneMinuteTimer.getElapsedTime().asSeconds() > 60)
    {
        oneMinuteTimer.restart();
        oneMinutePassed = true;
    }


    if(oneSecondPassed)
    {
        oneSecondPassed = false;
        if(network::server)
            sendLifeUpdate();
        //std::cout << "One Second Passed! \n";
    }
    if(tenSecondPassed)
    {
        tenSecondPassed = false;


        //std::cout << "Ten Seconds Passed! \n";
    }
    if(oneMinutePassed)
    {
        oneMinutePassed = false;
        //std::cout << "One Minute Passed! \n";
    }


    if(inputState.key[Key::X].time == 1)
        std::cout << "Breakin console. \n";


}

void simulationInitialization()
{
    worldTilesSetup();
    Organisms.clear();
    Flora.clear();
    worldPopulationSetup();
}

void runGame()
{
    static int globalCycle = 0;
    globalCycle++;

    if(inputState.key[Key::F])
    {
        std::cout << "Doing! \n";
        sf::Packet packetez;
        std::cout << "Packet Size: " << packetez.getDataSize() << std::endl;
        for(auto &critter : Organisms)
        {
            packetez << critter;
            packetez << *(critter.brain);

        }
        std::cout << "Packet Size: " << packetez.getDataSize() << std::endl;
        Organisms.clear();
        BrainStorage.clear();




        for(int i = 0; i != 10; i++)
        {
            organism Crit;
            brain Brain;

            Organisms.push_back(Crit);
            BrainStorage.push_back(Brain);

            packetez >> Organisms.back();
            packetez >> BrainStorage.back();

            Organisms.back().brain = &BrainStorage.back();
            BrainStorage.back().owner = &Organisms.back();


        }
        for(auto &critter : Organisms)
        {

        }



    }


    runBrains();

    if(inputState.key[Key::C].time == 1)
        addCreatures(100);


    if((globalCycle % 60) == 0)
    {

    }

}
