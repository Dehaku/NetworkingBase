#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include "globalvars.h"
#include "defs.h"
//#include "Networking.h"
#include "InputState.h"
#include "util.h"

#include "math.h"
#include "Shapes.h"

#include "Tiles.h"
#include "Game.h"

sf::RenderWindow window(sf::VideoMode(960, 720), "Networking the Networth");

// https://github.com/SFML/SFML/wiki/Source:-Network-Chat-Example
// https://forums.tigsource.com/index.php?topic=25048.0
// https://github.com/SFML/SFML/tree/master/examples/sockets

class GameInfo
{
public:
    sf::Vector2f vPos;
    bool isRotating;
    float rotation;
    bool isChatting;

    GameInfo()
    {
        rotation = 0;
        isRotating = false;
        isChatting = false;
    }

    void moveSquare();


};
GameInfo gameInfo;

void GameInfo::moveSquare()
{

    if(inputState.key[Key::Return].time == 1)
    {
        std::cout << "Entering!";
        toggle(isChatting);
    }

    // Ensure they're not typing, otherwise things go bonkers.
    if(isChatting)
        return;

    if(isRotating)
        rotation++;
    if(inputState.key[Key::A])
        vPos.x--;
    if(inputState.key[Key::D])
        vPos.x++;
    if(inputState.key[Key::W])
        vPos.y--;
    if(inputState.key[Key::S])
        vPos.y++;
    if(inputState.key[Key::R].time == 1)
        toggle(isRotating);



}

void handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        inputState.updateFromEvent(event);
        if (event.type == sf::Event::Closed)
        {
            deactivateServer();
            window.close();
        }
        if (event.type == sf::Event::MouseWheelMoved)
        {
            if (event.mouseWheel.delta > 0)
            {

                //if(menus.empty())
                {
                    std::cout << "Zoom Out \n";
                    if (gvars::cameraZoom < 2)
                    {
                        gvars::cameraZoom = gvars::cameraZoom / 0.5;
                        //CameraSize
                        gvars::view1.zoom(0.5);
                    }
                }

                /*
                else
                    for(auto &menu : menus)
                    {
                        menu.scrollOne += event.mouseWheel.delta;
                        if(menu.scrollOne > 0)
                            menu.scrollOne = 0;
                    }

                    */


            }
            if (event.mouseWheel.delta < 0)
            {
                //if(menus.empty())
                {
                   std::cout << "Zoom In \n";
                    if (gvars::cameraZoom > 0.5)
                    {
                        gvars::cameraZoom = gvars::cameraZoom / 2;
                        gvars::view1.zoom(2);
                    }
                }

                /*
                else
                    for(auto &menu : menus)
                        menu.scrollOne += event.mouseWheel.delta;

                        */

            }

            //Develop the scaling camera, Probably using .setsize instead of zoom.
            //http://www.sfml-dev.org/tutorials/2.0/graphics-view.php
        }

        if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            //sf::FloatRect visibleArea(0, 0, Event.size.width, Event.size.height);
            //View1.setViewport(visibleArea);
            //App.setView(sf::View(visibleArea));
        }

        if (event.type == sf::Event::LostFocus)
        {
            gvars::inFocus = false;
        }
        if (event.type == sf::Event::GainedFocus)
        {
            gvars::inFocus = true;
        }
        if (event.type == sf::Event::TextEntered)
        {
            /*

            if (event.text.unicode < 128 && network::chatting == true) //
            {
                if(event.text.unicode != 8 && event.text.unicode != 13) // 8 = backspace 13 = enter, Thanks to http://www.fileformat.info
                {
                    std::string TempStr;
                    TempStr = event.text.unicode;
                    cliCon.chatString.append( TempStr );
                }
                if(event.text.unicode == 8 && cliCon.chatString.size() != 0)
                {
                    cliCon.chatString.erase(cliCon.chatString.end()-1);
                }
                if(event.text.unicode == 13 && network::client == true)
                {
                    network::chatting = false;
                    sf::UdpSocket socket;
                    sf::Packet ToSend;
                    std::string SendText;
                    SendText.append(cliCon.name);
                    SendText.append(": ");
                    SendText.append(cliCon.chatString);

                    ToSend << ident.textMessage << SendText;
                    if(ToSend.getDataSize() != 0)
                    {

                        sf::Packet sendpacket;

                        sendpacket << ident.textMessage << SendText;

                        // Send an answer to the server
                        if(cliSocket.send(sendpacket) != sf::Socket::Done)
                        {
                            std::cout << "!= Socket Done (In client sending chat), Apparently! Tell the Devs! \n";
                        }


                        cliCon.chatString.clear();
                    }
                }

                if(event.text.unicode == 13 && network::server == true)
                {
                    network::chatting = false;
                    sf::Packet ToSend;
                    std::string SendText;
                    SendText.append("*Server*");
                    SendText.append(": ");
                    SendText.append(cliCon.chatString);

                    ToSend << ident.textMessage << SendText;
                    if(ToSend.getDataSize() != 0)
                    {

                        for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                        {
                            if(gvars::debug) std::cout << "Running through clients \n";
                            sf::TcpSocket& client = **it;
                            client.send(ToSend);
                        }
                        cliCon.chatString.clear();
                    }
                }
                if(event.text.unicode == 13)
                {
                    network::chatting = false;
                    std::string firstLetter;
                    firstLetter.append(cliCon.chatString,0,1);
                    std::cout << "(" << firstLetter << ")\n";
                    if(firstLetter == "/")
                        chatCommand(cliCon.chatString);
                    else if(firstLetter != "/" && network::connectedServer != "")
                    {
                        sf::Packet pack;
                        pack << ident.textMessage << network::name + ": " + cliCon.chatString;
                        cliSocket.send(pack);
                    }
                    //else
                        //chatBox.addChat(cliCon.chatString,sf::Color::White);
                    cliCon.chatString.clear();
                }

            }
            else if(event.text.unicode == 13 && network::chatting == false)
            {
                network::chatting = true;
            }

            */

        }

    }
}








