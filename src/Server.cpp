#include "Server.h"

sf::SocketSelector selector;
sf::TcpListener listener;
unsigned short port;

ClientPackage::ClientPackage()
{
    socket = nullptr;
    toDelete = false;
}

std::list<sf::TcpSocket*> clients;
std::list<ClientPackage> clients2;

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


void serverPingAll()
{
    sf::Packet packet;
    std::string out = "Hi, I'm the server";
    packet << sf::Uint8(ident::message) << out;

    for(auto &client : clients)
        client->send(packet);

    std::cout << "Sent: '" << out << "' to all. \n";
}

void serverListen()
{
    // https://www.sfml-dev.org/tutorials/2.4/network-packet.php
    if(selector.wait())
    {
        if(selector.isReady(listener))
        {
            std::cout << "Listener Ready! \n";
            sf::TcpSocket* client = new sf::TcpSocket;
            if(listener.accept(*client) == sf::Socket::Done)
            {
                std::cout << "New Client! Adding to list...";
                clients.push_back(client);
                std::cout << " and selector...";
                selector.add(*client);
                std::cout << "Done! \n";

            }
            else
            {
                std::cout << "Failed to make new client. \n";
                delete client;
            }

        }
        else
        {
            for(auto &client : clients)
            {
                if(selector.isReady(*client))
                {
                    char in[128];
                    std::size_t received;
                    sf::Socket::Status status = client->receive(in, sizeof(in), received);

                    if(status == sf::Socket::Disconnected)
                    {
                        std::cout << "Client disconnected! \n";
                        continue;
                    }

                    if(status != sf::Socket::Done)
                        continue;

                    //if (client->receive(in, sizeof(in), received) != sf::Socket::Done)

                    std::cout << "Client: \"" << in << "\"" << std::endl;

                }
            }
        }
    }
    network::listening = false;
}

void runServer()
{

}

void activateServer()
{
    port = 23636;

    if (listener.listen(port) != sf::Socket::Done)
        std::cout << "Unable to listen to port: " + port << std::endl;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

    selector.add(listener);
}

void deactivateServer()
{
    listener.close();
}
