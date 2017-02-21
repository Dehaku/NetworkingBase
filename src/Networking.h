#ifndef NETWORKING_H_INCLUDED
#define NETWORKING_H_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include "util.h"


#include "Server.h"
#include "Client.h"



class Profile
{
public:
    std::string name;
    sf::Uint8 ID;
    sf::Uint32 credits;
    sf::Uint32 pix;

    Profile();
};
extern Profile myProfile;

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
    extern bool listening;
    extern std::string name;
    extern std::string connectedServer;
    extern sf::Mutex clientHandling;
    extern sf::Mutex packetManagerHandling;
}

namespace ident
{
    enum ident
    {

        connectionAttempt,
        connectionSuccession,
        textMessage,
        initialization,
        simulationRequest,
        simulationInitialization,
        simulationUpdate,
        message,
        organismInitialization,
        clientID,
        organismUpdate,
        floraInitialization,
        floraUpdate,
        disconnecting
    };
}





#endif // NETWORKING_H_INCLUDED