void setup()
{
    // Font
    if (!gvars::defaultFont.loadFromFile("data/fonts/Xolonium-Regular.otf"))
        throw std::runtime_error("Failed to load font!");

    // View
    gvars::view1.setSize(1000, 1000);
    window.setView(gvars::view1);



    window.setSize(sf::Vector2u(200,100));
    gameSetup();


}

int main()
{
    // Initial
    setup();

    // Delta Timestep
    double t = 0.0;
    double dt = 0.01; // const
    double currentTime = fpsKeeper.startTime.getElapsedTime().asSeconds();
    double accumulator = 0.0;

	// Start the game loop
    while (window.isOpen())
    {
        // Basic Program Components
        handleEvents();
        inputState.update();

        if(inputState.key[Key::Left].time == 1)
        {
            if(dt < 0.00002)
                dt += 0.000001;
            else if(dt < 0.0002)
                dt += 0.00001;
            else if(dt < 0.002)
                dt += 0.0001;
            else if(dt < 0.02)
                dt += 0.001;
        }
        if(inputState.key[Key::Right].time == 1)
        {
            if(dt > 0.002)
                dt -= 0.001;
            else if(dt > 0.0002)
                dt -= 0.0001;
            else if(dt > 0.00002)
                dt -= 0.00001;
        }
        if(inputState.key[Key::Down].time == 1 || inputState.key[Key::Up].time == 1 )
            dt = 0.01;

        //std::cout << "DT: " << dt << std::endl;


        // Program Specific Components

        if(inputState.key[Key::Return].time == 1)
            simulationInitialization();

        double newTime = fpsKeeper.startTime.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        int runAmount = 0;

        while ( accumulator >= dt )
        {
            // Functions Start.
            runGame();

            // Functions end, Upkeep begins.
            if(fpsKeeper.framesPerSecond <= 1)
                dt = 0.01; // A nice little failsafe to help the player.
            fpsKeeper.updatesPassed++;
            runAmount++;
            accumulator -= dt;
            t += dt;

            // To hopefully prevent massive jumps if the system lags for a bit.
            if(accumulator > (dt*10000))
                accumulator = dt*10000;
        }

        //std::cout << "runAmount: " << runAmount << std::endl;

        // Networking
        runServerStuffs();



        // Graphics
        renderGame();

        shapes.drawShapes();
        AnyDeletes(shapes.shapes);
        window.display();
        window.clear();
    }

    return EXIT_SUCCESS;
}
