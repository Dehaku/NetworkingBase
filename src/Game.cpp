#include "Game.h"

StateTracker::StateTracker()
{
    currentState = mainMenu;
}
StateTracker stateTracker;


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
    << critter.colorPrime.a
    << critter.colorSecondary.r
    << critter.colorSecondary.g
    << critter.colorSecondary.b
    << critter.colorSecondary.a

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
    >> critter.colorPrime.a
    >> critter.colorSecondary.r
    >> critter.colorSecondary.g
    >> critter.colorSecondary.b
    >> critter.colorSecondary.a

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

sf::Packet& operator <<(sf::Packet& packet, const WorldTile& tile)
{
    return packet
    << tile.color.r
    << tile.color.g
    << tile.color.b
    << tile.color.a
    << sf::Uint8(tile.isWater)
    << tile.moveSpeedModifier
    << tile.temperature;

}

sf::Packet& operator >>(sf::Packet& packet, WorldTile& tile)
{
    return packet
    >> tile.color.r
    >> tile.color.g
    >> tile.color.b
    >> tile.color.a
    >> tile.isWater
    >> tile.moveSpeedModifier
    >> tile.temperature;
}

sf::Packet& operator <<(sf::Packet& packet, const Simulation& sim)
{
    // ID
    // Organism Pop
    // Organism then Brain
    // Flora Pop
    // Flora
    // Tiles
    // Runtime

    packet << sf::Uint32(sim.simulationID);

    packet << sf::Uint32(sim.organisms.size());
    for(auto critter : sim.organisms)
    {
        packet << *(critter.get());
        packet << *(critter.get()->brain.lock());
    }

    packet << sf::Uint32(sim.flora.size());
    for(auto plant : sim.flora)
    {
        packet << *(plant.get());
    }

    packet << sf::Uint32(sim.worldTiles.size());
    packet << sf::Uint32(sim.worldTiles[0].size()); // We only allow square worlds, so using 0 is just fine.
    packet << sf::Uint32(sim.worldTileSizeX);
    packet << sf::Uint32(sim.worldTileSizeY);

    for(int i = 0; i != sim.worldTiles.size(); i++)
        for(int t = 0; t != sim.worldTiles[0].size(); t++)
            packet << sim.worldTiles[i][t];



    return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Simulation& sim)
{
    packet >> sim.simulationID;
    int critterPop;
    packet >> critterPop;

    for(int i = 0; i != critterPop; i++)
    {
        // Create critter
        std::shared_ptr<Organism> Critter(new Organism());
        sim.organisms.push_back(Critter);

        // Extract and Overwrite
        packet >> *(sim.organisms.back().get());

        // Create Brain
        std::shared_ptr<Brain> creatureBrain(new Brain());
        sim.brainStorage.push_back(creatureBrain);

        // Extract and Overwrite
        packet >> *(sim.brainStorage.back().get());

        // Link brains and bodies.
        sim.organisms.back().get()->brain = sim.brainStorage.back();
        sim.brainStorage.back().get()->owner = sim.organisms.back();
    }

    int plantPop;
    packet >> plantPop;
    for(int i = 0; i != plantPop; i++)
    {

        // Create plant
        std::shared_ptr<Organism> plant(new Organism());
        sim.flora.push_back(plant);

        // Extract and Overwrite
        packet >> *(sim.flora.back().get());

    }


    int iTiles;
    int tTiles;
    packet >> iTiles;
    packet >> tTiles;
    packet >> sim.worldTileSizeX;
    packet >> sim.worldTileSizeY;

    resizeWorld(iTiles,tTiles,sim.worldTiles);

    for(int i = 0; i != sim.worldTiles.size(); i++)
        for(int t = 0; t != sim.worldTiles[0].size(); t++)
            packet >> sim.worldTiles[i][t];

    return packet;
}


