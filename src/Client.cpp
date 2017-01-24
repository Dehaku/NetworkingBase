#include "Client.h"

int clientNum = 0;

sf::IpAddress IPAddress;
unsigned short serverPort;
sf::TcpSocket socket;

void clientPacketManager::handlePackets()
{
    std::cout << "Packets: " << packets.size() << std::endl;
}

void clientSendingPing()
{
    const char out[] = "Hi, I'm a client";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
}

void exchangeHellos()
{
    // Receive a message from the server
    char in[128];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return;
    std::cout << "Message received from the server: \"" << in << "\"" << std::endl;

    // Send an answer to the server
    const char out[] = "Hi, I'm a client";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
}

void clientListen()
{
    // Receive a message from the server
    sf::Packet packet;
    if (socket.receive(packet) != sf::Socket::Done)
        return;
    std::string in;
    sf::Uint8 type;
    packet >> type >> in;

    std::cout << "Server" << int(type) << ": \"" << in << "\"" << std::endl;

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

    // exchangeHellos();

}
