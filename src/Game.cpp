#include "Game.h"

// sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window), *button.drawView);



// Fix network syncing of organisms, as well as flora and tiles
// Switch to smart pointers already.




sf::Packet& operator <<(sf::Packet& packet, const Brain& brain)
{
    return packet
    << brain.desiresMate
    << brain.desiredPos.x
    << brain.desiredPos.y;

}

sf::Packet& operator >>(sf::Packet& packet, Brain& brain)
{
    return packet
    >> brain.desiresMate
    >> brain.desiredPos.x
    >> brain.desiredPos.y;
}

sf::Packet& operator <<(sf::Packet& packet, const Organism& critter)
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

sf::Packet& operator >>(sf::Packet& packet, Organism& critter)
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

    CritterPositions(Organism Critter)
    {
        position = Critter.pos;
        if(auto thisBrain = Critter.brain.lock())
        {
            brainBool = true;
            desiredPosition = thisBrain->desiredPos;
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
    for(auto &currentPacket : packets)
    {


        sf::Packet &packet = currentPacket.packet;
        byteKeeper.byteInput(packet);


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
            if(population != organisms.size())
                std::cout << "Population Desync! Packet: " << population << ", Us: " << organisms.size() << std::endl;

            unsigned int counter = 0;
            for(auto &critter : organisms)
            {
                if(counter >= population)
                    break;

                CritterPositions cPos;
                packet >> cPos;


                // Update Position
                critter.pos = cPos.position;

                // If you've gotta brain, you gotta desire.
                if(cPos.brainBool && critter.brain.lock())
                    critter.brain.lock()->desiredPos = cPos.desiredPosition;

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

            Organism Creature;
            Brain brain;

            for(int i = 0; i != population; i++)
            {

                packet >> Creature;
                packet >> brain;

                organisms.push_back(Creature);
                brainStorage.push_back(brain);

                //* organisms.back().brain = brainStorage.back();
                //* brainStorage.back().owner = organisms.back();

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
    for(auto &currentPacket : packets)
    {
        sf::Packet &packet = currentPacket.packet;
        byteKeeper.byteInput(packet);

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
            sendPacket << sf::Uint32(organisms.size());
            int numbers = int(sf::Uint32(organisms.size()));
            std::cout << "Things: " << organisms.size() << "/" << sf::Uint32(organisms.size()) << "/" << numbers << std::endl;

            for(auto &critter : organisms)
            {
                sendPacket << critter;
                //* sendPacket << *(critter.brain);
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

void HUDTabs()
{
    static sf::Texture* hudButton;
    if(hudButton == nullptr)
        hudButton = &texturemanager.getTexture("HUDTab.png");

    int workDudeButt = shapes.createImageButton(sf::Vector2f(-33,190),*hudButton,"",0,&gvars::hudView);
    shapes.createText(-33-40,190-8,12,sf::Color::Black,"Workdude",&gvars::hudView);
    int evolutionButt = shapes.createImageButton(sf::Vector2f(179,190),*hudButton,"",0,&gvars::hudView);
    shapes.createText(179-40,190-8,12,sf::Color::Black,"Evolution",&gvars::hudView);


    std::string simAddon = "";
    if(simulationManager.simulations.size() > 0)
        simAddon.append("("+std::to_string(simulationManager.drawSimNumber+1)+"/"+std::to_string(simulationManager.simulations.size())+")");

    int simulationButt = shapes.createImageButton(sf::Vector2f(391,190),*hudButton,"",0,&gvars::hudView);
    shapes.createText(391-40,190-8,12,sf::Color::Black,"Simulation"+simAddon,&gvars::hudView);
    int contestButt = shapes.createImageButton(sf::Vector2f(606,190),*hudButton,"",0,&gvars::hudView);
    shapes.createText(606-40,190-8,12,sf::Color::Black,"Contests",&gvars::hudView);
    int shopButt = shapes.createImageButton(sf::Vector2f(818,190),*hudButton,"",0,&gvars::hudView);
    shapes.createText(818-40,190-8,12,sf::Color::Black,"Shops",&gvars::hudView);
    int archiveButt = shapes.createImageButton(sf::Vector2f(1030,190),*hudButton,"",0,&gvars::hudView);
    shapes.createText(1030-40,190-8,12,sf::Color::Black,"Archive",&gvars::hudView);


    if(shapes.shapeClicked(workDudeButt))
    {
        std::cout << "Workdude clicked! \n";
        sf::sleep(sf::seconds(2));
    }

    if(shapes.shapeClicked(simulationButt))
    {
        int simCount = simulationManager.simulations.size();
        int &simDraw = simulationManager.drawSimNumber;
        simDraw++;
        if(simDraw >= simCount) // For when a simulation is ended, and yet we were viewing it.
            simDraw = 0;


    }

    if(shapes.shapeHovered(workDudeButt))
    {
        int posX = -33;
        int posY = 190;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(evolutionButt))
    {
        int posX = 179;
        int posY = 190;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(simulationButt))
    {
        int posX = 391;
        int posY = 190;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);

        if(inputState.rmbTime == 1 && simulationManager.simulations.size() > 0)
            simulationManager.simulations.pop_back();

    }
    if(shapes.shapeHovered(contestButt))
    {
        int posX = 606;
        int posY = 190;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(shopButt))
    {
        int posX = 818;
        int posY = 190;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(archiveButt))
    {
        int posX = 1030;
        int posY = 190;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }


}

void drawHUD()
{

    static sf::Texture* mainHud;
    if(mainHud == nullptr)
        mainHud = &texturemanager.getTexture("MainHUD.png");

    // -281, 283
    shapes.createImageButton(sf::Vector2f((1280+(-281))/2,(720+283)/2),*mainHud,"",0,&gvars::hudView);

    HUDTabs();

}

void drawFPSandData()
{
    int yOffset = 21;
    { // FPS/UPS Richtext Display
        sfe::RichText fpsText(gvars::defaultFont);
        fpsText.setPosition(-130,10*yOffset);
        yOffset++;
        fpsText.setCharacterSize(10);
        fpsText << sf::Text::Bold << sf::Color::White << "FPS/UPS:" ;

        if(fpsKeeper.framesPerSecond < 60)
            fpsText << sf::Color::Red << std::to_string(int(fpsKeeper.framesPerSecond));
        else
            fpsText << sf::Color::White << std::to_string(int(fpsKeeper.framesPerSecond));

        fpsText << sf::Color::White << "/";

        if(fpsKeeper.updatesPerSecond > 10000)
            fpsText << sf::Color::Red << std::to_string(int(fpsKeeper.updatesPerSecond));
        else
            fpsText << sf::Color::White << std::to_string(int(fpsKeeper.updatesPerSecond));

        shapes.createRichText(fpsText, &gvars::hudView);
    }

    shapes.createText(-130,10*yOffset,10,sf::Color::White, "Data: "
                      + std::to_string(int(byteKeeper.bytesPerSecond)) + " B/s, "
                      + std::to_string(int(byteKeeper.bytesCollected)) + " B, "
                      + std::to_string(int(byteKeeper.kilobytesCollected)) + " KB, "
                      + std::to_string(int(byteKeeper.megabytesCollected)) + " MB, "
                      + std::to_string(int(byteKeeper.gigabytesCollected)) + " GB"
                      , &gvars::hudView);
    yOffset++;
}

void renderGame()
{
    if(simulationManager.simulations.size() > 0)
    {
        int i = 0;
        for(auto &sim : simulationManager.simulations)
        {
            if(i == simulationManager.drawSimNumber)
                sim.drawCritters();

            i++;
        }
    }

    //drawCritters();
    //displayCrittersInfo();
    drawHUD();
    drawFPSandData();
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
    packet << sf::Uint32(organisms.size());

    // Then to the meat of the packet!
    for(auto &critter : organisms)
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
    worldTilesSetup(worldTiles);
    organisms.clear();
    flora.clear();
    worldPopulationSetup();
}




void runGame()
{
    static int globalCycle = 0;
    globalCycle++;



    for(auto &sim : simulationManager.simulations)
        runBrains(sim.organisms);
    //runBrains(organisms);

    if(inputState.key[Key::C].time == 1)
        addCreatures(100);


    if((globalCycle % 60) == 0)
    {

    }

}