bool chatCommand(std::string input)
{
    std::vector<std::string> elements;
    bool finished = false;
    sf::Color errorColor(100,100,100);
    sf::Color warmColor(255,150,150);
    sf::Color goodColor = sf::Color::White;
    size_t tStart = 0;
    size_t tEnd = 0;

    while(finished == false)
    {

        tEnd = input.find(" ",tStart);
        std::string injection;
        injection.append(input,tStart,tEnd-tStart);
        elements.push_back(injection);
        tStart = tEnd+1;
        if(tEnd == input.npos)
            finished = true;
    }
    std::cout << "input: " << input << std::endl;
    for(auto &i : elements)
    {
        std::cout << "elements: " << i << std::endl;
    }

    if(elements[0] == "/name" || elements[0] == "/setname" || elements[0] == "/nick")
    {
        chatBox.addChat("Server: " + myProfile.name + " has changed their name to " + elements[1], goodColor);
        myProfile.name = elements[1];
        if(elements[1] == "Lithi" || elements[1] == "Biocava" || elements[1] == "Sneaky" || elements[1] == "SneakySnake")
            chatBox.addChat("Server: Ooo, Ooo, I like you!", warmColor);
        if(elements[1] == "Dehaku")
            chatBox.addChat("Server: Hey, that's my masters name!", warmColor);
        return true;
    }

    if(elements[0] == "/connect")
    {
        std::cout << "Connect chat command detected. \n";
        if(network::connectedServer != "")
        {
            chatBox.addChat("Server: Error, You're already connected to " + network::connectedServer, errorColor);
            return false;
        }
        if(myProfile.name == "Guest")
        {
            chatBox.addChat("Server: Error, please give yourself a name with /setname before attempting to connect.", errorColor);
            return false;
        }
        try
        {
            int test = std::stoi(elements[2]);
        }
        catch (std::exception &e)
        {
            chatBox.addChat("Command: /connect [IP Address] [Port]", errorColor);
            return false;
        }

        bool connectionBool = activateClient(elements[1], std::stoi(elements[2]));
        if(connectionBool)
        {
            std::cout << "Connected to server " << elements[1] << std::endl;
            network::client = true;
            chatBox.addChat("Server: Connected to " + elements[1] + "(" + elements[2] + ")", goodColor);
            return true;
        }
        else
        {
            std::cout << "Failed to connect to " << elements[1] << std::endl;
            chatBox.addChat("Server: FAILED to connect to " + elements[1] + "(" + elements[2] + ")", goodColor);
            return false;
        }
    }
    /*


    else if(elements[0] == "/setname")
    {
        chatBox.addChat("Server: " + network::name + " has changed their name to " + elements[1], goodColor);
        network::name = elements[1];
        if(elements[1] == "Lithi" || elements[1] == "Biocava" || elements[1] == "Sneaky" || elements[1] == "SneakySnake")
            chatBox.addChat("Server: Ooo, Ooo, I like you!", warmColor);
        if(elements[1] == "Argwm" || elements[1] == "Dehaku")
            chatBox.addChat("Server: Hey, that's my masters name!", warmColor);
        return true;
    }
    else if(elements[0] == "/repeat")
    {
        try
        {
            int test = std::stoi(elements[1]);
        }
        catch (std::exception &e)
        {
            chatBox.addChat("Invalid argument: " + elements[1] + " in command " + input, errorColor);
            chatBox.addChat("Command: /repeat [numberoftimes] [series of words or numbers]", errorColor);
            return false;
        }
        std::string repeatingLine;
        for(int i = 0; i != elements.size(); i++)
        {
            if(i != 0 && i != 1)
            {
                repeatingLine.append(elements[i] + " ");
            }
        }
        for(int i = 0; i != std::stoi(elements[1]); i++)
        {
            chatBox.addChat("Server: Repeating; " + repeatingLine, goodColor);
        }
        return true;
    }

    chatBox.addChat("Unrecognized command: " + input, errorColor);

    */
    return false;
}


