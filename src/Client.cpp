#include "Client.h"

int clientNum = 0;

sf::IpAddress IPAddress;
unsigned short serverPort;
sf::TcpSocket socket;




void activateClient()
{
    IPAddress = "127.0.0.1";
    serverPort = 23636;
    sf::Socket::Status status = socket.connect(IPAddress, serverPort);
    if (status != sf::Socket::Done)
        std::cout << "Failed to connect to... \n" << IPAddress << ":" << serverPort << std::endl;

    std::cout << "Probably connected!";


}
