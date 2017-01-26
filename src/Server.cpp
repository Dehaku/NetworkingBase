#include "Server.h"

sf::SocketSelector selector;
sf::TcpListener listener;
unsigned short port;
int clientIDCounter = 1;



serverPacketManager sPM;




ClientPackage::ClientPackage()
{
    id = 0;
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

    while(selector.wait())
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
                    client.id = clientIDCounter++;
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
                    BoolPacket packet;
                    packet.sender = &client; // So the server knows who to reply to when reading packets.

                    sf::Socket::Status status = client.socket->receive(packet.packet);

                    if(client.toDelete == false && status == sf::Socket::Disconnected)
                    {
                        std::cout << "Client disconnected! \n";
                        selector.remove(*client.socket);
                        delete client.socket;
                        client.toDelete = true;
                        continue;
                    }

                    if (status == sf::Socket::Error)
                        std::cout << "RECIEVE ERROR, PANIC. \n";
                    if (status == sf::Socket::NotReady)
                        std::cout << "Not Ready! \n";
                    if (status == sf::Socket::Partial)
                        std::cout << "P";



                    if(status == sf::Socket::Done)
                    { // Storing Packet in Manager for external use.
                        sf::Lock lock(network::packetManagerHandling);
                        sPM.packets.push_back(packet);
                    }
                }
            }
        }
    }
    if(sPM.packets.empty()) // To avoid possible nullptr references when a client vanishes.
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
