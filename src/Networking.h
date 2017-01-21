#ifndef NETWORKING_H_INCLUDED
#define NETWORKING_H_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include "util.h"


#include "Server.h"
#include "Client.h"


namespace network
{
    extern int mainPort;
    extern bool packetDeletion;
    extern bool servWait;
    extern bool cliWait;
    extern bool server;
    extern bool client;
    extern bool chatting;
    extern bool needTime;
    extern bool givingTime;
    extern std::string name;
    extern std::string connectedServer;
}


class BoolPacket
{
    public:
    sf::Packet packet;
    bool toDelete;
    BoolPacket();
};
extern std::vector<BoolPacket> packetContainer;


#endif // NETWORKING_H_INCLUDED
