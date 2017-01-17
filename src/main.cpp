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

    gameSetup();

}

int main()
{
    // Initial
    setup();

	// Start the game loop
    while (window.isOpen())
    {
        // Basic Program Components
        handleEvents();
        inputState.update();

        // Program Specific Components
        runGame();



        // Networking




        // Graphics
        renderGame();

        window.clear();
        shapes.drawShapes();
        AnyDeletes(shapes.shapes);
        window.display();
    }

    return EXIT_SUCCESS;
}
