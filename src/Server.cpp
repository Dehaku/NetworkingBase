#include "Server.h"

sf::TcpListener listener;
unsigned short port;


int serverNum = 0;



void serverListen()
{
    if (listener.listen(port) != sf::Socket::Done)
        std::cout << "Unable to listen to port: " + port << std::endl;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;
    sf::TcpSocket client;
    //client.setBlocking(false);
    //listener.setBlocking(false);
    if (listener.accept(client) != sf::Socket::Done)
        std::cout << "Listener had an error while listening for a client. \n";
    std::cout << "Client connected: " << client.getRemoteAddress() << std::endl;

}

void runServer()
{

}

void activateServer()
{
    port = 23636;
    serverListen();
}

void deactivateServer()
{
    listener.close();
}
