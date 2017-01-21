#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "Networking.h"

extern int serverNum;

void serverListen();
void activateServer();
void deactivateServer();

#endif // SERVER_H_INCLUDED
