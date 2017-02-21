#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "Networking.h"
#include <vector>
#include "util.h"

extern int clientNum;
extern sf::TcpSocket serverSocket;

class clientPacketManager
{
public:
    std::vector<BoolPacket> packets;
    void handlePackets();
};
extern clientPacketManager cPM;

void clientSendingPing();

void clientListen();

bool activateClient(sf::IpAddress servIP = "127.0.0.1", unsigned short servPort = 23636);

#endif // CLIENT_H_INCLUDED
