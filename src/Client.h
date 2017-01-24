#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "Networking.h"

extern int clientNum;


void clientSendingPing();

void clientListen();

void activateClient();

#endif // CLIENT_H_INCLUDED