void clientPacketManager::handlePackets()
{
    for(auto &currentPacket : packets)
    {


        sf::Packet &packet = currentPacket.packet;
        byteKeeper.byteInput(packet);


        sf::Uint8 type;
        packet >> type;
        //std::cout << "P: " << int(type) << std::endl;


        if(type == sf::Uint8(ident::textMessage))
        {
            std::string chatLine;
            packet >> chatLine;
            chatBox.addChat(chatLine);
        }

        if(type == sf::Uint8(ident::message))
        {
            std::string in;
            packet >> in;
            std::cout << "Server" << int(type) << ": \"" << in << "\"" << std::endl;
        }

        else if(type == sf::Uint8(ident::initialization))
        {
            std::cout << "Received initialization, ";
            int simCount;
            packet >> simCount;
            std::cout << simCount << " simulations." << std::endl;

            for(int i = 0; i != simCount; i++) // We know all the currently running sims, now we request them all individually.
            {
                sf::Packet sendPacket;
                sendPacket << sf::Uint8(ident::simulationRequest);

                // Take out the ID from the packet, and send it back with the request.
                sf::Uint32 simID;
                packet >> simID;
                sendPacket << simID;
                serverSocket.send(sendPacket);
                std::cout << "Requested Sim " << int(simID) << std::endl;
            }

            // TODO: Send/receive players connected here.

        }

        else if(type == sf::Uint8(ident::simulationInitialization))
        {
            Simulation sim;
            packet >> sim;
            std::cout << "Received Sim " << sim.simulationID;
            simulationManager.simulations.push_back(sim);
            std::cout << ", and inserted it. \n";
        }

        else if(type == sf::Uint8(ident::simulationUpdate ))
        {
            int simCount;
            packet >> simCount;
            for(int i = 0; i != simCount; i++)
            {
                int simID;
                int population;
                packet >> simID;
                packet >> population;
                Simulation* simPtr = simulationManager.getSimulation(simID);
                if(simPtr == nullptr)
                {
                    std::cout << "Attempted to update a non-existing simulation, Abandoning this packet. This is bad. \n";
                    break;
                }

                if(population != simPtr->organisms.size())
                    std::cout << "Population Desync! Packet: " << population << ", Us: " << simPtr->organisms.size() << std::endl;

                unsigned int counter = 0;
                for(auto &critter : simPtr->organisms)
                {
                    if(counter >= population)
                        break;

                    CritterPositions cPos;
                    packet >> cPos;

                    // Update Position
                    critter.get()->pos = cPos.position;

                    // If you've gotta brain, you gotta desire.
                    if(cPos.brainBool && critter.get()->brain.lock())
                        critter.get()->brain.lock()->desiredPos = cPos.desiredPosition;

                    counter++;
                }

                int plantPop;
                packet >> plantPop;
                if(plantPop != simPtr->flora.size())
                    std::cout << "Plant Pop Desync! Packet: " << population << ", Us: " << simPtr->flora.size() << std::endl;

                counter = 0;
                for(auto &plant : simPtr->flora)
                {
                    if(counter >= plantPop)
                        break;

                    CritterPositions cPos;
                    packet >> cPos;

                    // Update Position
                    plant.get()->pos = cPos.position;

                    counter++;
                }

            }
        }

        else if(type == sf::Uint8(ident::organismUpdate ))
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

        else if(type == sf::Uint8(ident::organismInitialization ))
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
            packet >> myProfile.ID;
            std::cout << int(myProfile.ID) << std::endl;



            std::cout << "Requesting initialization\n";
            sf::Packet requestPacket;
            requestPacket << sf::Uint8(ident::initialization) << myProfile.ID;
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


        if(type == sf::Uint8(ident::textMessage))
        {
            std::string chatLine;
            packet >> chatLine;
            chatBox.addChat(chatLine);

            sf::Packet returnPacket;
            returnPacket << sf::Uint8(ident::textMessage)
            << chatLine;
            sendToAllClients(returnPacket);
        }
        else if(type == sf::Uint8(ident::message))
        {
            std::string in;
            packet >> in;
            std::cout << "Client" << int(type) << ": \"" << in << "\"" << std::endl;
        }

        else if(type == sf::Uint8(ident::initialization))
        {
            std::cout << "Initialization 'Request' received from " << int(currentPacket.sender->id) << std::endl;
            sf::Packet sendPacket;

            // Send the same type back.
            sendPacket << type;
            sendPacket << sf::Uint32(simulationManager.simulations.size());

            for(auto &sim : simulationManager.simulations)
            {
                sendPacket << sf::Uint32(sim.simulationID);
                std::cout << "Sent " << sim.simulationID << std::endl;
            }

            // TODO: Send/receive players connected here.

            currentPacket.sender->socket->send(sendPacket);
        }

        else if(type == sf::Uint8(ident::simulationRequest))
        {
            std::cout << "Received request of simulation ";
            int simReqID;
            packet >> simReqID;
            std::cout << simReqID;

            Simulation* simPtr = simulationManager.getSimulation(simReqID);
            if(simPtr == nullptr)
            {
                std::cout << "Failed to find " << simReqID << std::endl;
                continue;
            }
            Simulation& sim = *simPtr;


            std::cout << "Sim(" << sim.simulationID << ")";

            std::cout << " has " << sim.organisms.size() << " creatures. \n";

            sf::Packet returnPacket;
            returnPacket << sf::Uint8(ident::simulationInitialization);
            returnPacket << sim;

            currentPacket.sender->socket->send(returnPacket);
        }

        else if(type == sf::Uint8(ident::organismInitialization ))
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
        else if(type == sf::Uint8(ident::floraInitialization) )
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

            returnPacket << sf::Uint8(ident::clientID) << currentPacket.sender->id;

            currentPacket.sender->socket->send(returnPacket);
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

void drawChat()
{

    int xDraw = 200-332;
    int yDraw = 200+637;


    // The Chatbox History
    {
        int yOffset = 0;


        //std::cout << "ChatSize: " << std::max(chatBox.chatStorage.size()-10,0) << std::endl;

        //for(int i = chatBox.chatStorage.size(); (i != 0 && i >= chatBox.chatStorage.size()-10); i--)
        for(int i = chatBox.chatStorage.size()-1; i >= chatBox.chatStorage.size()-10; i--)
        {

            sfe::RichText chatText(gvars::defaultFont);

            chatText.setPosition(xDraw,(yDraw-15)-(15*yOffset));
            //chatText.setPosition(300,300);
            chatText.setCharacterSize(15);

            chatText << chatBox.chatStorage[i].color
            << sf::Text::Regular << chatBox.chatStorage[i].line;

            shapes.createRichText(chatText, &gvars::hudView);
            yOffset++;

            if(i <= 0)
                break;
        }
    }








    // Our entered text

    sfe::RichText chatText(gvars::defaultFont);

    chatText.setPosition(xDraw,yDraw);
    chatText.setCharacterSize(15);

    chatText << sf::Color::White
    << sf::Text::Bold << myProfile.name << ": "
    << sf::Text::Regular << chatBox.chatString;

    // TODO: Enhance the chat to accept ^89 colors and such, ala Toribash.

    if(network::chatting)
    {
        int chatGrowth = chatText.getLocalBounds().width;
        shapes.createSquare(xDraw-10,yDraw,xDraw+1200,yDraw+22,sf::Color(0,0,0,100),0,sf::Color::Transparent, &gvars::hudView);

        shapes.createRichText(chatText, &gvars::hudView);
    }
}

void drawMainMenu()
{
    int mainButt = shapes.createImageButton(sf::Vector2f(500,500),texturemanager.getTexture("ArtevoMain.png"),"",0,&gvars::hudView);

    sf::Texture* hudButton = &texturemanager.getTexture("HUDTab.png");


    int startButt = shapes.createImageButton(sf::Vector2f(-33,843),*hudButton,"",0,&gvars::hudView);
    shapes.createText(-33-40,843-8,12,sf::Color::Black,"Start Game",&gvars::hudView);

    int multiplayerButt = shapes.createImageButton(sf::Vector2f(179,843),*hudButton,"",0,&gvars::hudView);
    shapes.createText(179-40,843-8,12,sf::Color::Black,"Multiplayer",&gvars::hudView);

    int profileButt = shapes.createImageButton(sf::Vector2f(391,843),*hudButton,"",0,&gvars::hudView);
    shapes.createText(391-40,843-8,12,sf::Color::Black,"Profile",&gvars::hudView);

    int optionsButt = shapes.createImageButton(sf::Vector2f(606,843),*hudButton,"",0,&gvars::hudView);
    shapes.createText(606-40,843-8,12,sf::Color::Black,"Options",&gvars::hudView);

    int creditsButt = shapes.createImageButton(sf::Vector2f(818,843),*hudButton,"",0,&gvars::hudView);
    shapes.createText(818-40,843-8,12,sf::Color::Black,"Credits",&gvars::hudView);

    int exitButt = shapes.createImageButton(sf::Vector2f(1030,843),*hudButton,"",0,&gvars::hudView);
    shapes.createText(1030-40,843-8,12,sf::Color::Black,"Quit",&gvars::hudView);


    if(shapes.shapeClicked(startButt))
    {
        stateTracker.currentState = stateTracker.mainLoop;
    }
    if(shapes.shapeClicked(multiplayerButt))
    {
        stateTracker.currentState = stateTracker.multiplayer;
    }
    if(shapes.shapeClicked(profileButt))
    {
        stateTracker.currentState = stateTracker.profile;
    }
    if(shapes.shapeClicked(optionsButt))
    {
        stateTracker.currentState = stateTracker.options;
    }
    if(shapes.shapeClicked(creditsButt))
    {
        stateTracker.currentState = stateTracker.credits;
    }
    if(shapes.shapeClicked(exitButt))
    {
        saveProfile("Sopheroph");
        window.close();
    }



    int posY = 843;
    if(shapes.shapeHovered(startButt))
    {
        int posX = -33;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(multiplayerButt))
    {
        int posX = 179;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(profileButt))
    {
        int posX = 391;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);

        if(inputState.rmbTime == 1 && simulationManager.simulations.size() > 0)
            simulationManager.simulations.pop_back();

    }
    if(shapes.shapeHovered(optionsButt))
    {
        int posX = 606;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(creditsButt))
    {
        int posX = 818;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }
    if(shapes.shapeHovered(exitButt))
    {
        int posX = 1030;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }



}

void drawSubMain()
{

    sf::Texture* hudButton = &texturemanager.getTexture("HUDTab.png");


    int mainButt = shapes.createImageButton(sf::Vector2f(-33,170),*hudButton,"",0,&gvars::hudView);
    shapes.createText(-33-40,170-8,12,sf::Color::Black,"Main Menu",&gvars::hudView);
    if(shapes.shapeClicked(mainButt))
    {
        stateTracker.currentState = stateTracker.mainMenu;
    }
    if(shapes.shapeHovered(mainButt))
    {
        int posX = -33;
        int posY = 170;
        shapes.createSquare(posX-105,posY-15,posX+106,posY+15,sf::Color(255,255,255,150),0,sf::Color::White,&gvars::hudView);
    }


    if(stateTracker.currentState == stateTracker.multiplayer)
    {
        shapes.createText(500,150,20,sf::Color::Cyan,"Multiplayer",&gvars::hudView);
    }
    if(stateTracker.currentState == stateTracker.profile)
    {
        shapes.createText(500,150,20,sf::Color::Cyan,"Profile",&gvars::hudView);

        shapes.createText(300,200,12,sf::Color::Cyan,"Name: " + myProfile.name,&gvars::hudView);
        shapes.createText(300,215,10,sf::Color::Cyan,"You can change your name while playing by pressing Enter, then /setname yourname",&gvars::hudView);
        shapes.createText(300,230,12,sf::Color::Cyan,"Credits: " + std::to_string(myProfile.credits),&gvars::hudView);
        shapes.createText(300,245,12,sf::Color::Cyan,"Pix: " + std::to_string(myProfile.pix),&gvars::hudView);
    }
    if(stateTracker.currentState == stateTracker.options)
    {
        shapes.createText(500,150,20,sf::Color::Cyan,"Options",&gvars::hudView);

        sf::Vector2f textPos(500,200);
        sf::Vector2f buttonPos;


        buttonPos = textPos;
        shapes.createText(textPos,10,sf::Color::White,"Resolution: "
                            + std::to_string(resolution.resolutions[resolution.currentRes].width)
                            +"/"+std::to_string(resolution.resolutions[resolution.currentRes].height)
                            +": "+std::to_string(resolution.resolutions[resolution.currentRes].bitsPerPixel)
                            +"\n \n \nFullscreen: " + str(resolution.fullscreen)
                            ,&gvars::hudView);
        shapes.shapes.back().layer = 15075;

        buttonPos.x += 90;
        buttonPos.y += 20;
        int decreaseResolution = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",-90,&gvars::hudView);
        shapes.shapes.back().layer = 15050;
        buttonPos.x += 30;
        int increaseResolution = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",90,&gvars::hudView);
        shapes.shapes.back().layer = 15050;
        buttonPos.x += 30;
        int applyResolution = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",180,&gvars::hudView);
        shapes.shapes.back().layer = 15050;

        buttonPos.y += 20;
        int toggleFullscreen = shapes.createImageButton(buttonPos,texturemanager.getTexture("ArrowButton.png"),"",180,&gvars::hudView);
        shapes.shapes.back().layer = 15050;




        // Warning, the increase and decrease resolution buttons are backwards, since the videomode put the highest res in first, and the smallest last.
        if(shapes.shapeHovered(increaseResolution) && (inputState.lmbTime == 1))
            if(resolution.currentRes > 0)
                resolution.currentRes--;
        if(shapes.shapeHovered(decreaseResolution) && (inputState.lmbTime == 1))
            if(resolution.currentRes < resolution.resolutions.size()-1)
                resolution.currentRes++;

        if(shapes.shapeHovered(toggleFullscreen))
        {
            shapes.createText(gvars::mousePos,9,sf::Color::White,"  Toggle fullscreen");
            shapes.shapes.back().layer = 15075;
            if((inputState.lmbTime == 1))
                toggle(resolution.fullscreen);
        }


        if(shapes.shapeHovered(applyResolution))
        {
            shapes.createText(gvars::mousePos,9,sf::Color::White,"  Apply the Resolution!",&gvars::hudView);
            shapes.shapes.back().layer = 15075;

            if(inputState.lmbTime == 1)
            {
                if(resolution.fullscreen)
                    window.create(resolution.resolutions[resolution.currentRes], randomWindowName(),sf::Style::Fullscreen);
                else
                    window.create(resolution.resolutions[resolution.currentRes], randomWindowName());
            }

        }



    }
    if(stateTracker.currentState == stateTracker.credits)
    {
        shapes.createText(500,150,20,sf::Color::Cyan,"Credits",&gvars::hudView);
    }




}

void renderGame()
{
    if(inputState.key[Key::Space].time == 1)
    {
        gvars::currentx = 20;
        gvars::currenty = 20;
    }

    if(stateTracker.currentState == stateTracker.mainMenu)
    {
        drawMainMenu();
    }
    else if(stateTracker.currentState == stateTracker.mainLoop)
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

        drawHUD();
        drawFPSandData();
        drawChat();
    }
    else
    {
        drawSubMain();
    }

}

