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
    std::string name = "";
    std::string connectedServer = "";
}




BoolPacket::BoolPacket()
{
    toDelete = false;
}
std::vector<BoolPacket> packetContainer;













