#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Text.h"
#include "globalvars.h"
#include "defs.h"
//#include "Networking.h"
#include "InputState.h"
#include "util.h"

#include "math.h"
#include "Shapes.h"

#include "Tiles.h"
#include "Game.h"

#include "Camera.h"
#include "SaveLoad.h"

sf::RenderWindow window(sf::VideoMode(960, 720), randomWindowName());

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








void setup()
{


    for(int i = 0; i != 10; i++)
    {
        sf::Color color(100+random(1,100),100+random(1,100),100+random(1,100));
        chatBox.addChat(" ",color);
    }

    // Font
    if (!gvars::defaultFont.loadFromFile("data/fonts/Xolonium-Regular.otf"))
        throw std::runtime_error("Failed to load font!");

    // View
    gvars::hudView.setSize(1280,720); // blargle.

    std::cout << "Size: " << gvars::hudView.getSize().x << "/" << gvars::hudView.getSize().y << std::endl;

    gvars::view1.setSize(1280, 720);
    window.setView(gvars::view1);
    gvars::view1.setCenter(1280/2,720/2);





    /* 16:9 Cheatsheet
    128 	72
    256 	144
    384 	216
    512 	288
    640 	360
    768 	432
    896 	504
    1024 	576
    1152 	648
    1280 	720 // Bing!
    1408 	792
    1536 	864
    // Apparently 1600:900 isn't divisable by 8, so I guess it's not a super good resolution? My monitor is 1600:900 by default though, hmm.
    1664 	936
    ...
    3840 	2160 // 4k apparently.

    */

    window.setSize(sf::Vector2u(640,360));
    gameSetup();


}



void mousePosTracker()
{
    gvars::mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), gvars::view1);
}



void handleEvents()
{
    mousePosTracker();

    sf::Event event;
    while (window.pollEvent(event))
    {
        inputState.updateFromEvent(event);

        if (event.type == sf::Event::Closed)
        {
            deactivateServer();
            saveProfile("Sopheroph");
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
            // std::cout << "Pressed: " << event.text.unicode << std::endl;

            if (event.text.unicode < 128 && network::chatting == true) //
            {
                if(event.text.unicode == 22)
                {
                    chatBox.chatString.append(GetClipboardText());
                    continue;
                }

                if(event.text.unicode != 8 && event.text.unicode != 13) // 8 = backspace 13 = enter, Thanks to http://www.fileformat.info
                {
                    std::string TempStr;
                    TempStr = event.text.unicode;
                    chatBox.chatString.append( TempStr );
                }
                if(event.text.unicode == 8 && chatBox.chatString.size() != 0)
                {
                    chatBox.chatString.erase(chatBox.chatString.end()-1);
                }
                if(event.text.unicode == 13 && chatBox.chatString != "")
                {
                    myProfile.enteredStrings.push_back(chatBox.chatString);

                    network::chatting = false;
                    std::string firstLetter;
                    firstLetter.append(chatBox.chatString,0,1);
                    std::cout << "(" << firstLetter << ")\n";
                    if(firstLetter == "/")
                    {
                        chatCommand(chatBox.chatString);
                        chatBox.chatString.clear();

                    }

                }

                if(event.text.unicode == 13 && network::client == true)
                {
                    network::chatting = false;
                    if(chatBox.chatString.empty())
                        continue;

                    sf::UdpSocket socket;
                    sf::Packet ToSend;
                    std::string SendText;
                    SendText.append(myProfile.name);
                    SendText.append(": ");
                    SendText.append(chatBox.chatString);

                    ToSend << sf::Uint8(ident::textMessage) << SendText;
                    if(ToSend.getDataSize() != 0)
                    {

                        sf::Packet sendpacket;

                        sendpacket << sf::Uint8(ident::textMessage) << SendText;
                        if(serverSocket.send(sendpacket) != sf::Socket::Done)
                        {
                            std::cout << "!= Socket Done (In client sending chat), Apparently! Tell the Devs! \n";
                        }


                        chatBox.chatString.clear();
                    }
                }

                if(event.text.unicode == 13 && network::server == true)
                {

                    network::chatting = false;
                    if(chatBox.chatString.empty())
                        continue;

                    sf::Packet ToSend;
                    std::string SendText;
                    SendText.append("*Server*");
                    SendText.append(": ");
                    SendText.append(chatBox.chatString);

                    ToSend << sf::Uint8(ident::textMessage) << SendText;
                    if(ToSend.getDataSize() != 0)
                    {
                        chatBox.addChat(SendText);
                        sendToAllClients(ToSend);

                        chatBox.chatString.clear();
                    }
                }

            }
            else if(event.text.unicode == 13 && network::chatting == false)
            {
                network::chatting = true;
            }

        }

    }
}


int main()
{
    // Initial
    setup();
    loadProfile("Sopheroph");


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
        cameraControls();

        if(inputState.key[Key::Return].time == 1 || inputState.key[Key::T].time == 1)
        {
            //network::chatting = true;
        }

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

        if(inputState.key[Key::RShift].time == 1)
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
            fpsKeeper.updatesPassedTotal++;
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


        fpsKeeper.calcFPS();
        byteKeeper.calcBytes();
        // Graphics
        renderGame();



        shapes.drawShapes();
        AnyDeletes(shapes.shapes);



        window.display();
        window.clear();
    }

    return EXIT_SUCCESS;
}
