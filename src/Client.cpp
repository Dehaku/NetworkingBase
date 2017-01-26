#include "Client.h"

int clientNum = 0;
sf::Uint8 myID = 0;

sf::IpAddress IPAddress;
unsigned short serverPort;
sf::TcpSocket socket;
sf::SocketSelector serverHolder;


clientPacketManager cPM;

void clientSendingPing()
{
    sf::Packet packet;
    std::string out = "Hi, I'm a client";
    packet << sf::Uint8(ident::message) << out;

    if (socket.send(packet) != sf::Socket::Done)
        return;

    std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
}


void sendServerMyInfo()
{
    sf::Packet packet;
    packet << sf::Uint8(ident::clientID) << myID;

    if (socket.send(packet) != sf::Socket::Done)
        return;

    std::cout << "Sent Server My ID: \"" << int(myID) << "\"" << std::endl;
}

void clientListen()
{
    // Receive a message from the server

    while(serverHolder.wait())
    {
        if(serverHolder.isReady(socket))
        {
            BoolPacket packet;

            sf::Socket::Status status = socket.receive(packet.packet);

            if (status == sf::Socket::Disconnected)
                std::cout << "We received a disconnect somehow. \n";
            if (status == sf::Socket::Error)
                std::cout << "RECIEVE ERROR, PANIC. \n";


            if (status != sf::Socket::Done)
                continue;

            { // Storing Packet in Manager for external use.
                sf::Lock lock(network::packetManagerHandling);
                cPM.packets.push_back(packet);
            }
        }
    }

    network::listening = false;
}

void activateClient()
{
    IPAddress = "127.0.0.1";
    serverPort = 23636;
    sf::Socket::Status status = socket.connect(IPAddress, serverPort);
    if (status != sf::Socket::Done)
        std::cout << "Failed to connect to... \n" << IPAddress << ":" << serverPort << std::endl;
    if (status == sf::Socket::Done)
    {
        std::cout << "Probably connected!";
        // socket.setBlocking(false);
    }

    sendServerMyInfo();

    serverHolder.add(socket);

}
