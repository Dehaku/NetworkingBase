#include "Networking.h"
#include "globalvars.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>



namespace network
{
    int mainPort = 23636;
    bool packetDeletion = false;
    bool servWait = false;
    bool cliWait = false;
    bool server = false;
    bool client = false;
    bool chatting = false;
    bool needTime = false;
    bool givingTime = false;
    bool listening = false;
    std::string name = "";
    std::string connectedServer = "";
    sf::Mutex clientHandling;
    sf::Mutex packetManagerHandling;
}


















