#include "Server.h"

sf::SocketSelector selector;
sf::TcpListener listener;
unsigned short port;



void serverPacketManager::handlePackets()
{
    if(packets.size() > 0)
        std::cout << "Packets: " << packets.size() << std::endl;
    for(auto &boolPacket : packets)
    {
        sf::Packet &packet = boolPacket.packet;

        std::string in;
        sf::Uint8 type;
        packet >> type >> in;

        std::cout << "Client" << int(type) << ": \"" << in << "\"" << std::endl;
    }
    packets.clear();
}
serverPacketManager sPM;




ClientPackage::ClientPackage()
{
    socket = nullptr;
    toDelete = false;
}
ClientPackage::~ClientPackage()
{
    //if(socket != nullptr)
        //delete socket;
}


std::list<ClientPackage> clients;

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
        client.socket->send(packet);

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
            ClientPackage client;
            client.socket = new sf::TcpSocket;

            //sf::TcpSocket* client = new sf::TcpSocket;
            if(listener.accept(*client.socket) == sf::Socket::Done)
            {
                std::cout << "New Client! Adding to list...";
                {
                    // sf::Lock lock(network::clientHandling);
                    clients.push_back(client);
                }


                std::cout << " and selector...";
                selector.add(*client.socket);
                std::cout << "Done! \n";

            }
            else
            {
                std::cout << "Failed to make new client. \n";
                delete client.socket;
            }

        }
        else
        {
            // sf::Lock lock(network::clientHandling);
            for(auto &client : clients)
            {
                if(selector.isReady(*client.socket))
                {
                    char in[128];
                    std::size_t received;

                    BoolPacket packet;

                    sf::Socket::Status status = client.socket->receive(packet.packet);

                    if(client.toDelete == false && status == sf::Socket::Disconnected)
                    {
                        std::cout << "Client disconnected! \n";
                        selector.remove(*client.socket);
                        delete client.socket;
                        client.toDelete = true;
                        continue;
                    }

                    if(status != sf::Socket::Done)
                        continue;

                    { // Storing Packet in Manager for external use.
                        sf::Lock lock(network::packetManagerHandling);
                        sPM.packets.push_back(packet);
                    }
                }
            }
        }
    }

    AnyDeletes(clients);
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

int clientCount()
{
    return clients.size();
}
