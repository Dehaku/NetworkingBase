#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Networking.h"

extern int serverNum;

class ClientPackage
{
public:
    std::string userName;
    sf::TcpSocket* socket;
    bool toDelete;
    ClientPackage();
    ~ClientPackage();
};

void serverPingAll();
void serverListen();
void activateServer();
void deactivateServer();
int clientCount();

#endif // SERVER_H_INCLUDED