sf::Thread serverListenThread(&serverListen);
sf::Thread clientListenThread(&clientListen);

void sendLifeUpdate()
{
    // TODO: Maybe: Perhaps have the clients request updates, rather than constantly pinging them with it.
    std::cout << "Sending Life Update! \n";
    sf::Packet packet;

    // Labeling the type of packet.
    packet << sf::Uint8(ident::simulationUpdate);

    // Sending amount of simulations.
    packet << sf::Uint32(simulationManager.simulations.size());

    // Sending the simulations infos...
    for(auto &sim : simulationManager.simulations)
    {
        // First, Sim ID
        packet << sf::Uint32(sim.simulationID);

        // Amount of organisms.
        packet << sf::Uint32(sim.organisms.size());

        // Then the Organism updates.
        for(auto &critter : sim.organisms)
        {
            CritterPositions CPos(*critter.get());
            packet << CPos;
        }

        // Amount of Plants
        packet << sf::Uint32(sim.flora.size());
        for(auto &plant : sim.flora)
        {
            CritterPositions CPos(*plant.get());
            packet << CPos;
        }

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
        activateClient("127.0.0.1",23636);
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

    if(inputState.key[Key::L].time == 1)
    {
        for(auto &sim : simulationManager.simulations)
        {
            for(auto &plant : sim.flora)
            {
                plant.get()->pos.x = random(1,1000);
                plant.get()->pos.y = random(1,1000);
            }
        }
    }

    for(auto &sim : simulationManager.simulations)
        runBrains(sim.organisms);
    //runBrains(organisms);

    if(inputState.key[Key::C].time == 1)
        addCreatures(100);


    if((globalCycle % 60) == 0)
    {

    }

}
