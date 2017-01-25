#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "Networking.h"
#include <vector>
#include "util.h"

extern int clientNum;

class clientPacketManager
{
public:
    std::vector<BoolPacket> packets;
    void handlePackets();
};
extern clientPacketManager cPM;

void clientSendingPing();

void clientListen();

void activateClient();

#endif // CLIENT_H_INCLUDED
