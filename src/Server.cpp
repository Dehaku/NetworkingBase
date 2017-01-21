#include "Server.h"

sf::TcpListener listener;
unsigned short port;


int serverNum = 0;


void exchangeHellos(sf::TcpSocket &socket)
{
    // Send a message to the connected client
    const char out[] = "Hi, I'm the server";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;

    // Receive a message back from the client
    char in[128];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return;
    std::cout << "Answer received from the client: \"" << in << "\"" << std::endl;
}

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

    exchangeHellos(client);

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
