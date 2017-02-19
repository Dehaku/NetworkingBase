#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Networking.h"

extern int serverNum;

class serverPacketManager
{
public:
    std::vector<BoolPacket> packets;
    void handlePackets();
};
extern serverPacketManager sPM;

class ClientPackage
{
public:
    std::string userName;
    sf::Uint8 id;
    sf::Uint32 credits;
    sf::Uint32 pix;









    sf::TcpSocket* socket;
    bool toDelete;
    ClientPackage();
    ~ClientPackage();
};
extern std::list<ClientPackage> clients;
void sendToAllClients(sf::Packet packet);

void serverPingAll();
void serverListen();
void activateServer();
void deactivateServer();
int clientCount();

#endif // SERVER_H_INCLUDED
